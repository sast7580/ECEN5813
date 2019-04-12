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
ring_t * fib_ring;
int stats[256];
uint32_t fib_arr[1000];
int fib_index = 2;
int report_flag = 0;

void fib_clear();

int main(void) {

	uint32_t baudrate = 115200;
    UART0_init(baudrate);
    for(int i = 0; i < 256; i++)
    {
    	stats[i] = 0;
    	fib_arr[i] = 0;
    }

    TX_ring = init(100);
    RX_ring = init(100);
    report_ring = init(10000);
    fib_ring = init(5000);




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


    uint32_t fib;
    fib_arr[0] = 0;
    fib_arr[1] = 1;

#endif


    //Main Loop
    while(1)
    {
#ifdef BLOCKING
	    char RX_char = UART_readData_blocking();
	    UART_writeData_blocking(RX_char);
#else
	    if(fib_index != sizeof(fib_arr)/sizeof(uint32_t))
	    {
			fib = fib_arr[fib_index-1] + fib_arr[fib_index-2];
			fib_arr[fib_index] = fib;
			fib_index++;
	    }
	    else
	    	clear_fib_arr();

	    for(int i = 0; i < 100000; i++){}
	    GPIOD->PTOR = (1<<1);
#endif
    }
    return 0;
}
