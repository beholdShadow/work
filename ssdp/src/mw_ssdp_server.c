#include "../inc/mw_ssdp_server.h"

int32_t mw_server_handle_create(mw_server_handle_t *handle)
{
    *handle=(mw_server_handle_t)malloc(sizeof (struct server_handle));
    if(*handle == NULL)
    {
        perror("mw_server_handle_t malloc");
        return -1;
    }

    (*handle)->mw_multicast_ip=(char *)malloc(IP_ADDR_LEN);
    if(*handle == NULL)
    {
        perror("mw_multicast_ip malloc");
        return -1;
    }
    strcpy((*handle)->mw_multicast_ip,"224.0.0.88");

    (*handle)->mw_device_name=(char *)malloc(INFO_LEN);
    if(*handle == NULL)
    {
        perror("mw_device_name malloc");
        return -1;
    }
    strcpy((*handle)->mw_device_name,"mw_device_name");

    (*handle)->mw_device_type=(char *)malloc(INFO_LEN);
    if(*handle == NULL)
    {
        perror("mw_device_type malloc");
        return -1;
    }
    strcpy((*handle)->mw_device_type,"mw_device_type");

    (*handle)->mw_multicast_port=8888;

    return 0;
}

void mw_server_handle_destory(mw_server_handle_t handle)
{
    free(handle->mw_multicast_ip);
    handle->mw_multicast_ip=NULL;
    printf("handle->mw_multicast_ip free\n");

    free(handle->mw_device_name);
    handle->mw_device_name=NULL;
    printf("handle->mw_device_name free\n");

    free(handle->mw_device_type);
    handle->mw_device_type=NULL;
    printf("handle->mw_device_type free\n");

    free(handle);
    handle=NULL;
    printf("handle free\n");

}

int32_t mw_server_set_info(mw_server_handle_t handle,
                           char *mw_mulitcast_ip,
                           char *mw_device_name,
                           char *mw_device_type,
                           int32_t mw_mulitcast_port)
{
    if(mw_mulitcast_ip != NULL)
        strcpy(handle->mw_multicast_ip,mw_mulitcast_ip);

    if(mw_device_name != NULL)
        strcpy(handle->mw_device_name,mw_device_name);

    if(mw_device_type != NULL)
        strcpy(handle->mw_device_type,mw_device_type);

    if(mw_mulitcast_port != 0)
        handle->mw_multicast_port=mw_mulitcast_port;
}

int32_t mw_server_set_cb(mw_server_handle_t handle,mw_stop_cb_t server_stop_cb)
{
    handle->server_stop_cb=server_stop_cb;
}

int32_t mw_server_start(mw_server_handle_t handle,uv_loop_t *event_loop)
{
    udp_server_start(handle,event_loop);
}
int32_t mw_server_reload(mw_server_handle_t handle)
{
    udp_server_reload(handle);
}
int32_t mw_server_stop(void)
{
    udp_server_stop();
}
