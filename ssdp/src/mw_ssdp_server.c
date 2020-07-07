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
#include <signal.h>
#include <cjson/cJSON.h>

#define MAC_ADDR_LEN        20
#define IP_ADDR_LEN         16
#define MAXSIZE             512

unsigned char Local_Ipaddr[IP_ADDR_LEN]={0};
unsigned char Local_Macaddr[MAC_ADDR_LEN]={0};
unsigned char BroadcastAddress[IP_ADDR_LEN] ="224.0.0.88";
int PORT=8888;
unsigned char Device_Name[50]="vaya-ubuntu-eno1";

 int ssdp_sock;
 struct  sockaddr_in   ssdp_addrin;

//获取本地设备的IP、MAC地址
int get_address(unsigned char *ipaddr, unsigned char *macaddr);

//初始化ssdp套接字,设置组播的ip地址、端口等基本信息
int init_ssdp_sock(void);

//设备接入网络后向组播网络发送初始包，通知组内client更新信息
int ssdp_notify(void);

//设备定时向组播上发送自身信息
int ssdp_loop_send(void);

//SIGALRM定时触发的函数
void timed_trigger (int signo)
{
    if(ssdp_loop_send() == -1)
    {
        perror(" ssdp_loop_send()");
    }
}

//设备组播发包的定时信号的初始化函数
void init_sigaction (void)
{
    struct sigaction tact;

    tact.sa_handler = timed_trigger;

    tact.sa_flags = 0;

    sigemptyset (&tact.sa_mask);

    sigaction (SIGALRM, &tact, NULL);

}

//设备定时的时间初始化
void init_time ()
{
    struct itimerval value;

    value.it_value.tv_sec = 10;

    value.it_value.tv_usec = 0;

    value.it_interval = value.it_value;

    setitimer (ITIMER_REAL, &value, NULL);

}

//通过JSON打包设备的相关信息
char* set_Device_Mluticast_Json(const char*Type)
{
    cJSON *root=cJSON_CreateObject();

    cJSON_AddItemToObject(root,"Type",cJSON_CreateString(Type));
    cJSON_AddItemToObject(root,"DeviceName",cJSON_CreateString(Device_Name));
    cJSON_AddItemToObject(root,"IpAddr",cJSON_CreateString(Local_Ipaddr));
    cJSON_AddItemToObject(root,"MacAddr",cJSON_CreateString(Local_Macaddr));

    char *str=cJSON_Print(root);

    cJSON_Delete(root);

    return str;
}

int get_address(unsigned char *ipaddr, unsigned char *macaddr)
{
        int sock;
        unsigned char buf[MAXSIZE]={0};
        struct ifreq* ifr;
        struct ifconf ifc;

        ifc.ifc_len=MAXSIZE;
        ifc.ifc_buf=buf;

        if((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
        {
                        perror("socket ");
                        return  -1;
        }

        if(ioctl(sock, SIOCGIFCONF, &ifc) == -1)
        {
                        perror("ioctl ");
                        return  -1;
        }

        ifr=(struct ifreq*)buf;
        for(int i=(ifc.ifc_len/sizeof(struct ifreq)); i>0; i--)
        {
            // printf("name = [%s]\n", ifr->ifr_name);
            // printf("local addr = [%s]\n",  inet_ntoa(((struct sockaddr_in*)&(ifr->ifr_addr))->sin_addr));

            strcpy(ipaddr, inet_ntoa(((struct sockaddr_in*)&(ifr->ifr_addr))->sin_addr));
            for(int i = 0; i < 6; ++i)
                sprintf(macaddr + 3*i, "%02x:", (unsigned char)ifr->ifr_hwaddr.sa_data[i]);

            //选择非本地回环地址
            if(strcmp(ipaddr,"127.0.0.1")  != 0)
                        break;

            macaddr[17]='\0';

            ifr++;
        }

        close(sock);

        return 0;
}

int init_ssdp_sock(void)
{
    bzero(&ssdp_addrin, sizeof(ssdp_addrin));
    ssdp_addrin.sin_family = AF_INET;
    ssdp_addrin.sin_port = htons(PORT);
    ssdp_addrin.sin_addr.s_addr = inet_addr("224.0.0.88");


    ssdp_sock=socket(AF_INET,SOCK_DGRAM,0);
    if( ssdp_sock < 0 )
    {
         perror("ssdp_sock socket\n");
         return -1;
    }

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
    if(get_address(Local_Ipaddr,Local_Macaddr ) ==  -1)
    {
        perror("get_address\n");
        return -1;
    }

    char* buf=set_Device_Mluticast_Json("NOTIFY");
    if(buf == NULL)
    {
        perror("set_Device_Mluticast_Json");
        return -1;
    }
    printf("%s\n",buf);
    sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)&ssdp_addrin,sizeof(ssdp_addrin));

    free(buf);

    return 0;
}

