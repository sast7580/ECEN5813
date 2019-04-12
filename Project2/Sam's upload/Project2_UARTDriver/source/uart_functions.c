/*
 * uart_functions.c
 *
 *  Created on: Apr 7, 2019
 *      Author: sam
 */
#include "uart_functions.h"

void clear_fib_arr()
{
    	fib_arr[0] = 0;
    	fib_arr[1] = 1;
    	fib_arr[2] = 1;
    	for(int i = 2; i<(sizeof(fib_arr)/sizeof(uint32_t)); i++)
    	{
    		fib_arr[i] = 0;
    	}
    	fib_index = 2;
}

void report_stats()
{

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

			char number[5];
			sprintf(number, "%d", stats[i]);
			int j = 0;
			while(number[j] != '\0')
			{
				insert(report_ring, number[j]);
				j++;
			}
		}
	}

	insert(report_ring, '\n');
	insert(report_ring, '\r');
	insert(report_ring, '-');
	insert(report_ring, '-');
	insert(report_ring, '-');
	tx_ring(report_ring);



	insert(fib_ring, 'F');
	insert(fib_ring, 'i');
	insert(fib_ring, 'b');
	insert(fib_ring, 'b');
	insert(fib_ring, 'o');
	insert(fib_ring, 'n');
	insert(fib_ring, 'a');
	insert(fib_ring, 'c');
	insert(fib_ring, 'c');
	insert(fib_ring, 'i');
	insert(fib_ring, '-');
	insert(fib_ring, '-');
	insert(fib_ring, '-');
	insert(fib_ring, '\n');
	insert(fib_ring, '\r');

	insert(fib_ring, '0');
	insert(fib_ring, ',');
	insert(fib_ring, '1');
	insert(fib_ring, ',');

	int f = 2;
	while(fib_arr[f] != 0)
	{
		char number[5];
		sprintf(number, "%zu", fib_arr[f]);
		int j = 0;
		while(number[j] != '\0')
		{
			insert(fib_ring, number[j]);
			j++;
		}
		f++;
		insert(fib_ring, ',');
	}

	insert(fib_ring, '\n');
	insert(fib_ring, '\r');

	tx_ring(fib_ring);
	clear_fib_arr();

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
	 //SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;

	 //Set Port A Pin Configurations to alternative 2
	 PORTA->PCR[1] |= PORT_PCR_MUX(2);
	 PORTA->PCR[2] |= PORT_PCR_MUX(2);

	 //LED setup
	 SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	 GPIOD->PSOR |= (1<<1);
	 GPIOD->PDDR |= (1<<1);
	 PORTD->PCR[1] |= PORT_PCR_MUX(1);

    //Disable TX and RX before changing baud
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
	NVIC_EnableIRQ(UART0_IRQn);

	UART0->C2 |= UART_C2_RIE(1);
//	UART0->C2 |= UART_C2_TIE(1);
//    UART0->C2 |= UART_C2_TCIE(1);

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

	char RXed_char;

	//NVIC_DisableIRQ(UART0_IRQn);
	//Check receive buffer has data, RX_ring is not full
	if(check_RX_ready())
	{
		RXed_char = UART0->D;
		bool check = check_RX_ready();
		insert(RX_ring, RXed_char);
		insert(TX_ring, RXed_char);
		remove_entry(RX_ring, &RXed_char);
		stats[((int) RXed_char)]++;
	}

	if(check_TX_ready())
	{
#ifdef ECHO
		tx_ring(TX_ring);
#endif
		if(entries(RX_ring) == 0)
		{
			report_stats();
		}
	}

	//NVIC_EnableIRQ(UART0_IRQn);
}

bool check_TX_ready()
{
	//check TX register is empty and previous transmission is complete
	if((UART0->S1 & UART_S1_TDRE_MASK) && (UART0->S1 & UART_S1_TC_MASK))
		return true;
	else
		return false;
}

bool check_RX_ready()
{
	//check TX register is empty and previous transmission is complete
	if((UART0->S1 & UART_S1_RDRF_MASK))
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

int TX_char(char TX)
{
	ring_t * TX_ring = init(4);
	insert(TX_ring, TX);
	char removed;

	//check TX is ready and ring is not empty
	if(check_TX_ready() && entries(TX_ring) > 0)
	{
		remove_entry(TX_ring, &removed);
		UART0->D = removed;
		return 0;
	}
	else
		return -1;
}

#endif


