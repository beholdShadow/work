#include "../inc/mw_ssdp_server.h"

uv_loop_t *loop;

mw_server_handle_t handle;
int flag=0;

int32_t deal_with_stop(void)
{
    mw_server_handle_destory(handle);
}

void on_time(uv_timer_t* out)
{
    if((flag &1) == 0)
    {
        mw_server_handle_create(&handle);

        mw_server_set_cb(handle,deal_with_stop);

        mw_server_start(handle,loop);
    }
    else
    {
        mw_server_stop();
    }

    flag++;

}
int main(int argc,char *argv[])
{
    loop=uv_default_loop();

    uv_timer_t out;

    uv_timer_init(loop,&out);

    uv_timer_start(&out,on_time,0,10000);

    uv_run(loop,UV_RUN_DEFAULT);

    return 0;
}

