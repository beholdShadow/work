#include "../inc/mw_ssdp_client.h"

int32_t mv_client_handle_create(mw_client_handle_t *handle)
{}

void mv_client_handle_destory(mw_client_handle_t handle)
{}

int32_t mw_client_set_info(mw_client_handle_t handle,
                           char *mw_mulitcast_Ip,
                           int32_t mw_mulitcast_port
                           )
{}
int32_t mw_client_set_callback(mw_client_handle_t handle,
                           p_callback server_online_cb,
                           p_callback server_offline_cb,
                           p_callback client_search_cb_
                           )
{}
int32_t mw_client_search(mw_client_handle_t handle,char *mw_request_device_type)
{}

int32_t mw_client_start(mw_client_handle_t handle)
{}

int32_t mw_client_stop(void)
{}
