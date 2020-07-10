//#include "../../inc/common/udp_client.h"

//unsigned char request_device_type[INFO_LEN]={0};

//unsigned char multicast_address[IP_ADDR_LEN]={0};
//uint32_t multicast_port;

//uv_loop_t* event_loop=NULL;

//uv_udp_t ssdp_socket;
//struct  sockaddr_in   ssdp_addrin;

//mw_client_cb_t server_online_cb;
//mw_client_cb_t server_offline_cb;
//mw_client_cb_t search_response_cb;
//mw_stop_cb_t client_stop_cb;

//void udp_client_start(mw_client_handle_t client_handle,uv_loop_t *loop)
//{
//    event_loop=loop;

//    parse_handle(client_handle);

//    init_ssdp_socket();

//    uv_udp_recv_start(&ssdp_socket, uv_alloc_buf, after_uv_udp_recv);

//}

//void udp_client_search(mw_client_handle_t client_handle)
//{
//    parse_handle(client_handle);

//    ssdp_search();
//}


//void udp_client_stop()
//{
//    uv_udp_recv_stop(&ssdp_socket);

//    if(ssdp_socket.data != NULL)
//    {
//        free(ssdp_socket.data);
//        ssdp_socket.data=NULL;
//    }

//    uv_close((uv_handle_t *)&ssdp_socket,close_cb);

//}

//char* set_client_mluticast_json(const char* pack_type)
//{
//    char *str=NULL;

//    cJSON *root=cJSON_CreateObject();

//    cJSON_AddItemToObject(root,"pack_type",cJSON_CreateString(pack_type));
//    cJSON_AddItemToObject(root,"request_device_type",cJSON_CreateString(request_device_type));

//    str=cJSON_Print(root);

//    cJSON_Delete(root);

//    return str;
//}

//void parse_mluticast_json(const char* data)
//{
//    server_info_t info;

//    cJSON *root=cJSON_Parse(data);

//    cJSON *pack_type=cJSON_GetObjectItem(root,"pack_type");

//    cJSON *device_name=cJSON_GetObjectItem(root,"device_name");
//    strcpy(info.device_name,device_name->valuestring);

//    cJSON *device_type=cJSON_GetObjectItem(root,"device_type");
//    strcpy(info.device_type,device_type->valuestring);

//    cJSON *ip_addr=cJSON_GetObjectItem(root,"ip_addr");
//    strcpy(info.ip_addr,ip_addr->valuestring);

//    cJSON *mac_addr=cJSON_GetObjectItem(root,"mac_addr");
//    strcpy(info.mac_addr,mac_addr->valuestring);


//    if(strcmp(pack_type->valuestring,"RESPONSE") == 0)
//    {
//        search_response_cb(&info);
//    }
//    else if(strcmp(pack_type->valuestring,"LEAVE") == 0)
//    {
//        server_offline_cb(&info);
//    }
//    else
//    {
//        server_online_cb(&info);
//    }

//    return;
//}

//void parse_handle(mw_client_handle_t client_handle)
//{
//    strcpy(multicast_address,client_handle->mw_multicast_ip);
//    printf("%s",multicast_address);
//    strcpy(request_device_type,client_handle->mw_request_device_type);

//    multicast_port=client_handle->mw_multicast_port;

//    server_online_cb=client_handle->server_online_cb;
//    server_offline_cb=client_handle->server_offline_cb;
//    search_response_cb=client_handle->search_response_cb;

//    client_stop_cb=client_handle->client_stop_cb;
//}

//int32_t init_ssdp_socket(void)
//{
//    memset(&ssdp_socket, 0, sizeof(uv_udp_t));

//    uv_udp_init(event_loop, &ssdp_socket);

//    uv_ip4_addr(multicast_address, multicast_port, &ssdp_addrin);

//    uv_udp_bind(&ssdp_socket, (const struct sockaddr*)&ssdp_addrin, 0);

//    uv_udp_set_membership(&ssdp_socket,multicast_address,"0.0.0.0", UV_JOIN_GROUP);

//    uv_udp_set_multicast_loop(&ssdp_socket,0);

//    return 0;
//}

//int32_t ssdp_search(void)
//{
//    uv_udp_send_t *req=malloc(sizeof(uv_udp_send_t));

//    req->data=set_client_mluticast_json("SEARCH");
//    if(req->data == NULL)
//    {
//        perror("set_cleint_mluticast_json");
//        return -1;
//    }

//    uv_buf_t w_buf= uv_buf_init((char *)req->data, strlen((char *)req->data));

//    uv_udp_send(req, &ssdp_socket, &w_buf,1, &ssdp_addrin, after_uv_udp_send);
//}

//void uv_alloc_buf(uv_handle_t* handle,
//                size_t suggested_size,
//                uv_buf_t* buf)
//{
//    if (handle->data != NULL)
//    {
//        free(handle->data);
//        handle->data = NULL;
//    }

//    handle->data = malloc(suggested_size + 1);
//    buf->base = handle->data;
//    buf->len = suggested_size;

//}

//void after_uv_udp_send(uv_udp_send_t* req, int status)
//{
//    free(req->data);

//    req->data=NULL;

//    free(req);

//    req=NULL;
//}

//void after_uv_udp_recv(uv_udp_t* handle,
//                    ssize_t nread,
//                    const uv_buf_t* buf,
//                    const struct sockaddr* addr,
//                    unsigned flags)
//{
//    char* data = buf->base;
//    data[nread] ='\0';
//    printf("recv %s\n", data);

//    parse_mluticast_json(data);
//}

//void close_cb(uv_handle_t* handle)
//{
//    uv_is_closing(handle);

//    client_stop_cb();

//}
