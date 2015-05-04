/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2015年04月29日 星期三 17时43分45秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "debug.h"
#include "protocal.h"
#include "comm.h"


#define SALTSIZE  128
#define ENCSIZE   256

enum server_status{
    ST_RECV, 
    ST_LOGIN, 
    ST_MSG, 
    ST_LIST, 
    ST_MANAGE,
    ST_HEART, 
    ST_ERR, 
};


int ProcessPkt(struct packet *pkt)
{
    switch(pkt ->major){
    case MAJOR_LOGIN:
        return ST_LOGIN;
    case MAJOR_MSG:
        return ST_MSG;
    case MAJOR_LIST:
        return ST_LIST;
    case MAJOR_MANAGE:
        return ST_MANAGE;
    case MAJOR_HEART:
        return ST_HEART;
    case MAJOR_ERR:
        return ST_ERR;
    }
    return 0;
}

struct loginfo{
    uint32_t id;
    struct sockaddr_in addr;
};
struct shadow{
    uint32_t id;
    char salt[SALTSIZE];
    char encryption[ENCSIZE];
};
static int FindShadow(struct shadow *sdw)
{
    FILE *fp;
    char *p, *p1, *p2, *p3;
    int i;
    char buf[SALTSIZE + ENCSIZE + sizeof(uint32_t)];

    fp = fopen("./shadow", "r");
    if(fp == NULL){
        perror("FindShadow():fopen(shadow)");
        exit(-1);
    }
    while(1){
        memset(buf, '\0', sizeof(buf));
        p = fgets(buf, sizeof(buf), fp);
        if(NULL == p){
            return 0;
        }
        if(sdw->id != atoi(buf)){
            continue;
        }
        
        p1 = strchr(buf, '$');
        p2 = strchr(p1 + 1, '$');
        p3 = strchr(p2 + 1, '$');
        for(i = 0, p = p1; p <= p3;){
            sdw->salt[i++] = *p++;
        }
        sdw->salt[i] = '\0';
        debug("%s\n",sdw->salt);

        for(i = 0, p = p1;;){
            sdw->encryption[i++] = *p++;
            if(*p == '\n'){
                sdw->encryption[i] = '\0';
                break;
            }
        }
        debug("%s\n",sdw->encryption);

        return 1;
    }
}
static void *login_thread(void *arg)
{
    int ret;
    int sd;
    struct loginfo *info = arg;
    struct packet pkt;
    struct shadow sdw;

    pthread_detach(pthread_self());

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    
    sdw.id = info->id;
    ret = FindShadow(&sdw);
    if(0 == ret){
        debug("%d did not find\n", info->id);
    }else{
        pkt.major = MAJOR_LOGIN;
        pkt.minor = MINOR_SALT;
        ret = snprintf(pkt.salt, SALTSIZE, "%s", sdw.salt);
        sendto(sd, &pkt, ret + 2 + 1, 0, (struct sockaddr *)&info->addr, sizeof(info->addr));
        memset(&pkt, '\0', sizeof(pkt));
        /*time out*/
        recvfrom(sd, &pkt, sizeof(pkt), 0, NULL, NULL);
        if(0 == strcmp(sdw.encryption, pkt.encryption)){
            pkt.major = MAJOR_LOGIN;
            pkt.minor = MINOR_ACK;
            pkt.ack = 1;
            sendto(sd, &pkt, ret + 2, 0, (struct sockaddr *)&info->addr, sizeof(info->addr));
        }else{
            pkt.major = MAJOR_LOGIN;
            pkt.minor = MINOR_ACK;
            pkt.ack = 0;
            sendto(sd, &pkt, ret + 2, 0, (struct sockaddr *)&info->addr, sizeof(info->addr));
        }
    }
    
    free(info);
    return NULL;
}

static void login(struct packet *pkt, struct sockaddr_in *addr)
{
    struct loginfo *info;
    pthread_t tid;
    int ret;

    info = malloc(sizeof(*info));
    if(NULL == info){
        return;
    }
    info->id = pkt->id;
    info->addr = *addr;
    
    ret = pthread_create(&tid, NULL, login_thread, info);
    if(0 != ret){
        fprintf(stderr, "pthread_create(): %d", ret);
        free(info);
    }
}

int main(void)
{
    int sd;
    int ret;
    int val;
    socklen_t hislen;
    struct sockaddr_in myaddr, hisaddr;
    enum server_status status;
    ssize_t len;
    struct packet pkt;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == sd){
        perror("main():socket()");
        goto socket_err;
    }

    val = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(SERVER_PORT);
    myaddr.sin_addr.s_addr = INADDR_ANY; 
    ret = bind(sd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if(-1 == ret){
        printf("main():bind()");
        goto bind_err;
    }

    hislen = sizeof(hislen);
    status = ST_RECV;
    while(1){
        switch(status){
        case ST_RECV:
            len = recvfrom(sd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&hisaddr, (socklen_t *)&hislen);
            if(-1 == len){
                perror("main():recvfrom()");
                goto recv_err;
            }
            status = ProcessPkt(&pkt);
            break;
        case ST_LOGIN:
            login(&pkt, &hisaddr);
            status = ST_RECV;
            break;
        case ST_MSG:
            break;
        case ST_MANAGE:
            break;
        case ST_LIST:
            break;
        case ST_HEART:
            break;
        case ST_ERR:
            break;
        }
    }

    return 0;

    recv_err:
    bind_err:
    close(sd);
    socket_err:
    return 1;
}
