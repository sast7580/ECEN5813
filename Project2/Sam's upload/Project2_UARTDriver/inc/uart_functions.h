/*
 * uart_functions.h
 *
 *  Created on: Apr 7, 2019
 *      Author: sam
 */

#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "ring.h"

//#define BLOCKING	1
//#define ECHO		1

extern ring_t * RX_ring;
extern ring_t * TX_ring;
extern ring_t * report_ring;
extern ring_t * fib_ring;
extern int stats[256];
extern uint32_t fib_arr[1000];
extern int fib_index;

void UART0_init(uint32_t baudrate);
void UART_writeData_blocking(char TXed_char);
char UART_readData_blocking();
bool check_TX_ready();
bool check_RX_ready();
int TX_char(char TX);
int tx_ring(ring_t * ring);

void report_stats();
void clear_fib_arr();


#endif /* UART_FUNCTIONS_H_ */
