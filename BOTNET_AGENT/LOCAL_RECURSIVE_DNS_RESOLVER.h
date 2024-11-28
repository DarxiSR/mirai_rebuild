/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#pragma once

#include "LIBRARIES.h"

struct __RESOLVE_DNS_ENTRIES 
{
    uint8_t _ADDRESS_LENGHT;
    __IPV4 *_ADDRESS;
};

void __DNS_RESOLVE(char *, char *);
struct __RESOLVE_DNS_ENTRIES *__LOOKUP(char *);
void __RESOLVE_FREE_ENTRIES(struct __RESOLVE_DNS_ENTRIES *);
