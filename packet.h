/*************************************************************************
> File Name: packet.h
> Author: 
> Mail: 
> Created Time: 2015年04月29日 星期三 23时14分03秒
************************************************************************/

#ifndef QG_PACKET_H
#define QG_PACKET_H

#include <arpa/inet.h>

#define PAKET_SIZE      1480
#define PAD_SIZE        (PAKET_SIZE - 2)
#define MSG_SIZE        (PAD_SIZE - sizeof(uint32_t) * 2)
#define LIST_SIZE       (PAD_SIZE / sizeof(uint32_t))

struct __attribute__ ((__packed__)) st_packet{
    uint8_t major;
    uint8_t minor;
    union{
        uint32_t id;
        uint8_t salt[0];
        uint8_t encryption[0];
        uint8_t ack;
        struct __attribute__ ((__packed__)) st_msg{
            uint32_t dest;
            uint32_t src;
            uint8_t message[0];
        }msg;
        uint32_t list[0];
    }
    uint8_t pad[PAD_SIZE];
};

#endif 
