/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#pragma once

#include "LIBRARIES.h"

#define CONVERT_ADDR(X_SETTER) X_SETTER & 0xff, (X_SETTER >> 8) & 0xff, (X_SETTER >> 16) & 0xff, (X_SETTER >> 24) & 0xff

void __MEMCPY(void *, void *, int);
void __ZERO(void *, int);

int __STRLEN(char *);
int __STRCOPY(char *, char *);
int __ATOI(char *, int);

int __ISUPPER(char);
int __ISALPHA(char);
int __ISSPACE(char);
int __ISDIGIT(char);

__IPV4 __LOCAL_ADDR(void);
