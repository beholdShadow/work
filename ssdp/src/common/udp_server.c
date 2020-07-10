#include "../../inc/common/udp_server.h"

char local_ipaddr[IP_ADDR_LEN]={0};
char local_macaddr[MAC_ADDR_LEN]={0};
char device_name[INFO_LEN]={0};
char device_type[INFO_LEN]={0};

unsigned char multicast_address[IP_ADDR_LEN]={0};
uint32_t multicast_port;

uv_loop_t* event_loop=NULL;
uv_timer_t timer;
uv_udp_t ssdp_socket;
struct  sockaddr_in   ssdp_addrin;
mw_server_cb_t server_stop_cb;

void udp_server_start(mw_server_handle_t server_handle,uv_loop_t *loop)
{
    event_loop=loop;

    server_handle->ready_stop=0;

    parse_handle(server_handle);

    init_ssdp_socket();

    ssdp_notify();

    init_timer();

    uv_udp_recv_start(&ssdp_socket, uv_alloc_buf, after_uv_udp_recv);

}

void udp_server_reload(mw_server_handle_t server_handle)
{
    uv_udp_recv_stop(&ssdp_socket);
    uv_udp_set_membership(&ssdp_socket,multicast_address,"0.0.0.0", UV_LEAVE_GROUP);

    parse_handle(server_handle);

    uv_ip4_addr(multicast_address, multicast_port, &ssdp_addrin);
    uv_udp_bind(&ssdp_socket, (const struct sockaddr*)&ssdp_addrin, 0);

    uv_udp_set_membership(&ssdp_socket,multicast_address,"0.0.0.0", UV_JOIN_GROUP);
    uv_udp_recv_start(&ssdp_socket, uv_alloc_buf, after_uv_udp_recv);

}

void udp_server_stop()
{
    ssdp_quit();
}

char* set_device_mluticast_json(const char* pack_type)
{
    char *str=NULL;

    cJSON *root=cJSON_CreateObject();

    cJSON_AddItemToObject(root,"pack_type",cJSON_CreateString(pack_type));
    cJSON_AddItemToObject(root,"device_name",cJSON_CreateString(device_name));
    cJSON_AddItemToObject(root,"device_type",cJSON_CreateString(device_type));
    cJSON_AddItemToObject(root,"ip_addr",cJSON_CreateString(local_ipaddr));
    cJSON_AddItemToObject(root,"mac_addr",cJSON_CreateString(local_macaddr));

    str=cJSON_Print(root);

    cJSON_Delete(root);

    return str;
}

char* parse_mluticast_json(const char* data)
{
    cJSON *root;
}

