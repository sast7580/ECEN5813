/*
 * uart_functions.c
 *
 *  Created on: Apr 7, 2019
 *      Author: sam
 */
#include "uart_functions.h"

void add_stat(char occurence)
{
	int s = (int) occurence;
	stats[s]++;
}

void report_stats()
{
	for(int i = 0; i<(sizeof(stats)/sizeof(int)); i++)
	{
		if(stats[i] > 0){
			UART_writeData_blocking('\n');
			UART_writeData_blocking('\r');
			UART_writeData_blocking((char) i);
			UART_writeData_blocking(' ');
			UART_writeData_blocking('-');
			UART_writeData_blocking(' ');
			UART_writeData_blocking((char)(stats[i] + 48));
			UART_writeData_blocking('\n');
			UART_writeData_blocking('\r');
		}
	}
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

	UART0->C2 = UART0->C2 & ~UART_C2_TIE_MASK            //Transmt Interrupt Enable          (Transmit Data Register Empty)
                              & ~UART_C2_TCIE_MASK          //Transmit Complete Interrupt Enable (Transmission Complete)
                              & ~UART_C2_RIE_MASK;           //Receiver Interrupt Enable         (Receive Data Register Full)

#else
    	//UART0->C2 = UART0->C2 | UART_C2_TIE_MASK            //Transmt Interrupt Enable          (Transmit Data Register Empty)
                                  //| UART_C2_TCIE_MASK          //Transmit Complete Interrupt Enable (Transmission Complete)
                                  //| UART_C2_RIE_MASK;           //Receiver Interrupt Enable         (Receive Data Register Full)
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->C2 |= UART_C2_RIE_MASK;

#endif

}

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
#else
void UART0_IRQHandler()
{
	char TXed_char;
	char RXed_char;

	// receive interrupt
	//Check receive buffer has data, RX_ring is not full
	if((UART0->S1 & UART_S1_RDRF_MASK) && (((RX_ring->Ini + 1) % RX_ring->Length) != RX_ring->Outi))
	{
		RXed_char = UART0->D;

		int store_result = insert(RX_ring, RXed_char);
		//If receive buffer is full, disable interrupts
		if(store_result == -1)
			UART0->C2 &= ~UART_C2_RIE_MASK;	// Disable RX Interrupt for control register 2

		add_stat(RXed_char);
		remove_entry(RX_ring, &RXed_char);
		insert(TX_ring, RXed_char);

		UART0->C2 |= UART_C2_RIE_MASK;	// Enable RX Interrupt for control register 2
	}

	// transmit interrupt
	//check register is empty, ring is not empty, previous transmission is complete
	if((UART0->S1 & UART_S1_TDRE_MASK) && (TX_ring->Outi != TX_ring->Ini) && (UART0->S1 & UART_S1_TC_MASK))
	{
		remove_entry(TX_ring, &TXed_char);
		UART0->D = TXed_char;
	}

}
#endif


