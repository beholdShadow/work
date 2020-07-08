#ifndef UDP_COM_H
#define UDP_COM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include<sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "cJSON.h"

struct info
{
    char *ip_addr;
    char *mac_addr;
    char *device_name;
    char *device_type;

};

typedef struct info server_info_t;

#endif // UDP_COM_H
