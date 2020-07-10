#ifndef UDP_COM_H
#define UDP_COM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/ioctl.h>
#include <net/if.h>

#include "uv.h"
#include "cJSON.h"

#define MAC_ADDR_LEN        20
#define IP_ADDR_LEN         16
#define MAXSIZE             512
#define INFO_LEN           100

struct info
{
    char *ip_addr;
    char *mac_addr;
    char *device_name;
    char *device_type;

};

typedef struct info server_info_t;

#endif // UDP_COM_H
