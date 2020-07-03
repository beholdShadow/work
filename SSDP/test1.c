//Udp_group_send.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
                char group_addr[16]="224.0.0.88";
	int socked=socket(AF_INET,SOCK_DGRAM,0);  
	if(socked<0)
	{
		perror("socket failed!");
		return 2;
	}
	
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));
	
	remote_addr.sin_family=AF_INET;
	remote_addr.sin_addr.s_addr=inet_addr(group_addr);
	remote_addr.sin_port=htons(8888);
	
	char buf[1024]="This is a group udp";
	int length=0;
                memset(buf, 0, sizeof(buf));  
                recvfrom(socked,buf,sizeof(buf),0,NULL,NULL);
                printf("Send Message%s\n",buf);
                // struct Device_Msg  Res;
                // memcpy(&Res,buf,sizeof(struct Device_Msg));

                // printf("%s\n%s\n",Res.ipaddr,Res.macaddr);
	close(socked);

                struct  Device_Msg tmp={Discovery,"",""};
                memcpy(buf,&tmp,sizeof(struct Device_Msg));
                length=sendto(socked,buf,strlen(buf),0,(struct sockaddr *)&remote_addr,sizeof(remote_addr));
                printf("Send Message%s\n",buf);
                 memset(buf, 0, sizeof(buf));  

	return 0;
}