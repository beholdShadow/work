#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "udp_com.h"

struct server_handle
{
    char *mw_mulitcast_Ip;
    char *mw_device_name;
    char *mw_device_type;
    int32_t mw_mulitcast_port;

    char buf[512];
};

typedef struct server_handle* mw_server_handle_t;

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
char* set_device_mluticast_json(const char *pack_type);

//解析查询的请求并组成JSON包返回
 char*  handle_discovery(unsigned char *recv_data);

//通过ssdp_socket从组播地址接受client的search请求的json数据包,处理结果通过单播响应给client
int ssdp_discovery(void);

#endif // UDP_SERVER_H
