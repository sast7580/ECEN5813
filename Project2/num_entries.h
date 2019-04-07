#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "UARTdriver.h"
#include "ring.h"

#ifdef LINUX
#include "frdm.h"
#endif

#ifndef NUM_ENTRIES_H
#define NUM_ENTRIES_H

int entries( ring_t *ring );

#endif