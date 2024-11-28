/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **args) 
{
	if (argc < 2) 
	{
		printf("[!] Use %s <KEY LENGHT> for generate key!\n", args[0]);
		return 1;
	}

	int _KEY_LENGHT = atoi(args[1]);

	srand(time(0));

	for (int _GET_COUNTER = 0; _GET_COUNTER < _KEY_LENGHT; _GET_COUNTER++) 
	{
		uint32_t GET_KEY = rand();

		printf("0x%x, ", GET_KEY);
	}
	
	puts("");
}
