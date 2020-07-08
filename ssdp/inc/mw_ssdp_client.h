/*
* ===========================================================================
*
*       Filename:  mv_ssdp_client.h
*       Description:  自实现SSDP协议客户端查询/接收服务
*       Version:  1.0
*       Compiler:  gcc_64
*
* ===========================================================================
*/
#ifndef MV_SSDP_CLIENT_H
#define MV_SSDP_CLIENT_H
#include "common/udp_client.h"

/*
struct client_handle
{
   char *mw_mulitcast_Ip;
   int32_t mw_mulitcast_port;
   char *mw_request_device_type;

   p_callback server_online_cb;
   p_callback server_offline_cb;
};

typedef struct cleint_handle* mw_client_handle_t;
*/

/*
* =====================================================================
*   FunctionName: mv_client_handle_create
*
*   Purpose:      创造mw_client_handle_t句柄
*
*   Parameter:    handle:句柄
*
*   Return:        return  -1  if   error  else return  0
* ======================================================================
*/
int32_t mv_client_handle_create(mw_client_handle_t *handle);

/*
* =====================================================================
*   FunctionName: mv_client_handle_destory
*
*   Purpose:      销毁mw_client_handle_t句柄
*
*   Parameter:    handle:句柄
*
*   Return:       void
* ======================================================================
*/
void mv_client_handle_destory(mw_client_handle_t handle);
/*
* =====================================================================
*   FunctionName: mw_client_set_info
*
*   Purpose:        设置client加入的组播的信息
*
*   Parameter:      handle:句柄
*                   mw_mulitcast_Ip:   client加入的多播ip地址
*                   mw_mulitcast_port: client加入的多播端口
*
*   Return:        return  -1  if   error  else return  0
* ======================================================================
*/
int32_t mw_client_set_info(mw_client_handle_t handle,
                           char *mw_mulitcast_Ip,
                           int32_t mw_mulitcast_port
                           );

/*
* =====================================================================
*   FunctionName: mw_client_set_callback
*
*   Purpose:        设置client回调函数
*
*   Parameter:      handle:句柄
*
*   Return:        return  -1  if   error  else return  0
* ======================================================================
*/
int32_t mw_client_set_callback(mw_client_handle_t handle,
                           p_callback server_online_cb,
                           p_callback server_offline_cb,
                           p_callback client_search_cb_
                           );

/*
* =====================================================================
*   FunctionName:  mw_client_search
*
*   Purpose:        client主动search网络中的设备信息
*
*   Parameter:      handle:句柄
*                   mw_request_device_type: 查询的设备类型
*
*   Return:        return  -1  if   error  else return  0
* ======================================================================
*/

int32_t mw_client_search(mw_client_handle_t handle,char *mw_request_device_type);

/*
* ===========================================================================
*   FunctionName:  mw_client_start
*
*   Purpose:        启动client，加入多播组，从组播地址获取网络中的设备信息
*
*   Parameter:      handle: 句柄
*
*   Return:         return  -1  if   error  else return  0
* ===========================================================================
*/
int32_t mw_client_start(mw_client_handle_t handle);

/*
* ===========================================================================
*   FunctionName:  mw_cleint_stop
*
*   Purpose:       结束client，退出多播组
*
*   Parameter:      void
*
*   Return:         return  -1  if   error  else return  0
* ===========================================================================
*/
int32_t mw_client_stop(void);

#endif // MW_SSDP_CLIENT_H
