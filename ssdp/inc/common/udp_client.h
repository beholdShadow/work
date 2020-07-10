//#ifndef UDP_CLIENT_H
//#define UDP_CLIENT_H

//#include "udp_com.h"

//typedef int32_t (*mw_client_cb_t)(server_info_t *p_info);

//struct client_handle
//{
//    char *mw_multicast_ip;
//    int32_t mw_multicast_port;
//    char *mw_request_device_type;

//    mw_client_cb_t server_online_cb;
//    mw_client_cb_t server_offline_cb;
//    mw_client_cb_t search_response_cb;
//    mw_stop_cb_t client_stop_cb;
//};

//typedef struct client_handle* mw_client_handle_t;

////client模块启动
//void udp_client_start(mw_client_handle_t client_handle, uv_loop_t *loop);
////client模块重载
//void udp_client_reload(mw_client_handle_t client_handle);
////client search
//void udp_client_search(mw_client_handle_t client_handle);

////client模块终止
//void udp_client_stop();

////通过JSON打包client的request信息
//char* set_client_mluticast_json(const char *pack_type);

////解析mw_server_handle_t句柄，获取设备和组播的相关信息
//void parse_handle(mw_client_handle_t handle);

////通过JSON解析接受的组播消息
//void parse_mluticast_json(const char* data);

////初始化ssdp套接字,设置组播的ip地址、端口等基本信息
//int32_t init_ssdp_socket(void);

////设备接入网络后向组播网络发送初始包，通知组内client更新信息
//int32_t ssdp_search(void);

//// 以下均是回调函数

//void uv_alloc_buf(uv_handle_t* handle,
//                size_t suggested_size,
//                uv_buf_t* buf);

//void after_uv_udp_recv(uv_udp_t* handle,
//                    ssize_t nread,
//                    const uv_buf_t* buf,
//                    const struct sockaddr* addr,
//                    unsigned flags);

//void after_uv_udp_send(uv_udp_send_t* req, int status);

//void close_cb(uv_handle_t* handle);

//typedef struct client_handle* mw_client_handle_t;

//#endif // UDP_CLIENT_H
