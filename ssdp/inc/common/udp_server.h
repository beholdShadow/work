#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "udp_com.h"

struct server_handle
{
    char *mw_multicast_ip;
    int32_t mw_multicast_port;

    char *mw_device_name;
    char *mw_device_type;

    char buf[512];

    mw_stop_cb_t server_stop_cb;
};

typedef struct server_handle* mw_server_handle_t;

//服务器模块启动
void udp_server_start(mw_server_handle_t server_handle, uv_loop_t *loop);
//服务器模块重载
void udp_server_reload(mw_server_handle_t server_handle);
//服务器模块终止
void udp_server_stop();

//通过JSON打包设备的相关信息
char* set_device_mluticast_json(const char *pack_type);

//通过JSON解析接受的组播消息
char* parse_mluticast_json(const char *data);

//解析mw_server_handle_t句柄，获取设备和组播的相关信息
void parse_handle(mw_server_handle_t handle);

//获取本地设备的IP、MAC地址
int32_t get_address(char *ip_addr, char *mac_addr);

//初始化定时器信息
int32_t init_timer(void);

//初始化ssdp套接字,设置组播的ip地址、端口等基本信息
int32_t init_ssdp_socket(void);

//设备接入网络后向组播网络发送初始包，通知组内client更新信息
int32_t ssdp_notify(void);

//设备离开网络后向组播网络发出退出的消息
int32_t ssdp_quit(void);

// 以下均是回调函数

void uv_alloc_buf(uv_handle_t* handle,
                size_t suggested_size,
                uv_buf_t* buf);

void ssdp_loop_send(uv_timer_t* handle);

void after_uv_udp_recv(uv_udp_t* handle,
                    ssize_t nread,
                    const uv_buf_t* buf,
                    const struct sockaddr* addr,
                    unsigned flags);

void after_uv_udp_send(uv_udp_send_t* req, int status);

void end_uv_udp_send(uv_udp_send_t* req, int status);

void close_cb(uv_handle_t* handle);

#endif // UDP_SERVER_H
