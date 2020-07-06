//Udp_group_send.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

enum   Msg_Type{Notify, Discovery,Response} ;

struct  Device_Msg
{
                enum Msg_Type  type;
                unsigned char ipaddr[16];
                unsigned char macaddr[16];
};

int main(int argc,char *argv[])
{	
	int socked=socket(AF_INET,SOCK_DGRAM,0);  
	if(socked<0)
	{
		perror("socket failed!");
		return 2;
	}
                int opt = 1;
                 if(setsockopt(socked, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt)) < 0)
                {
                                perror("fail to setsockopt");
                                return -1;
                }
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));
	
               bzero(&remote_addr, sizeof(remote_addr));  
                remote_addr.sin_family = AF_INET;  
                remote_addr.sin_port = htons(8888);
                remote_addr.sin_addr.s_addr = htonl(INADDR_ANY);
                if(bind(socked, (struct sockaddr *)&remote_addr, sizeof(remote_addr))<0) 
                 {
                                 perror("ssdp_sock bind"); 
                                 return -1;
                } 
                struct ip_mreq mreq;
                mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.0");
                mreq.imr_interface.s_addr =htonl(INADDR_ANY);

                if(setsockopt(socked, IPPROTO_IP,IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
                {
                                perror("fail to setsockopt");
                                return -1;
                }


	char buf[1024]="This is a group udp";
	int length=0;
                memset(buf, 0, sizeof(buf));  
                // recvfrom(socked,buf,sizeof(buf),0,NULL,NULL);
                //  printf("Send Message%s\n",buf);
                // struct Device_Msg  Res;
                // memcpy(&Res,buf,sizeof(struct Device_Msg));

                // printf("%s\n%s\n",Res.ipaddr,Res.macaddr);
	// close(socked);

                // struct  Device_Msg tmp={Discovery,"",""};
                // memcpy(buf,&tmp,sizeof(struct Device_Msg));
                // strcpy(buf,"hello world");
                // length=sendto(socked,buf,strlen(buf),0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
                // memset(buf, 0, sizeof(buf));  
                if(recvfrom(socked,buf,sizeof(buf),0,NULL,NULL) >=0)
                                printf("receive data from server\n");
                printf("buf=%s\n",buf);

	return 0;
}