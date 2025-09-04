#include "sdrapi.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int serviceHandler( int sock, uint8_t* buf, size_t bufsize)
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
        buf[0] = SDR_SIG;
        send(sock, buf, 1, 0);
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
    if(argc < 1)
    {                                                                                                                                                                                                                                                                               
            fprintf(stderr, "Invalid argc!\n");
            return -1;
    }                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                    
    int sock,  listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
                                                                                                                                                                                                                                                                                    
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {                                                                                                                                                                                                                                                                               
            fprintf(stderr, "Socket error!\n");
            return -1;
    }                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                    
    int port = atoi(argv[0])
                                                                                                                                                                                                                                                                                    
    addr.sin_family = AF_INET;                                                                                                                                                                                                                                                      
    addr.sin_port = htons(port);                                                                                                                                                                                                                                                    
    addr.sin_addr.s_addr = htonl(INADDR_ANY)                                                                                                                                                                                                                                        
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {                                                                                                                                                                                                                                                                               
            fprintf(stderr, "Bind error!\n");
            return -1;
    }                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                    
    listen(listener, 1);

    while(true)
    {                                                                                                                                                                                                                                                                               
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {                                                                                                                                                                                                                                                                       
                fprintf(stderr, "Accept error!\n");
                return -1;
        }                                                                                                                                                                                                                                                                       
                                                                                                                                                                                                                                                                                
        while(true)
        {                                                                                                                                                                                                                                                                       
                bytes_read = recv(sock, buf, 1024, 0);
                if(bytes_read <= 0) break;
                serviceHandler(sock, buf, bytes_read);                                                                                                                                                                                                                             
        }                                                                                                                                                                                                                                                                       
    } 

    return 0;
}