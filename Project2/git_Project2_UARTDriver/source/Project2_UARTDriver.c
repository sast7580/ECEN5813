/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Project2_UARTDriver.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "ring.h"
#include "uart_functions.h"


ring_t * TX_ring;
ring_t * RX_ring;
ring_t * report_ring;
int stats[256];


int main(void) {

	uint32_t baudrate = 115200;
    UART0_init(baudrate);
    enable_leds();

    for(int i = 0; i < 256; i++)
    	stats[i] = 0;

    TX_ring = init(14641);
    RX_ring = init(14641);
    report_ring = init(14641);

#ifdef ELEMENTS
    UART_writeData_blocking('\n');
    UART_writeData_blocking('\r');
    UART_writeData_blocking('R');
    UART_writeData_blocking('X');
    UART_writeData_blocking(' ');
    UART_writeData_blocking('E');
    UART_writeData_blocking('l');
    UART_writeData_blocking('e');
    UART_writeData_blocking('m');
    UART_writeData_blocking('e');
    UART_writeData_blocking('n');
    UART_writeData_blocking('t');
    UART_writeData_blocking('s');
    UART_writeData_blocking(' ');
    UART_writeData_blocking('=');
    UART_writeData_blocking(' ');
#else

#ifdef BLOCKING
    UART_writeData_blocking('\n');
    UART_writeData_blocking('\r');
    UART_writeData_blocking('B');
    UART_writeData_blocking('l');
    UART_writeData_blocking('o');
    UART_writeData_blocking('c');
    UART_writeData_blocking('k');
    UART_writeData_blocking('i');
    UART_writeData_blocking('n');
    UART_writeData_blocking('g');
    UART_writeData_blocking('\n');
    UART_writeData_blocking('\r');
#else

#ifdef NONBLOCKING
    UART_writeData_blocking('\n');
    UART_writeData_blocking('\r');
    UART_writeData_blocking('I');
    UART_writeData_blocking('n');
    UART_writeData_blocking('t');
    UART_writeData_blocking('e');
    UART_writeData_blocking('r');
    UART_writeData_blocking('r');
    UART_writeData_blocking('u');
    UART_writeData_blocking('p');
    UART_writeData_blocking('t');
    UART_writeData_blocking('s');
    UART_writeData_blocking('\n');
    UART_writeData_blocking('\r');
#else
    // fibonacci
    UART_writeData_blocking('\n');
    UART_writeData_blocking('\r');
    UART_writeData_blocking('F');
    UART_writeData_blocking('i');
    UART_writeData_blocking('b');
    UART_writeData_blocking('o');
    UART_writeData_blocking('n');
    UART_writeData_blocking('a');
    UART_writeData_blocking('c');
    UART_writeData_blocking('c');
    UART_writeData_blocking('i');
    UART_writeData_blocking('\n');
    UART_writeData_blocking('\r');
#endif
#endif
#endif

    //Main Loop
    while(1)
    {
#ifdef ELEMENTS
	    wait(1000);
    	int count = entries( RX_ring );
	    char string = '0' + count;
	    wait(1000);
	    UART_writeData_blocking(string);
#else

#ifdef BLOCKING
	    char RX_char = UART_readData_blocking();
	    UART_writeData_blocking(RX_char);
#else

#ifdef NONBLOCKING
	    // toggle blue LED
	    GPIOD->PTOR = (1 << 1);
	    wait(30);
	    GPIOD->PTOR = (1 << 1);
	    wait(30);
#else
	    // fibonacci
	    int first = 1;
	    int previous = 0;
	    int next;

        UART_writeData_blocking('\n');
        UART_writeData_blocking('\r');

	    while (first < 1000)
	    {
	    	next = first + previous;

	    	if(first < 10)
	    	{
	    		char first_char = '0' + first;

	    		UART_writeData_blocking(first_char);
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 13)
	    	{
	    		UART_writeData_blocking('1');
	    		UART_writeData_blocking('3');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 21)
	    	{
	    		UART_writeData_blocking('2');
	    		UART_writeData_blocking('1');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 34)
	    	{
	    		UART_writeData_blocking('3');
	    		UART_writeData_blocking('4');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 55)
	    	{
	    		UART_writeData_blocking('5');
	    		UART_writeData_blocking('5');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 89)
	    	{
	    		UART_writeData_blocking('8');
	    		UART_writeData_blocking('9');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 144)
	    	{
	    		UART_writeData_blocking('1');
	    		UART_writeData_blocking('4');
	    		UART_writeData_blocking('4');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 233)
	    	{
	    		UART_writeData_blocking('2');
	    		UART_writeData_blocking('3');
	    		UART_writeData_blocking('3');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 377)
	    	{
	    		UART_writeData_blocking('3');
	    		UART_writeData_blocking('7');
	    		UART_writeData_blocking('7');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 610)
	    	{
	    		UART_writeData_blocking('6');
	    		UART_writeData_blocking('1');
	    		UART_writeData_blocking('0');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	if(first == 987)
	    	{
	    		UART_writeData_blocking('9');
	    		UART_writeData_blocking('8');
	    		UART_writeData_blocking('7');
	    		UART_writeData_blocking(',');
	    		UART_writeData_blocking(' ');
	    	    wait(3000);
	    	}
	    	previous = first;
	    	first = next;
	    }
#endif
#endif
#endif
    }

    return 0;
}