void parse_handle(mw_server_handle_t server_handle)
{
    strcpy(multicast_address,server_handle->mw_multicast_ip);

    multicast_port=server_handle->mw_multicast_port;

    strcpy(device_name,server_handle->mw_device_name);

    strcpy(device_type,server_handle->mw_device_type);

    server_stop_cb=server_handle->server_stop_cb;

}
int32_t get_address(char *ip_addr, char *mac_addr)
{
        int32_t sock;
        unsigned char buf[MAXSIZE]={0};
        struct ifreq* ifr;
        struct ifconf ifc;

        ifc.ifc_len=MAXSIZE;
        ifc.ifc_buf=(char *)buf;

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

int32_t init_timer(void)
{
    memset(&timer, 0, sizeof(uv_timer_t));

    uv_timer_init(event_loop, &timer);

    uv_timer_start(&timer,ssdp_loop_send,1000,5000);

}

int32_t init_ssdp_socket(void)
{
    memset(&ssdp_socket, 0, sizeof(uv_udp_t));

    uv_udp_init(event_loop, &ssdp_socket);

    uv_ip4_addr(multicast_address, multicast_port, &ssdp_addrin);

    uv_udp_bind(&ssdp_socket, (const struct sockaddr*)&ssdp_addrin, 0);

    uv_udp_set_membership(&ssdp_socket,multicast_address,"0.0.0.0", UV_JOIN_GROUP);

    uv_udp_set_multicast_loop(&ssdp_socket,0);

    return 0;
}

int32_t ssdp_quit(void)
{
    if(get_address(local_ipaddr,local_macaddr ) ==  -1)
    {
        perror("get_address\n");
        return -1;
    }

    uv_udp_send_t *req=malloc(sizeof(uv_udp_send_t));

    req->data=set_device_mluticast_json("LEAVE");
    if(req->data == NULL)
    {
        perror("set_Device_Mluticast_Json");
        return -1;
    }

    uv_buf_t w_buf= uv_buf_init((char *)req->data, strlen((char *)req->data));

    uv_udp_send(req, &ssdp_socket, &w_buf,1, &ssdp_addrin, end_uv_udp_send);

    return 0;
}

int32_t ssdp_notify(void)
{
    if(get_address(local_ipaddr,local_macaddr ) ==  -1)
    {
        perror("get_address\n");
        return -1;
    }

    uv_udp_send_t *req=malloc(sizeof(uv_udp_send_t));

    req->data=set_device_mluticast_json("NOTIFY");
    if(req->data == NULL)
    {
        perror("set_Device_Mluticast_Json");
        return -1;
    }

    uv_buf_t w_buf= uv_buf_init((char *)req->data, strlen((char *)req->data));

    uv_udp_send(req, &ssdp_socket, &w_buf,1, &ssdp_addrin, after_uv_udp_send);
}

void uv_alloc_buf(uv_handle_t* handle,
                size_t suggested_size,
                uv_buf_t* buf)
{
    if (handle->data != NULL)
    {
        free(handle->data);
        handle->data = NULL;
    }

    handle->data = malloc(suggested_size + 1);
    buf->base = handle->data;
    buf->len = suggested_size;

}

void ssdp_loop_send(uv_timer_t* handle)
{
    if(get_address(local_ipaddr,local_macaddr ) ==  -1)
    {
        perror("get_address\n");
        return ;
    }

    uv_udp_send_t *req=malloc(sizeof(uv_udp_send_t));

    req->data=set_device_mluticast_json("TIMING");
    if(req->data == NULL)
    {
        perror("set_Device_Mluticast_Json");
        return ;
    }

    uv_buf_t w_buf= uv_buf_init((char *)req->data, strlen((char *)req->data));

    uv_udp_send(req, &ssdp_socket, &w_buf,1, &ssdp_addrin, after_uv_udp_send);

    return;

}

void after_uv_udp_send(uv_udp_send_t* req, int status)
{
    free(req->data);

    req->data=NULL;

    free(req);

    req=NULL;
}

void after_uv_udp_recv(uv_udp_t* handle,
                    ssize_t nread,
                    const uv_buf_t* buf,
                    const struct sockaddr* addr,
                    unsigned flags)
{
    char* data = buf->base;
    data[nread] ='\0';
    printf("recv %s\n", data);

    uv_udp_send_t *req=malloc(sizeof(uv_udp_send_t));

    req->data=parse_mluticast_json(data);
    if(req->data != NULL)
    {
        uv_buf_t w_buf= uv_buf_init((char *)req->data, strlen((char *)req->data));

        uv_udp_send(req, &ssdp_socket, &w_buf,1, &ssdp_addrin, after_uv_udp_send);
    }
}

void end_uv_udp_send(uv_udp_send_t* req, int status)
{
    free(req->data);

    req->data=NULL;

    free(req);

    req=NULL;

    uv_udp_recv_stop(&ssdp_socket);

    uv_timer_stop(&timer);

    if(ssdp_socket.data != NULL)
    {
        free(ssdp_socket.data);
        ssdp_socket.data=NULL;
    }

    uv_close((uv_handle_t *)&ssdp_socket,close_cb);

    server_stop_cb();

}

void close_cb(uv_handle_t* handle)
{
    uv_is_closing(handle);

}
