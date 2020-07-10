#include "../inc/mw_ssdp_client.h"

int32_t mw_client_handle_create(mw_client_handle_t *handle)
{
    *handle=(mw_client_handle_t)malloc(sizeof (struct client_handle));
    if(*handle == NULL)
    {
        perror("mw_client_handle_t malloc");
        return -1;
    }

    (*handle)->mw_multicast_ip=(char *)malloc(IP_ADDR_LEN);
    if(*handle == NULL)
    {
        perror("mw_multicast_ip malloc");
        return -1;
    }
    strcpy((*handle)->mw_multicast_ip,"224.0.0.88");

    (*handle)->mw_request_device_type=(char *)malloc(INFO_LEN);
    if(*handle == NULL)
    {
        perror("mw_request_device_type malloc");
        return -1;
    }
    strcpy((*handle)->mw_request_device_type,"all");
    printf("%s",(*handle)->mw_request_device_type);

    (*handle)->mw_multicast_port=8888;

    return 0;
}

void mw_client_handle_destory(mw_client_handle_t handle)
{
    free(handle->mw_multicast_ip);
    handle->mw_multicast_ip=NULL;
    printf("free handle->mw_multicast_ip\n");

    free(handle->mw_request_device_type);
    handle->mw_request_device_type=NULL;
    printf("free handle->mw_request_device_type\n");

    free(handle);
    handle=NULL;
    printf("free handle\n");
}

int32_t mw_client_set_info(mw_client_handle_t handle,
                           char *mw_mulitcast_Ip,
                           int32_t mw_mulitcast_port,
                           char *mw_request_device_type
                           )
{}
int32_t mw_client_set_callback(mw_client_handle_t handle,
                           mw_client_cb_t server_online_cb,
                           mw_client_cb_t server_offline_cb,
                           mw_client_cb_t search_response_cb,
                           mw_stop_cb_t client_stop_cb
                           )
{
    handle->server_online_cb=server_online_cb;
    handle->server_offline_cb=server_offline_cb;
    handle->search_response_cb=search_response_cb;
    handle->client_stop_cb=client_stop_cb;

}
int32_t mw_client_search(mw_client_handle_t handle)
{
    udp_client_search(handle);
}

int32_t mw_client_start(mw_client_handle_t handle, uv_loop_t *loop)
{
    udp_client_start(handle, loop);
}

int32_t mw_client_stop(void)
{
    udp_client_stop();
}
