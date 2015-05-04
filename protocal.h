/*************************************************************************
> File Name: protocal.h
> Author: 
> Mail: 
> Created Time: 2015年04月29日 星期三 23时14分03秒
************************************************************************/

#ifndef QG_PROTOCAL_H
#define QG_PROTOCAL_H

#include <arpa/inet.h>

#define PAKET_SIZE      1472
#define PAD_SIZE        (PAKET_SIZE - 2)
#define MSG_SIZE        (PAD_SIZE - sizeof(uint32_t) * 2)
#define LIST_SIZE       (PAD_SIZE / sizeof(uint32_t))
#define ENC_SIZE        PAD_SIZE
#define SAIL_SIZE       PAD_SIZE

#define HEART_INTERVAL  10
#define OFLINE_INTERVAL (HEART_INTERVAL * 5)

enum e_major{
    MAJOR_LOGIN = 1,
    MAJOR_MSG,
    MAJOR_LIST,
    MAJOR_MANAGE,
    MAJOR_HEART,
    MAJOR_ERR,
};
enum e_minor{
    MINOR_IN = 1,
    MINOR_SALT,
    MINOR_ENCRYP,
    MINOR_ACK,
    MINOR_OUT,

    MINOR_MEG,
    
    MINOR_LIST_ASK,
    MINOR_LIST_ANSWER,

    MINOR_HEART,
};

struct __attribute__ ((__packed__)) message{
    uint32_t dest;
    uint32_t src;
    char msg[0];
};

struct __attribute__ ((__packed__)) packet{
    uint8_t major;
    uint8_t minor;
    union{
        uint32_t id;
        char salt[0];
        char encryption[0];
        uint8_t ack;
        struct message msg;
        uint32_t list[0];
        char pad[PAD_SIZE];
    };
};

#endif 
