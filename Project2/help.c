#include "help.h"

void help()
{
	printf("\n\r(Help)");
	printf("\n\r---Commands available---");
	printf("\n\r'New'\t\t Create a new circular ring buffer.\n");
	printf("\n\r'Blocking'\t\t Output user data by polling the UART hardware.\n");
	printf("\n\r'Nonblocking'\t\t Output user data by using interrupts.\n");
	return;
}
