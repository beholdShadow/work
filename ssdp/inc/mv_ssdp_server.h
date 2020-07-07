/*
 * ===========================================================================
 *
 *       Filename:  Server.h
 *       Description:  自实现服务器SSDP协议的访问接口API
 *        Version:  1.0
 *       Compiler:  gcc
 *
 * ===========================================================================
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>

/*
 * ===========================================================================
 *
 *       FunctionName:： Server_Start
 * 
 *       Purpose：             设备启动,加入多播组,通过broadcast向client发送设备相关信息
 * 
 *       Parameter：        Mulitcast_Ip ：入参 ； 设备加入的多播ip地址    
*                                           Mulitcast_Ip ：入参 ；设备加入的多播端口 

*       Return：                 return  -1  if   error  else return  0                          
 *
 * ===========================================================================
 */
int Server_Start(void *Mulitcast_Ip, int Mulitcast_Port);

/*
 * ===========================================================================
 *
 *       FunctionName:： Server_Stop
 * 
 *       Purpose：             设备停止,退出多播组,通过broadcast向client发送退出消息
 * 
 *       Parameter：        void：空

*       Return：                 return  -1  if   error  else return  0                          
 *
 * ===========================================================================
 */
int Server_Stop(void);