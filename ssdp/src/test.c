#include "../inc/mw_ssdp_server.h"

uv_loop_t *loop;

mw_server_handle_t handle;
int flag=0;

int32_t deal_with_stop(void)
{
    handle->ready_stop=1;
    perror("call successful");
}
void on_time(uv_timer_t* out)
{
    printf("handle->ready_stop",handle->ready_stop);
    if(handle->ready_stop == 0)
    {
        mw_server_stop();
        mw_server_handle_destory(handle);
    }
    else
    {
        mw_server_handle_destory(handle);
        perror("free successful");
        uv_timer_stop(out);
    }

}
int main(int argc,char *argv[])
{
    loop=uv_default_loop();
    uv_timer_t out;
    uv_timer_init(loop,&out);

    uv_timer_start(&out,on_time,10000,2000);

    mw_server_handle_create(&handle);

    mw_server_set_cb(handle,deal_with_stop);

    mw_server_start(handle,loop);

    uv_run(loop,UV_RUN_DEFAULT);

    return 0;
}

