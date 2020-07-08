#include "../inc/common/udp_server.h"

int main(int argc,char *argv[])
{
    init_sigaction();

    init_time (10);

    if(init_ssdp_sock( )==  -1)
    {
        perror("init_ssdp_sock");
        return -1;
    }

    if(ssdp_notify( )== -1)
    {
        perror("ssdp_notify\n");
        return -1;
    }

    while(1);

    return 0;
}
