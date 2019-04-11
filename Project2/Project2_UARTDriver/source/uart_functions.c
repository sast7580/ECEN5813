/*
 * uart_functions.c
 *
 *  Created on: Apr 7, 2019
 *      Author: sam
 */
#include "uart_functions.h"

extern ring_t * report_ring;

void add_stat(char occurence)
{
	int s = (int) occurence;
	stats[s]++;
}

void report_stats()
{

	insert(report_ring, '\n');
	insert(report_ring, '\r');
	insert(report_ring, '-');
	insert(report_ring, '-');

	insert(report_ring, '\n');
	insert(report_ring, '\r');
	insert(report_ring, '-');
	insert(report_ring, '-');
	insert(report_ring, '-');
	insert(report_ring, 'R');
	insert(report_ring, 'e');
	insert(report_ring, 'p');
	insert(report_ring, 'o');
	insert(report_ring, 'r');
	insert(report_ring, 't');
	insert(report_ring, '-');
	insert(report_ring, '-');
	insert(report_ring, '-');

	for(int i = 0; i<(sizeof(stats)/sizeof(int)); i++)
	{
		if(stats[i] > 0){
			insert(report_ring, '\n');
			insert(report_ring, '\r');
			insert(report_ring, (char) i);
			insert(report_ring, ' ');
			insert(report_ring, '-');
			insert(report_ring, ' ');
			if(stats[i] > 9)
			{
				insert(report_ring, (char)((stats[i]/10)+48));
				insert(report_ring, (char)((stats[i] % 10)+48));

			}
			else
				insert(report_ring, (char)(stats[i] + 48));
		}
	}

	insert(report_ring, '\n');
	insert(report_ring, '\r');
	insert(report_ring, '-');
	insert(report_ring, '-');
	insert(report_ring, '-');
	insert(report_ring, '\n');
	insert(report_ring, '\r');
	tx_ring(report_ring);
}

void UART0_init(uint32_t baudrate)
{
	 //Enable Port A Clock Gate control - SCGC5
	 SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	 //Enable UART0 Clock Gate control - SCGC4
	 SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	 //Set UART0 clock source to MCGFLLCLK - SOPT2
	 SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	 //Set PLLFLLSEL Multiplexor for MCGFLLCLK - SOPT2
	 SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;

	 //Set Port A Pin Configurations to alternative 2
	 PORTA->PCR[1] |= PORT_PCR_MUX(2);
	 PORTA->PCR[2] |= PORT_PCR_MUX(2);

    //Disable TX and RX before changing settings
    UART0->C2 &= ~UART_C2_TE_MASK & ~UART_C2_RE_MASK;

    //Set the Baudrate
    uint32_t sbr = 0;
    sbr = 21e6/ (baudrate * 16);	//OSR default 15, OSR + 1 = 16
    sbr++;
    UART0->BDH = (UART0->BDH & ~UART_BDH_SBR_MASK) | ((uint8_t ) sbr >> 8);
    UART0->BDL = (UART0->BDL & ~UART_BDL_SBR_MASK) | (uint8_t) sbr;

    //Enable TX and RX
    UART0->C2 = UART0->C2 | UART_C2_TE_MASK | UART_C2_RE_MASK;

#ifdef BLOCKING

	UART0->C2 = UART0->C2 & ~UART_C2_TIE_MASK            //Transmit Interrupt Enable          (Transmit Data Register Empty)
                              & ~UART_C2_TCIE_MASK          //Transmit Complete Interrupt Enable (Transmission Complete)
                              & ~UART_C2_RIE_MASK;           //Receiver Interrupt Enable         (Receive Data Register Full)

#else
    	//UART0->C2 = UART0->C2 | UART_C2_TIE_MASK            //Transmit Interrupt Enable          (Transmit Data Register Empty)
                                  //| UART_C2_TCIE_MASK          //Transmit Complete Interrupt Enable (Transmission Complete)
                                  //| UART_C2_RIE_MASK;         check_TX_ready() && check_TX_ready() &&   //Receiver Interrupt Enable         (Receive Data Register Full)
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->C2 |= UART_C2_RIE_MASK;
#endif
}

