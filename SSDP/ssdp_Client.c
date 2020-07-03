/*
 * ===========================================================================
 *
 *       Filename:  ssdpCLient.c
 *       Description:  客户端查询服务（自实现ssdp协议）
 *        Version:  1.0
 *       Compiler:  gcc
 *
 * ===========================================================================
 */

#include <sys/types.h>
#include <sys/select.h>
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
#include<sys/ioctl.h>


#define MAC_ADDR_LEN          16
#define IP_ADDR_LEN              16
#define MAXSIZE                 512
#define BroadcastAddress  "224.0.0.88"
#define PORT 8888

 int ssdp_sock;
 struct  sockaddr_in   ssdp_addrin;

enum   Msg_Type{Notify, Discovery,Response}  ;

struct  Device_Msg
{
                enum Msg_Type  type;
                unsigned char ipaddr[16];
                unsigned char macaddr[16];
};

//解析查询的请求并及时响应
void  handle_discovery(unsigned char *recv_data)
{
                struct Device_Msg  Res;
                char  buf[MAXSIZE]={0};
                struct Device_Msg tmp;

                while(recv_data)
                {
                                memcpy(&Res,recv_data,sizeof(struct Device_Msg));

                                if(Res.type == Response ||  Res.type  == Notify)     
                                {
                                                printf("device IPaddress=%s     MACaddress=%s\n",Res.ipaddr,Res.macaddr);
                                }                                           
                                else if(Res.type == Discovery )     
                                {
                                                printf("Other client discovery request\n");
                                }
                                recv_data+=sizeof(struct Device_Msg);
                }


}

//client通过ssdp socket向组播地址 239.255.255.250 发送查询device设备请求
int ssdp_discovery(void)
{
                int ret;
                char  buf[MAXSIZE]={0};
                struct Device_Msg tmp={Discovery,NULL,NULL};

                memcpy(buf, &tmp, sizeof(struct Device_Msg));

                sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)&ssdp_addrin,sizeof(ssdp_addrin));           
                     
                ret=recvfrom(ssdp_sock,buf,sizeof(buf),0,NULL,NULL);
                if(ret == -1)
                {
                                perror("ssdp_sock recvfrom\n");
                                return -1;
                }
                                
                handle_discovery(buf);     

                return 0;
                         
}

int init_ssdp_sock(void)
{
                bzero(&ssdp_addrin, sizeof(ssdp_addrin));  
                ssdp_addrin.sin_family = AF_INET;  
                ssdp_addrin.sin_port = htons(PORT);
                ssdp_addrin.sin_addr.s_addr = htonl(INADDR_ANY);
                
                ssdp_sock=socket(AF_INET,SOCK_DGRAM,0);  
                if( ssdp_sock < 0 )  
                 {
                                 perror("ssdp_sock socket\n"); 
                                 return -1;
                } 

                return 0;
}

int main()
{
                if(init_ssdp_sock( )== -1)
                {
                                perror("init_ssdp_sock\n");
                                return -1;
                }

               if(ssdp_discovery()== -1)
               {
                                perror("ssdp_discovery\n");
                                return -1; 
               }

                return 0;
}