int ssdp_loop_send(void)
{
    if(get_address(Local_Ipaddr,Local_Macaddr ) ==  -1)
    {
        perror("get_address\n");
        return -1;
    }

    char* buf=set_Device_Mluticast_Json("TIMING");
    if(buf == NULL)
    {
        perror("set_Device_Mluticast_Json");
        return -1;
    }

    printf("%s",buf);

    sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)&ssdp_addrin,sizeof(ssdp_addrin));

    free(buf);

    return 0;

}

int ssdp_quit(void)
{
    if(get_address(Local_Ipaddr,Local_Macaddr ) ==  -1)
    {
        perror("get_address\n");
        return -1;
    }

    char* buf=set_Device_Mluticast_Json("LEAVE");
    if(buf == NULL)
    {
        perror("set_Device_Mluticast_Json");
        return -1;
    }

    sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)&ssdp_addrin,sizeof(ssdp_addrin));

    free(buf);

    return 0;
}

int main(int argc,char *argv[])
{
    init_sigaction();

    init_time ();

    if(init_ssdp_sock( )==  -1)
    {
        perror("init_ssdp_sock");
        return -1;
    }

    if(ssdp_notify( )== -1)
    {
        perror("ssdp_notify\n");
        return -1;
    }

    while(1);

    return 0;
}



//
//解析查询的请求并及时响应
// void  handle_discovery(unsigned char *recv_data, struct  sockaddr_in* clientaddr,socklen_t  addrlen)
// {
//                 struct Device_Msg  Request,Res;
//                 char  buf[MAXSIZE]={0};

//                 memcpy(&Request,recv_data,sizeof(struct Device_Msg));

//                 if(Request.type == Discovery )     
//                 {
//                                 memset(buf,0,sizeof(buf));

//                                 Res.type=Response;
//                                 strncpy(Res.ipaddr,Local_Ipaddr,sizeof(Local_Ipaddr));
//                                 strncpy(Res.macaddr,Local_Macaddr,sizeof(Local_Macaddr));

//                                 memcpy(buf, &Res, sizeof(struct Device_Msg));

//                                 sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)clientaddr,addrlen);           
//                 }                                           
//                 else
//                 {
//                                 printf("Other device notify  message\n");
//                 }
                
            
// }

//通过ssdp socket从组播地址 239.255.255.250 接受client和其他的device发送的数据
// int ssdp_discovery(void)
// {
//                 struct  sockaddr_in   client_addrin;
//                 socklen_t  client_len=sizeof(client_addrin);
                
//                 char buf[MAXSIZE];
//                 int length;
//                 printf("ssdp_sock=%d\n",ssdp_sock);
//                 while(1)
//                 {
//                                 memset(buf,0,sizeof(buf));
                                
//                                 length=recvfrom(ssdp_sock,buf,sizeof(buf),0,(struct sockaddr*)&client_addrin,&client_len);
//                                 if(length == -1)
//                                 {
//                                                 perror("ssdp_sock recvfrom\n");
//                                                 return -1;
//                                 }
//                                 buf[length]='\0';
//                                 printf("%s\n",buf);
//                                 strcpy(buf,"Server get your message");
//                                 sendto(ssdp_sock,buf,strlen(buf),0,(struct sockaddr*)&client_addrin,client_len);           
//                                 //handle_discovery(buf, &client_addrin,client_len);
//                 }

//                 return 0;  
// }