#ifdef ELEMENTS
void UART_writeData_blocking(char TXed_char)
{
    while(!(UART0->S1 & UART_S1_TDRE_MASK))
    {
    	//Wait until transmit data buffer is empty
    }
    UART0->D = TXed_char;
    while(!(UART0->S1 & UART_S1_TC_MASK))
    {
    	//Wait until transmitter activity is complete
    }
}
#endif

#ifdef BLOCKING

void UART_writeData_blocking(char TXed_char)
{
    while(!(UART0->S1 & UART_S1_TDRE_MASK))
    {
    	//Wait until transmit data buffer is empty
    }
    UART0->D = TXed_char;
    while(!(UART0->S1 & UART_S1_TC_MASK))
    {
    	//Wait until transmitter activity is complete
    }
}

char UART_readData_blocking()
{
    char RX_char;
    while(!(UART0->S1 & UART_S1_RDRF_MASK))
    {
        //Wait until receive buffer is full
    }
    RX_char = UART0->D;
    return RX_char;
}
#endif

#ifdef NONBLOCKING

void UART0_IRQHandler()
{
	char TXed_char;
	char RXed_char;

	NVIC_DisableIRQ(UART0_IRQn);

	//UART0->C2 &= ~UART_C2_RIE_MASK;

	//Check receive buffer has data, RX_ring is not full
	if((UART0->S1 & UART_S1_RDRF_MASK))
	{
		RXed_char = UART0->D;
		int store_result = insert(RX_ring, RXed_char);
		//If receive buffer is full, disable interrupts
//		if(store_result == -1)
//			UART0->C2 &= ~UART_C2_RIE_MASK;	// Disable RX Interrupt for control register 2

		add_stat(RXed_char);
		remove_entry(RX_ring, &RXed_char);
		TX_char(TX_ring, RXed_char);

		//UART0->C2 |= UART_C2_RIE_MASK;	// Enable RX Interrupt for control register 2
	}
	report_stats();
	NVIC_EnableIRQ(UART0_IRQn);
}

bool check_TX_ready()
{
	//check TX register is empty and previous transmission is complete
	if((UART0->S1 & UART_S1_TDRE_MASK) && (UART0->S1 & UART_S1_TC_MASK))
		return true;
	else
		return false;
}

int tx_ring(ring_t * ring)
{
	while(entries(ring) > 0)
	{
		if(check_TX_ready())
		{
			char removed;
			remove_entry(ring, &removed);
			UART0->D = removed;
		}
	}
	return 0;
}

int TX_char(ring_t * ring, char TX)
{
	insert(ring, TX);
	char removed;

	//check TX is ready and ring is not empty
	if(check_TX_ready())
	{
		remove_entry(ring, &removed);
		UART0->D = removed;
		return 0;
	}
	else
		return -1;
}

void UART_writeData_blocking(char TXed_char)
{
    while(!(UART0->S1 & UART_S1_TDRE_MASK))
    {
    	//Wait until transmit data buffer is empty
    }
    UART0->D = TXed_char;
    while(!(UART0->S1 & UART_S1_TC_MASK))
    {
    	//Wait until transmitter activity is complete
    }
}
#endif

void enable_leds()
{
	// enable port B (red and green) and port D (blue)
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	PORTB->PCR[18] = PORT_PCR_MUX(1); 							// activate red led GPIO in Pin Mux Control
	PORTB->PCR[19] = PORT_PCR_MUX(1);							// activate green led GPIO in Pin Mux Control
	PORTD->PCR[1] = PORT_PCR_MUX(1);							// activate blue led GPIO in Pin Mux Control

	// red led
	GPIOB->PSOR |= (1 << 18);									// port set (on)
	GPIOB->PDDR |= (1 << 18);									// port data direction

	// green led
	GPIOB->PSOR |= (1 << 19);									// port set (on)
	GPIOB->PDDR |= (1 << 19);									// port data direction

	// blue led
	GPIOD->PSOR = (1 << 1);										// port set (on)
	GPIOD->PDDR |= (1 << 1);									// port data direction
}

void wait(int time)
{
	// count down timer
	for(int i = time*1000; i != 0; i--)
    {
	    ;
    }
}

