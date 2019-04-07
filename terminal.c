#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "help.h"
#include "blocking.h"
#include "nonblocking.h"
#include "frdm.h"


#define FRDM	1 

#ifdef FRDM
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#define printf PRINTF
#endif

// Terminal takes user input to choose between blocking (using polling for data input) and nonblocking (using interrupts for data input).

int main(void)
{
  #ifdef FRDM
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  #endif

	bool exit = false;
	while(exit == false)
	{
		printf("\n\r------------------------------------------------------------------");
		printf("\n\rEnter a command. Type 'Help' to get a list of commands. Type 'Exit' to quit.\n\r");

		char command[10];

		#ifdef FRDM
			readin(command, sizeof(command));
		#else
			scanf("%25s", command);
		#endif
	
		if(!strcmp(command, "new") | !strcmp(command, "New"))
		{
			printf("\n\rEnter the number of words you would like to allocate:\n\r");
			char ring_size[100];

			#ifdef FRDM
			readin(ring_size, sizeof(ring_size));
			#else
			scanf("%s", ring_size);
			#endif
			
			int ring_length = atol(ring_size);

			init(ring_length);
		}
		
		else if(!strcmp(command, "help") | !strcmp(command, "Help"))
		{
			help();
		}
		
		else if(!strcmp(command, "exit") | !strcmp(command, "Exit"))
		{
			printf("\n\rBye!");
			exit = true;
			return 0;
		}
		
		else if(!strcmp(command, "blocking") | !strcmp(command, "Blocking"))
		{
			blocking_method(BLOCKING);
		}
		
		else if(!strcmp(command, "nonblocking") | !strcmp(command, "Nonblocking"))
		{
			blocking_method(NONBLOCKING);
		}
		
		else
		{
			printf("\n\rFunction not yet implemented, try again later.\n");
		}
	}
}
