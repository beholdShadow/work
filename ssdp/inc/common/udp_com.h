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
    char ip_addr[IP_ADDR_LEN];
    char mac_addr[MAC_ADDR_LEN];
    char device_name[INFO_LEN];
    char device_type[INFO_LEN];

};

typedef struct info server_info_t;

typedef int32_t (*mw_stop_cb_t)(void);

#endif // UDP_COM_H
