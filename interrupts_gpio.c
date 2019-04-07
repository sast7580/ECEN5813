#include "blocking_method.h"

void initialize_UART0_interrupts(int baud)
{
	// enable port A
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK;
	
	// enable and select UART0 48 Mhz clock
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;								// system options register
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);									// choose 48 Mhz clock
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;									// system clock gating control
	
	// enable UART0 tx (pcr1) and rx (pcr2)
	PORTA_PCR1 = PORT_PCR_MUX(2);
    PORTA_PCR2 = PORT_PCR_MUX(2);
		
	// enable interrupts for control register 2 for UART0
	UART0_C2 = UART0_C2_RE_MASK | UART0_C2_TE_MASK | UART_C2_RIE_MASK | UART_C2_TIE_MASK;
	enable_interrupt(INT_UART0);
	
	// create baud rate
	UART0_C4 = UART0_C4_OSR(15);
	UART0_BDH = (((CORE_CLOCK/16)/baud) >> 8) & UART0_BDH_SBR_MASK;		// baud rate hypo
	UART0_BDL = (((CORE_CLOCK/16)/baud) & UART0_BDH_SBR_MASK);			// baud rate low

	// prepare uart receive and transmit rings
	reset(rxRing, newRingPtr->Length);
	reset(txRing, newRingPtr->Length);
};

void initialize_UART0_polling(int baud)
{
	// enable port A
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK;
	
	// enable and select UART0 48 Mhz clock
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;								// system options register
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);									// choose 48 Mhz clock
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;									// system clock gating control
	
	// enable UART0 tx (pcr1) and rx (pcr2)
	PORTA_PCR1 = PORT_PCR_MUX(2);
    PORTA_PCR2 = PORT_PCR_MUX(2);
		
	// enable receiver and transmitter for UART0
	UART0_C2 = UART0_C2_RE_MASK | UART0_C2_TE_MASK;
	
	// create baud rate
	UART0_C4 = UART0_C4_OSR(15);
	UART0_BDH = (((CORE_CLOCK/16)/baud) >> 8) & UART0_BDH_SBR_MASK;		// baud rate hypo
	UART0_BDL = (((CORE_CLOCK/16)/baud) & UART0_BDH_SBR_MASK);			// baud rate low

	// prepare uart receive and transmit rings
	reset(rxRing, newRingPtr->Length);
	reset(txRing, newRingPtr->Length);
};

void enable_interrupt(int interrupt)
// enables the specific interrupt number
{
	// nvic max is 32, so don't enable if not valid
	if(interrupt > 32)
	{
		printf("\rinterrupt not valid... cannot enable.\n");
		exit = true;
	}
	// set enable and clear registers
	else
	{
		NVIC_ISER |= 1 << (interrupt - 16);
		NVIC_ICPR |= 1 << (interrupt - 16);
	}
};

void disable_interrupt(int interrupt)
// disables the specific interrupt number
{
	// nvic max is 32, so don't disable if not valid
	if(interrupt > 32)
	{
		printf("\rinterrupt not valid... cannot disable.\n");
		exit = true;
	}
	// set clear register
	else
		NVIC_ICER |= 1 << (interrupt - 16);
};

void IRQ0()
{
	// transmit interrupt
	if((UART0_S1 & UART_S1_TDRE_MASK) && (ring->Outi != ring->Ini))
	{
		UART0_D = remove_entry( txRing, UART0_D );
		// if UART0_C2 = 0 then interrupts disabled, = 1 then interrupt enabled
		if(ring->Outi == ring->Ini)
			// Transmit Interrupt Enable for control register 2
			UART0_C2 &= ~UART_C2_TIE_MASK;
	}
	
	// receive interrupt
	if((UART0_S1 & UART_S1_RDRF_MASK) && (((ring->Ini + 1) % ring->Length) != ring->Outi))
	{
		insert( rxRing, UART0_D );
		if(((ring->Ini + 1) % ring->Length) == ring->Outi)
			// Receiver Interrupt Enable for control register 2
			UART0_C2 &= ~UART_C2_RIE_MASK;
	}
};

void enable_leds()
{
	// enable port B (red and green) and port D (blue)
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 = SIM_SCGC5_PORTD_MASK;
	
	// red led
	PORTB_PCR18 = PORT_PCR_MUX(1); 										// activate GPIO in Pin Mux Control
	GPIOB_PSOR |= (1 << 18);											// port set (on)
	GPIOB_PDDR |= (1 << 18);											// port data direction
	
	// green led
	PORTB_PCR19 = PORT_PCR_MUX(1);										// activate GPIO in Pin Mux Control
	GPIOB_PSOR |= (1 << 19);											// port set (on)
	GPIOB_PDDR |= (1 << 19);											// port data direction
	
	// blue led
	PORTD_PCR1 = PORT_PCR_MUX(1);										// activate GPIO in Pin Mux Control
	GPIOD_PSOR = (1 << 1);												// port set (on)
	GPIOD_PDDR |= (1 << 1);												// port data direction
};

// check if transmitter is available to accept new char and then transmit char
void tx_when_available(UART0_MemMapPtr uartReg)
{
	while (UART0_S1_RDRF_MASK) & !(UART0_S1_REG(uartReg)))
	{
		;// wait for character
	}
	
	// save data
	char received_char = UART0_D_REG(uartReg);
	
	// check if transmitter available
	while (UART0_S1_TDRE_MASK & !(UART0_S1_REG(uartReg)))
	{
		;// wait for availablility
	}
	
	// output data
	UART0_D_REG(uartReg) = received_char;
};