/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define _GNU_SOURCE

#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "LIBRARIES.h"
#include "RANDOM_GENERATOR.h"

static uint32_t X_NUMBER, Y_NUMBER, Z_NUMBER, P_NUMBER;

void __RANDOM_INIT(void)
{
    X_NUMBER = time(NULL);
    Y_NUMBER = getpid() ^ getppid();
    Z_NUMBER = clock();
    P_NUMBER = Z_NUMBER ^ Y_NUMBER;
}

uint32_t __RANDOM_NEXT(void)
{
    uint32_t T_NUMBER = X_NUMBER;

    T_NUMBER ^= T_NUMBER << 11;
    T_NUMBER ^= T_NUMBER >> 8;
    X_NUMBER = Y_NUMBER; Y_NUMBER = Z_NUMBER; Z_NUMBER = P_NUMBER;
    P_NUMBER ^= P_NUMBER >> 19;
    P_NUMBER ^= T_NUMBER;

    return P_NUMBER;
}

void __RANDOM_STRING(char *_STRING, int _LENGHT)
{
    while (_LENGHT > 0)
    {
        if (_LENGHT >= 4)
        {
            *((uint32_t *)_STRING) = __RANDOM_NEXT();

            _STRING += sizeof(uint32_t);
            _LENGHT -= sizeof(uint32_t);
        }     
        else if (_LENGHT >= 2)
        {
            *((uint16_t *)_STRING) = __RANDOM_NEXT() & 0xFFFF;

            _STRING += sizeof(uint16_t);
            _LENGHT -= sizeof(uint16_t);
        }
        else
        {
            *_STRING++ = __RANDOM_NEXT() & 0xFF;
            _LENGHT--;
        }
    }
}

uint32_t __RANDOM_NEXT_RANGE(uint32_t _SET_MIN_VALUE, uint32_t _SET_MAX_VALUE) 
{
    return (__RANDOM_NEXT() % (_SET_MAX_VALUE - _SET_MIN_VALUE) + 1) + _SET_MIN_VALUE;
}
