#include "../inc/mw_ssdp_client.h"

uv_loop_t *loop;

mw_client_handle_t handle;

int32_t deal_with_stop(void)
{
    mw_client_handle_destory(handle);
}

void server_on(server_info_t *info)
{
    printf("    NOTIFY message\nserver ipaddr:%s\nserver macaddr:%s\nserver device name:%s\nserver device type:%s\n",
           info->ip_addr,info->mac_addr,info->device_name,info->device_type);
}
void server_off(server_info_t *info)
{
    printf("    LEAVE message\nserver ipaddr:%s\nserver macaddr:%s\nserver device name:%s\nserver device type:%s\n",
           info->ip_addr,info->mac_addr,info->device_name,info->device_type);
}
void server_search(server_info_t *info)
{
    printf("    RESPONSE message\nserver ipaddr:%s\nserver macaddr:%s\nserver device name:%s\nserver device type:%s\n",
           info->ip_addr,info->mac_addr,info->device_name,info->device_type);
}
void on_time(uv_timer_t* out)
{
    mw_client_search(handle);
}

int main(int argc,char *argv[])
{
    loop=uv_default_loop();
    uv_timer_t out;
    uv_timer_init(loop,&out);

    uv_timer_start(&out,on_time,30000,10000);

    mw_client_handle_create(&handle);
    mw_client_start(&handle,loop);


    uv_run(loop,UV_RUN_DEFAULT);

    return 0;
}

