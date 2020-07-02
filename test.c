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

#define MAC_ADDR_LEN (16)
#define IP_ADDR_LEN (16)
#define maxsize 100
#define  NET_CARD_NAME  "eno1" //网卡名称

enum   flag{Notify, Discovery} Msg_Type ;

struct  Msg
{
                Msg_Type  type;
                unsigned char ipaddr[16];
                unsigned char macaddr[16];
}Device_Msg;

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
                                printf("ioctl error\n")；
                                return  -1；
                }

                ipaddr= (struct sockaddr_in *)&(ifr.ifr_addr);

                for(int i = 0; i < 6; ++i)  
                sprintf(macaddr + 3*i, "%02x:", (unsigned char)ifr.ifr_hwaddr.sa_data[i]);  

                return 0;  
 }

void  handle_discovery(unsigned char *buf, Device_Msg *data )
{
                unsigned char *p=buf;

                while(p)
                {
                                memcpy(data, buf, sizeof(Device_Msg));
                                
                                p+=sizeof(Device_Msg);
                                data++;
                                
                }

}

int ssdp_discovery()
{
                struct  sockaddr_in   addrin;
                int ssdp_sock;
                char *buf[maxsize];

                bzero(&addrin, sizeof(addrin));  
                addrin.sin_family = AF_INET;  
                //addrin.sin_addr.s_addr = inet_addr("0.0.0.0"); //htonl(INADDR_ANY)  
                addrin.sin_addr.s_addr = inet_addr("239.255.255.250"); 
                addrin.sin_port = htons(1900);  

                ssdp_sock=socket(AF_INET,SOCK_DGRAM,0);  
                
                int res;
                while(1)
                {
                                res=recvfrom(ssdp_sock,buf,sizeof(buf),0,&addrin,sizeof(addrin));
                                if(res == -1)
                                {
                                                printf("recvfrom error\n")；
                                                return -1;
                                }

                                handle_discovery(unsigned char *buf, Device_Msg *data );

                }
                
                
}


int main()
{
                char  ipaddr[IP_ADDR_LEN]={0},macaddr[MAC_ADDR_LEN]={0};

                get_address(ipaddr,macaddr);

               printf("ip=%s\n mac=%s\n",ipaddr,macaddr);

                return 0;
}