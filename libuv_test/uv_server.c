#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uv.h"

static uv_loop_t* event_loop = NULL;
static uv_timer_t timer;
static uv_udp_t ssdp_socket;
struct sockaddr_in addr;
char base[10]={"helloworld"};
static void
uv_alloc_buf(uv_handle_t* handle,
                size_t suggested_size,
                uv_buf_t* buf) {
    if (handle->data != NULL) {
        free(handle->data);
        handle->data = NULL;
    }

    handle->data = malloc(suggested_size + 1);
    buf->base = handle->data;
    buf->len = suggested_size;
}
static void
after_uv_udp_recv(uv_udp_t* handle,
                    ssize_t nread,
                    const uv_buf_t* buf,
                    const struct sockaddr* addr,
                    unsigned flags);
void loop_send();

static void
after_uv_udp_send(uv_udp_send_t* req, int status)
{
    printf("send successful\n");

    free(req);

    printf("free");

    loop_send();
}

static void
after_uv_udp_recv(uv_udp_t* handle,
                    ssize_t nread,
                    const uv_buf_t* buf,
                    const struct sockaddr* addr,
                    unsigned flags) {

    // 收数据
    char ip_addr[128];
    uv_ip4_name((struct sockaddr_in*)addr, ip_addr, 128);
    int port = ntohs(((struct sockaddr_in*)addr)->sin_port);
    printf("ip: %s:%d nread = %d\n", ip_addr, port, nread);

    char* str_buf = buf->base;
    str_buf[nread] ='\0';
    printf("recv %s\n", str_buf);

    // 写数据
    uv_buf_t w_buf = uv_buf_init("PING", 4);
    uv_udp_send_t req;

    // 告诉事件循环，要帮我管理send事件
    uv_udp_send(&req, handle, &w_buf,1, addr, after_uv_udp_send);
}

void loop_send()
{
    uv_buf_t w_buf= uv_buf_init(base, 10);
    uv_udp_send_t *req=malloc(sizeof (uv_udp_send_t));

    // 告诉事件循环，要帮我管理send事件
    uv_udp_send(req, &ssdp_socket, &w_buf,1, &addr, after_uv_udp_send);


}
int main(int argc, char** argv) {
    event_loop =uv_default_loop();

    memset(&ssdp_socket, 0, sizeof(uv_udp_t));
    memset(&timer, 0, sizeof(uv_timer_t));

    uv_udp_init(event_loop, &ssdp_socket);
    uv_timer_init(event_loop, &timer);

    uv_ip4_addr("224.0.0.88", 8888, &addr);
    uv_udp_bind(&ssdp_socket, (const struct sockaddr*)&addr, 0);

    uv_udp_set_membership(&ssdp_socket, "224.0.0.88","0.0.0.0", UV_JOIN_GROUP);

    uv_udp_set_multicast_loop(&ssdp_socket,0);

    // 告诉事件循环，要帮我管理recv事件
    uv_udp_recv_start(&ssdp_socket, uv_alloc_buf, after_uv_udp_recv);

    loop_send();
    printf("hello world");
    uv_run(event_loop, UV_RUN_DEFAULT);

    uv_loop_close(event_loop);

    return 0;
}
