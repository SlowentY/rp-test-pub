#include "sdrapi.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int serviceHandler(uint8_t* buf, size_t bufsize, int sock)
{
    uint8_t command = 0;
    memcpy(&command, buf, sizeof(uint8_t));

    if(command == SDR_SIG)
    {
        uint32_t fbsize = 0;
        memcpy(&fbsize, buf + 1, sizeof(uint32_t));

        uint32_t dec_factor = 0;
        memcpy(&dec_factor, buf + 5, sizeof(uint32_t));

        uint32_t num_buffers = 0;
        memcpy(&num_buffers, buf + 5, sizeof(uint32_t));

        buf[0] = SDR_RCV_SIGPAC
        send(sock, buf, 1, 0);

        float* fbuffer = new float[fbsize]
        uint32_t seek = 0;
        uint32_t datasize = 0;

        while (true) {
            int bytes = recv(sock, buf, 1024, 0);
            if(buf[0] == SDR_EOB || buf[0] == SDR_ERROR) break;
            memcpy(&datasize, buf + 1, sizeof(uint32_t));
            memcpy(fbuffer + seek, buf + 5, datasize * sizeof(float));
            seek += datasize * sizeof(float);
            buf[0] = SDR_RCV_SIGPAC;
            send(sock, buf, 1, 0);
        }

        genSignal(fbuffer, fbsize * 2, dec_factor, num_buffers);
    }
    else if(command == SDR_GPIO)
    {
        uint8_t pinout = 0;
        memcpy(&pinout, buf + 1, sizeof(uint8_t));
        setGPIO(pinout);
        buf[0] = SDR_GPIO;
        send(sock, buf, 1, 0);
    }
}

int main()
{
    return 0;
}