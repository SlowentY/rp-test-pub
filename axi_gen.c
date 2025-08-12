#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rp.h"
#include "rp_asg_axi.h"

int main(int argc, char **argv) {

    // Select channel
    rp_channel_t channel = RP_CH_1;

    // Data size in samples (16 bit)
    uint32_t bufferSize = 4096;             // Should be twice the actual number of samples
    uint32_t dec_factor = 1;
    uint32_t num_buffers = 1;

    uint32_t ncyc = 2;
    uint32_t nor = 2;
    uint32_t period = 100;

    // Start addresses of buffers must be multiple of 4096 (DDR page size)
    uint32_t min_addr_diff = 4096;

    // Initialize the interface
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Rp api init failed!\n");
        return 1;
    }

    // Reset generator
    rp_GenReset();

    /* Setting up the Deep Memory Generation (DMG) */
    uint32_t dac_axi_start, dac_axi_size;

    // Get the reserved memory region start and size
    if (rp_AcqAxiGetMemoryRegion(&dac_axi_start, &dac_axi_size) != RP_OK) {
        fprintf(stderr, "Error get memory!\n");
        return 1;
    }

    // Lower the buffer size to the available memory size (if needed)
    uint32_t dmg_min_size = (uint32_t) (ceil((float) bufferSize / (float) min_addr_diff) * (float) min_addr_diff * (float) num_buffers);
    printf("Buffer size: %d\n", bufferSize);
    printf("Minimum size: %d\n", dmg_min_size);

    if (dmg_min_size > dac_axi_size) {
        printf("Buffer size is too large, reducing to available memory size\n");
        bufferSize = dac_axi_size / num_buffers;
        printf("Buffer size reduced to %d\n", bufferSize);
    }

    // Calculate the start address
    uint32_t gen1_start_addr = dac_axi_start;

    // Reserve memory for the generator
    printf("Reserved memory for OUT1 Start: %x Size: %x End: %x\n", gen1_start_addr, bufferSize, gen1_start_addr + bufferSize);

    if (rp_GenAxiReserveMemory(channel, gen1_start_addr, gen1_start_addr + bufferSize) != RP_OK) {
        fprintf(stderr, "Error setting address for DMG mode for OUT1\n");
        return 1;
    }

    // Set decimation factor
    if (rp_GenAxiSetDecimationFactor(channel, dec_factor) != RP_OK) {
        fprintf(stderr, "Error setting decimation for generator OUT1\n");
        return 1;
    }

    // Enable DMG mode
    if (rp_GenAxiSetEnable(channel, true) != RP_OK) {
        fprintf(stderr, "Error enable axi mode for OUT1\n");
        return 1;
    }

    // Define output waveform for both channels
    bufferSize /= 2;                // Samples are float32 which is 4 bytes, int16 is 2 bytes


    float *t = (float *)malloc(bufferSize * sizeof(float));
    float *x = (float *)malloc(bufferSize * sizeof(float));

    for (uint32_t i = 1; i < bufferSize; i++) {
        t[i] = (2 * M_PI) / bufferSize * i;
    }

    for (uint32_t i = 0; i < bufferSize; ++i) {
        x[i] = sin(t[i]) + ((1.0 / 3.0) * sin(t[i] * 3));
    }

    // Configure calibration and offset
    rp_GenSetAmplitudeAndOffsetOrigin(channel);

    // Set burst parameters
    rp_GenMode(channel, RP_GEN_MODE_BURST);
    rp_GenBurstCount(channel, ncyc);
    rp_GenBurstRepetitions(channel, nor);
    rp_GenBurstPeriod(channel, period);
    rp_GenAxiWriteWaveform(channel, x, bufferSize);

    rp_GenOutEnable(channel);
    rp_GenTriggerOnly(channel);

    // Release resources
    free(t);
    free(x);

    rp_Release();

    return 0;
}
