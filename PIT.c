/*
 * PIT.c
 *
 *  Created on: 5 abr 2023
 *      Author: 2399d
 */

#include "PIT.h"
#include "DMA.h"

#define PIT_CLOCK 50000000
#define PIT_LDVAL(freq)(PIT_CLOCK / freq) - 1

void PIT0_IRQHandler(void){
	PIT->CHANNEL[bit_0].TFLG |= PIT_TFLG_TIF_MASK;
//	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	DMA_StartTransfer();
}
void PIT_init(uint32_t period){
	//Habilitar clock del PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

	//Habilitar el PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;

	NVIC_EnableIRQ(PIT0_IRQn);
	NVIC_SetPriority(PIT0_IRQn, 3);

	PIT->CHANNEL[bit_0].LDVAL = PIT_LDVAL(500000); // freq de 500 kHz
	PIT->CHANNEL[bit_0].TCTRL = PIT_TCTRL_TIE_MASK; //Habilita Interrupcion

}

void PIT_set_freq(uint32_t frequency){
	PIT->CHANNEL[bit_0].LDVAL = PIT_LDVAL(frequency);// valor de carga del PIT para la frecuencia deseada
}

void PIT_enable(void){
	PIT->CHANNEL[bit_0].TCTRL |= PIT_TCTRL_TEN_MASK;//Habilita temporizador del PIT0
}
