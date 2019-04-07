#include <stdio.h>
#include "frdm.h"
#include "ring.h"
#include "stdlib.h"
#include "MKL25Z4.h"

#ifndef BLOCKING_METHOD_H_
#define BLOCKING_METHOD_H_

#define BAUD_RATE			115200
#define BLOCKING			0
#define NONBLOCKING			1
#define BLOCKING_TYPE		BLOCKING

void blocking_method(BLOCKING_TYPE);
void initialize_UART0_interrupts(int baud);
void initialize_UART0_polling(int baud);
void IRQ0();
void disable_interrupt(int interrupt);
void enable_interrupt(int interrupt);
void enable_leds()

#endif