/*
 * PIT.h
 *
 *  Created on: 5 abr 2023
 *      Author: 2399d
 */

#ifndef PIT_H_
#define PIT_H_

#include "fsl_pit.h"
#include "NVIC.h"
#include "GPIO.h"
#include "DMA.h"
#include "Senales.h"

void  PIT0_IRQHandler(void);

void PIT_init(uint32_t period);

void PIT_set_freq(uint32_t frequency);

void PIT_enable(void);
#endif /* PIT_H_ */
