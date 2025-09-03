#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SDR_SIG 0x01
#define SDR_RCV_SIGPAC 0x11
#define SDR_EOB 0x21
#define SDR_GPIO 0x02
#define SDR_ERROR 0xFF

#ifndef __SDRAPI_H__
#define __SDRAPI_H__

int genSignal(float* buffer, uint32_t bsize, uint32_t dec_factor, uint32_t num_buffers);
int setGPIO(uint8_t pinout);

#endif