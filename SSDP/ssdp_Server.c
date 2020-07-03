/*
 * ===========================================================================
 *
 *       Filename:  ssdpServer.c
 *       Description:  设备发现/通知服务（自实现ssdp协议）
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
#include<sys/ioctl.h>
#include <errno.h>


#define MAC_ADDR_LEN          20
#define IP_ADDR_LEN              16
#define MAXSIZE                 512
#define BroadcastAddress  "224.0.0.88"
#define PORT 8888
#define  NET_CARD_NAME  "eno1" //网卡名称

unsigned char Local_Ipaddr[IP_ADDR_LEN]={0};
unsigned char Local_Macaddr[MAC_ADDR_LEN]={0};
 int ssdp_sock;
 struct  sockaddr_in   ssdp_addrin;

enum   Msg_Type{Notify, Discovery,Response} ;

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
	struct sockaddr_in sin;

                sock = socket(AF_INET, SOCK_STREAM, 0);  
                strcpy(ifr.ifr_name, NET_CARD_NAME);  

                res = ioctl(sock, SIOCGIFADDR, &ifr);  
                if(res == -1)
                {
                                perror("ioctl \n");
                                return  -1;
                }

                memcpy(&sin, &ifr.ifr_dstaddr, sizeof(sin));
	strcpy(ipaddr,inet_ntoa(sin.sin_addr));

                for(int i = 0; i < 6; ++i)  
                sprintf(macaddr + 3*i, "%02x:", (unsigned char)ifr.ifr_hwaddr.sa_data[i]);  
                macaddr[17]='\0';

                close(sock);
                return 0;  
 }

//解析查询的请求并及时响应
void  handle_discovery(unsigned char *recv_data, struct  sockaddr_in* clientaddr,socklen_t  addrlen)
{
                struct Device_Msg  Request,Res;
                char  buf[MAXSIZE]={0};

                memcpy(&Request,recv_data,sizeof(struct Device_Msg));

                if(Request.type == Discovery )     
                {
                                memset(buf,0,sizeof(buf));

                                Res.type=Response;
                                strncpy(Res.ipaddr,Local_Ipaddr,sizeof(Local_Ipaddr));
                                strncpy(Res.macaddr,Local_Macaddr,sizeof(Local_Macaddr));

                                memcpy(buf, &Res, sizeof(struct Device_Msg));

                                sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)clientaddr,addrlen);           
                }                                           
                else
                {
                                printf("Other device notify  message\n");
                }
                
            
}

//通过ssdp socket从组播地址 239.255.255.250 接受client和其他的device发送的数据
int ssdp_discovery(void)
{
                struct  sockaddr_in   client_addrin;
                socklen_t  client_len=sizeof(client_addrin);
                
                char buf[MAXSIZE];
                int length;
                printf("ssdp_sock=%d\n",ssdp_sock);
                while(1)
                {
                                memset(buf,0,sizeof(buf));
                                
                                length=recvfrom(ssdp_sock,buf,sizeof(buf),0,(struct sockaddr*)&client_addrin,&client_len);
                                if(length == -1)
                                {
                                                perror("ssdp_sock recvfrom\n");
                                                return -1;
                                }
                                buf[length]='\0';
                                printf("%s\n",buf);
                                handle_discovery(buf, &client_addrin,client_len);
                }

                return 0;  
}

//初始化ssdp套接字,设置组播地址
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
                printf("ssdp_sock=%d\n",ssdp_sock);

                if(bind(ssdp_sock, (struct sockaddr *)&ssdp_addrin, sizeof(ssdp_addrin))<0) 
                 {
                                 perror("ssdp_sock bind"); 
                                 return -1;
                } 

                struct ip_mreq mreq;
                mreq.imr_multiaddr.s_addr = inet_addr(BroadcastAddress);
                mreq.imr_interface.s_addr =htonl(INADDR_ANY);

                if(setsockopt(ssdp_sock, IPPROTO_IP,IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
                {
                                perror("fail to setsockopt");
                                return -1;
                }

                return 0;
}

int ssdp_notify(void)
{
                char  buf[MAXSIZE]={0};
                struct Device_Msg tmp; 

                tmp.type=Notify;
                strncpy(tmp.ipaddr,Local_Ipaddr,sizeof(Local_Ipaddr));
                strncpy(tmp.macaddr,Local_Macaddr,sizeof(Local_Macaddr));
                printf("%s  %s\n",tmp.ipaddr,tmp.macaddr);

                memcpy(buf, &tmp, sizeof(struct Device_Msg));
                printf("buf=%s\n",buf);

                return sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)&ssdp_addrin,sizeof(ssdp_addrin));           

}

int main(int argc,char *argv[])
{
                if(get_address(Local_Ipaddr,Local_Macaddr ) ==  -1)
                {
                                perror("get_address\n");
                                return -1;
                }

                // if(init_ssdp_sock( )==  -1)
                // {
                //                 perror("init_ssdp_sock");
                //                 return -1;
                // }

              printf("ip=%s\nmac=%s\n",Local_Ipaddr,Local_Macaddr);

//                 if((ret=ssdp_notify( ))== -1)
//                 {
//                                 perror("init_ssdp_sock\n");
//                                 return -1;
//                 }


//                if(ssdp_discovery()== -1)
//                {
//                                 perror("ssdp_discovery\n");
//                                 return -1; 
//                }

                return 0;
}