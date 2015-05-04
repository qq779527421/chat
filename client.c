/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2015年05月04日 星期一 13时49分33秒
************************************************************************/
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <crypt.h>

#include "protocal.h"
#include "debug.h"
#include "comm.h"

#define PWDSIZE 64

void InitWindow(void)
{

}


int main(void)
{
    uint32_t id;
    char passwd[PWDSIZE] = {0} ;
    int sd, ret;
    char *enctypt;
    struct packet pkt;
    socklen_t hislen;
    struct sockaddr_in hisaddr, hisaddr_tmp;

    printf("id:");
    fflush(stdout);
    scanf("%d", &id);

    printf("psw:");
    fflush(stdout);
    ret = scanf("%s", passwd);
    passwd[strlen(passwd) - 1] = '\0';
    
    pkt.major = MAJOR_LOGIN;
    pkt.minor = MINOR_IN;
    pkt.id = id;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    /*if err*/
    hisaddr.sin_family = AF_INET;
    hisaddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &hisaddr.sin_addr.s_addr);
    ret = sendto(sd, &pkt, 1+1+4, 0, (struct sockaddr *)&hisaddr, sizeof(hisaddr));
    /*if err*/
    hislen = sizeof(hisaddr);
    ret = recvfrom(sd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&hisaddr_tmp, &hislen);
    debug("%s\n", pkt.salt);
    /*if err*/
    pkt.major = MAJOR_LOGIN;
    pkt.minor = MINOR_ENCRYP;
    enctypt = crypt(passwd, pkt.salt);
    ret = snprintf(pkt.encryption, PAD_SIZE, "%s", enctypt);
    debug("%s\n", pkt.encryption);

    ret = sendto(sd, &pkt, ret + 2, 0, (struct sockaddr *)&hisaddr_tmp, sizeof(hisaddr));
    /*if err*/
    ret = recvfrom(sd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&hisaddr_tmp, &hislen);
    
    if(0 == pkt.ack){
        printf("ACK:%d, faild\n", pkt.ack);
    }

    InitWindow();

    return 0;
}

