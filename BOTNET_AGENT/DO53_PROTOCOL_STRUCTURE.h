/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#pragma once

#include <stdint.h>

#include "LIBRARIES.h"

struct __DNS_PROTOCOL_HEADERS 
{
    uint16_t _ID, _OPCODE, _QDCOUNT, _ANCOUNT, _NSCOUNT, _ARCOUNT;
};

struct __DNS_QUESTION_HEADER 
{
    uint16_t _QTYPE, _QCLASS;
};

struct __DNS_ANSWER_HEADER 
{
    uint16_t _TYPE, _CLASS;
    uint32_t _TTL;
    uint16_t _RDLENGTH;
} __attribute__((packed));
