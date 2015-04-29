/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2015年04月29日 星期三 17时43分45秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "debug.h"
#include "packet.h"

#define SERVER_PORT htons(8989)

enum em_server_status { ST_RECV, ST_LOGIN, ST_MSG, ST_LIST, ST_HEART, ST_ERR };

int main(void)
{
    int sd;
    int ret;
    socklen_t hislen;
    struct sockaddr_in myaddr, hisaddr;
    enum em_server_status status;
    ssize_t len;
    struct st_packet pkt;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == sd){
        perror("main():socket()");
        goto socket_err;
    }
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = SERVER_PORT;
    myaddr.sin_addr.s_addr = INADDR_ANY; 
    ret = bind(sd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if(-1 == ret){
        printf("main():bind()");
        goto bind_err;
    }

    hislen = sizeof(hislen);
    status = ST_RECV;
    while(1){
       len = recvfrom(sd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&hisaddr, (socklen_t *)&hislen);
        if(-1 == len){
            perror("main():recvfrom()");
            goto recv_err;
        }
    }

    return 0;

    recv_err:
    bind_err:
    close(sd);
    socket_err:
    return 1;
}
