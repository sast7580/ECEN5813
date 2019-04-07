#include "blocking_method.h"

void blocking_method(BLOCKING_TYPE)
{       
	#if BLOCKING_TYPE == BLOCKING
	// use polling
	initialize_UART0_polling(BAUD_RATE);
		
	while(1)
	{
		// wait for data then transmit via UART when available.
		tx_when_available(UART0_MemMapPtr uartReg);
	}
	
	#else
	// use interrupts
	initialize_UART0_interrupts(BAUD_RATE);
	enable_leds();
	IRQ0();
	
	while(1)
	{
		// toggle the red led when not in use
		GPIOB_PTOR = (1 << 18);
	}
	
	#endif
	
}
