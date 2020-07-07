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

int main(int argc,char *argv[])
{	
	int socked=socket(AF_INET,SOCK_DGRAM,0);  
	if(socked<0)
	{
		perror("socket failed!");
		return 2;
	}

	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));
	
	bzero(&remote_addr, sizeof(remote_addr));  
        remote_addr.sin_family = AF_INET;  
        remote_addr.sin_port = htons(8888);
        remote_addr.sin_addr.s_addr =inet_addr("224.0.0.88");

	if(bind(socked, (struct sockaddr *)&remote_addr, sizeof(remote_addr))<0) 
         {
                         perror("ssdp_sock bind"); 
                         return -1;
        } 

	struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.88");
        mreq.imr_interface.s_addr =htonl(INADDR_ANY);

        if(setsockopt(socked, IPPROTO_IP,IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
        {
                        perror("fail to setsockopt");
                        return -1;
        }
        
	char buf[1024]={0};
	int length=0;
        memset(buf, 0, sizeof(buf));  
	struct sockaddr_in client_addrin;
        socklen_t  client_len=sizeof(client_addrin);	
        
	while(1)
	{
	    memset(buf,0,sizeof(buf));
	    
	    recvfrom(socked,buf,sizeof(buf),0,(struct sockaddr*)&client_addrin,&client_len);
           
	    printf("buf=%s\n",buf);
           
	}

	return 0;
}
