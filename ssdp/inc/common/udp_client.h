#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "udp_com.h"

typedef int32_t (*mw_cleint_cb_t)(server_info_t *p_info);

struct client_handle
{
   char *mw_mulitcast_Ip;
   int32_t mw_mulitcast_port;
   char *mw_request_device_type;

   mw_cleint_cb_t server_online_cb;
   mw_cleint_cb_t server_offline_cb;
   mw_cleint_cb_t search_response_cb;
};

typedef struct client_handle* mw_client_handle_t;

#endif // UDP_CLIENT_H
