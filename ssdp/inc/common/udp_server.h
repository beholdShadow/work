#ifndef UDP_MULTICAST_H
#define UDP_MULTICAST_H

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

//获取本地设备的IP、MAC地址
int32_t get_address(unsigned char *ipaddr, unsigned char *macaddr);

//初始化ssdp套接字,设置组播的ip地址、端口等基本信息
int32_t init_ssdp_sock(void);

//设备接入网络后向组播网络发送初始包，通知组内client更新信息
int32_t ssdp_notify(void);

//设备定时向组播上发送自身信息
int32_t ssdp_loop_send(void);

//SIGALRM定时触发的函数
void time_trigger(int32_t signo);

//设备组播发包的定时信号的初始化函数
void init_sigaction (void);

//初始化设备定时的时间
void init_time ();

//通过JSON打包设备的相关信息
char* set_device_mluticast_json(const char* pack_type);

#endif // UDP_BROADCAST_H
