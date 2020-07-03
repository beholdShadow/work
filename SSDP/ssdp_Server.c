/*
 * ===========================================================================
 *
 *       Filename:  ssdpServer.c
 *       Description:  设备发现服务（自实现ssdp协议）
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
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/ioctl.h>


#define MAC_ADDR_LEN          16
#define IP_ADDR_LEN              16
#define MAXSIZE                 512
#define  NET_CARD_NAME  "eno1" //网卡名称

unsigned char Local_Ipaddr[IP_ADDR_LEN];
unsigned char Local_Macaddr[MAC_ADDR_LEN];
 int ssdp_sock;

enum   Msg_Type{Notify, Discovery,Response}  ;

struct  Device_Msg
{
                enum Msg_Type  type;
                unsigned char ipaddr[16];
                unsigned char macaddr[16];
};

//获取本地设备的IP、MAC地址
int get_address(unsigned char *ipaddr, unsigned char *macaddr)
{
                int sock;  
                int res;  
                struct ifreq ifr;  

                sock = socket(AF_INET, SOCK_STREAM, 0);  
                strcpy(ifr.ifr_name, NET_CARD_NAME);  

                res = ioctl(sock, SIOCGIFADDR, &ifr);  
                if(res == -1)
                {
                                perror("ioctl \n");
                                return  -1;
                }

                ipaddr= (struct sockaddr_in *)&(ifr.ifr_addr);

                for(int i = 0; i < 6; ++i)  
                sprintf(macaddr + 3*i, "%02x:", (unsigned char)ifr.ifr_hwaddr.sa_data[i]);  

                return 0;  
 }

//解析查询的请求并及时响应
void  handle_discovery(unsigned char *recv_data, struct  sockaddr_in* clientaddr,socklen_t  addrlen)
{
                struct Device_Msg  Request;
                char  buf[MAXSIZE]={0};
                struct Device_Msg tmp;

                while(recv_data)
                {
                                memcpy(&Request,recv_data,sizeof(struct Device_Msg));
                                
                                recv_data+=sizeof(struct Device_Msg);

                                if(Request.type == Discovery )     
                                {
                                                memset(buf,0,sizeof(buf));

                                                tmp.type=Response;
                                                strncpy(tmp.ipaddr,Local_Ipaddr,sizeof(Local_Ipaddr));
                                                strncpy(tmp.macaddr,Local_Macaddr,sizeof(Local_Macaddr));

                                                memcpy(buf, &tmp, sizeof(struct Device_Msg));

                                                sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)clientaddr,addrlen);           
                                }                                           
                                else
                                {
                                                printf("Other device notify  message\n");
                                }
                }
            
}

//通过ssdp socket从组播地址 239.255.255.250 接受client和其他的device发送的数据
int ssdp_discovery(void)
{
                struct  sockaddr_in   client_addrin;
                socklen_t  client_len;
                char buf[MAXSIZE];
                int ret;

                while(1)
                {
                                memset(buf,0,sizeof(buf));
                                
                                ret=recvfrom(ssdp_sock,buf,sizeof(buf),0,(struct sockaddr*)&client_addrin,&client_len);
                                if(ret == -1)
                                {
                                                perror("ssdp_sock recvfrom\n");
                                                return -1;
                                }
                                
                                handle_discovery(buf, &client_addrin,client_len);
                }
                         
}

int init_ssdp_sock(void)
{
                 struct  sockaddr_in   addrin, 
                 bzero(&addrin, sizeof(addrin));  
                addrin.sin_family = AF_INET;  
                addrin.sin_addr.s_addr = inet_addr("239.255.255.250"); 
                addrin.sin_port = htons(1900);  
                
                ssdp_sock=socket(AF_INET,SOCK_DGRAM,0);  
                ret=bind(ssdp_sock, (struct sockaddr *)&addrin, sizeof(addrin));  
                if( ret < 0 )  
                 {
                                 perror("ssdp_sock bind\n"); 
                                 return -1;
                } 
}

int ssdp_notify(void)
{
                struct Device_Msg  Request;
                char  buf[MAXSIZE]={0};
                struct Device_Msg tmp; 

                tmp.type=Notify;
                strncpy(tmp.ipaddr,Local_Ipaddr,sizeof(Local_Ipaddr));
                strncpy(tmp.macaddr,Local_Macaddr,sizeof(Local_Macaddr));

                memcpy(buf, &tmp, sizeof(struct Device_Msg));

                sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)clientaddr,addrlen);           

}

int main()
{
                int ret;

                if(ret=init_ssdp_sock( )== -1)
                {
                                perror("init_ssdp_sock\n");
                                return -1;
                }

                if(ret=init_ssdp_sock( )== -1)
                {
                                perror("init_ssdp_sock\n");
                                return -1;
                }

                if(ret=get_address(Local_Ipaddr,Local_Macaddr )== -1)
                {
                                perror("ssdp_notify\n");
                                return -1;
                }

               printf("ip=%s\n mac=%s\n",Local_Ipaddr,Local_Macaddr);

               if((ret=ssdp_discovery())== -1)
               {
                                perror("ssdp_discovery\n");
                                return -1; 
               }

                return 0;
}