/*
* ===========================================================================
*
*       Filename:  mw_ssdp_server.h
*       Description:  自实现SSDP协议设备发现/通知服务
*       Version:  1.0
*       Compiler:  gcc_64
*
* ===========================================================================
*/
#ifndef MW_SSDP_SERVER_H
#define MW_SSDP_SERVER_H

#include "common/udp_server.h"

/*
struct server_handle
{
   char *mw_mulitcast_Ip;
   char *mw_device_name;
   char *mw_device_type;
   int32_t mw_mulitcast_port;

   char buf[512];
};

typedef struct server_handle* mw_server_handle_t;
*/

/*
* =====================================================================
*   FunctionName: mw_server_handle_create
*
*   Purpose:      创造mw_server_handle_t句柄
*
*   Parameter:    handle:句柄
*
*   Return:        return  -1  if   error  else return  0
* ======================================================================
*/
int32_t mw_server_handle_create(mw_server_handle_t *handle);

/*
* =====================================================================
*   FunctionName: mw_server_handle_destory
*
*   Purpose:      mw_server_handle_t
*
*   Parameter:    handle:句柄
*
*   Return:       void
* ======================================================================
*/
void mw_server_handle_destory(mw_server_handle_t handle);

/*
* =====================================================================
*   FunctionName: mw_server_set_info
*
*   Purpose:        设置设备的属性和组播的信息
*
*   Parameter:      mw_mulitcast_Ip:   设备加入的多播ip地址
*                   mw_mulitcast_port: 设备加入的多播端口
*
*   Return:        return  -1  if   error  else return  0
* ======================================================================
*/
int32_t mw_server_set_info(mw_server_handle_t handle,
                           char *mw_mulitcast_Ip,
                           char *mw_device_name,
                           char *mw_device_type,
                           int32_t mw_mulitcast_port);

/*
* ===========================================================================
*   FunctionName:  mw_server_start
*
*   Purpose:        设备启动,加入多播组,通过broadcast向client发送设备相关信息
*
*   Parameter:      handle: mw_server_handle_t句柄
*
*   Return:         return  -1  if   error  else return  0
* ===========================================================================
*/
int32_t mw_server_start(mw_server_handle_t handle);

/*
* ===========================================================================
*   FunctionName:  mw_server_stop
*
*   Purpose:        设备停止,退出多播组,通过broadcast向client发送退出消息
*
*   Parameter:      void
*
*   Return:         return  -1  if   error  else return  0
* ===========================================================================
*/
int32_t mw_server_stop(void);

#endif // MW_SSDP_SERVER_H
