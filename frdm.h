/*
 * frdm.h
 *
 *  Created on: Mar 10, 2019
 *      Author: sam
 */

#ifndef PROJECT1_FRDM_H_
#define PROJECT1_FRDM_H_

#define FRDM 1
#ifdef FRDM
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "readin.h"
#define printf PRINTF
#endif


#endif /* PROJECT1_FRDM_H_ */
