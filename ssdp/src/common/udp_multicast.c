#include "../../inc/common/udp_multicast.h"

#define MAC_ADDR_LEN        20
#define IP_ADDR_LEN         16
#define MAXSIZE             512

unsigned char local_ipaddr[IP_ADDR_LEN]={0};
unsigned char local_macaddr[MAC_ADDR_LEN]={0};

unsigned char broadcast_address[IP_ADDR_LEN] ="224.0.0.88";
uint32_t port=8888;
unsigned char device_name[50]="vaya-ubuntu-eno1";

int32_t ssdp_sock;
struct  sockaddr_in   ssdp_addrin;

void time_trigger(int32_t signo)
{
    if(ssdp_loop_send() == -1)
        perror(" ssdp_loop_send()");
}

void init_sigaction (void)
{
    struct sigaction tact;

    tact.sa_handler = time_trigger;

    tact.sa_flags = 0;

    sigemptyset (&tact.sa_mask);

    sigaction (SIGALRM, &tact, NULL);

}

void init_time ()
{
    struct itimerval value;

    value.it_value.tv_sec = 10;

    value.it_value.tv_usec = 0;

    value.it_interval = value.it_value;

    setitimer (ITIMER_REAL, &value, NULL);

}

char* set_device_mluticast_json(const char* pack_type)
{
    char *str=NULL;

    cJSON *root=cJSON_CreateObject();

    cJSON_AddItemToObject(root,"pack_type",cJSON_CreateString(pack_type));
    cJSON_AddItemToObject(root,"device_name",cJSON_CreateString(device_name));
    cJSON_AddItemToObject(root,"ip_addr",cJSON_CreateString(local_ipaddr));
    cJSON_AddItemToObject(root,"mac_addr",cJSON_CreateString(local_macaddr));

    str=cJSON_Print(root);

    cJSON_Delete(root);

    return str;
}

int32_t get_address(unsigned char *ip_addr, unsigned char *mac_addr)
{
        int32_t sock;
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
        for(int32_t i=(ifc.ifc_len/sizeof(struct ifreq)); i>0; i--)
        {
            // printf("name = [%s]\n", ifr->ifr_name);
            // printf("local addr = [%s]\n",  inet_ntoa(((struct sockaddr_in*)&(ifr->ifr_addr))->sin_addr));

            strcpy(ip_addr, inet_ntoa(((struct sockaddr_in*)&(ifr->ifr_addr))->sin_addr));
            for(int i = 0; i < 6; ++i)
                sprintf(mac_addr + 3*i, "%02x:", (unsigned char)ifr->ifr_hwaddr.sa_data[i]);

            //选择非本地回环地址
            if(strcmp(ip_addr,"127.0.0.1")  != 0)
                        break;

            mac_addr[17]='\0';

            ifr++;
        }

        close(sock);

        return 0;
}

int32_t init_ssdp_sock(void)
{
    bzero(&ssdp_addrin, sizeof(ssdp_addrin));
    ssdp_addrin.sin_family = AF_INET;
    ssdp_addrin.sin_port = htons(port);
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
    mreq.imr_multiaddr.s_addr = inet_addr(broadcast_address);
    mreq.imr_interface.s_addr =htonl(INADDR_ANY);

    if(setsockopt(ssdp_sock, IPPROTO_IP,IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("fail to setsockopt");
        return -1;
    }

        return 0;
}

int32_t ssdp_notify(void)
{
    if(get_address(local_ipaddr,local_macaddr ) ==  -1)
    {
        perror("get_address\n");
        return -1;
    }

    char* buf=set_device_mluticast_json("NOTIFY");
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

int32_t ssdp_loop_send(void)
{
    if(get_address(local_ipaddr,local_macaddr ) ==  -1)
    {
        perror("get_address\n");
        return -1;
    }

    char* buf=set_device_mluticast_json("TIMING");
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

int32_t ssdp_quit(void)
{
    char* buf=set_device_mluticast_json("LEAVE");
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

//通过ssdp socket从组播地址接受client和其他的device的数据包
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
