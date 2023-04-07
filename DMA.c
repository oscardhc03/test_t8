/*
 * DMA.c
 *
 *  Created on: 5 abr 2023
 *      Author: 2399d
 */
#include "DMA.h"
#include "fsl_edma.h"
#define DMA_CHANNEL 0
#define DAC_CHANNEL 0
#define DMA_SOURCE 58 //source del eDMA para el PIT0


#define DMA_SOURCE_ADDR(src_buffer) ((uint32_t) (src_buffer) & 0x1FFFFFFF)
#define DMA_DAC_DEST_ADDR ((uint32_t)(&(DAC0->DAT[DAC_CHANNEL].DATL)) & 0x1FFFFFFF)

static edma_handle_t g_DMA_Handler;// g-->global
static edma_transfer_config_t transferConfig;

volatile bool g_Transfer_Done = false;

void DMA_IRQHandler(void){
	EDMA_HandleIRQ(&g_DMA_Handler);
}


void DMA_Callback(edma_handle_t *handle, void *param, bool transferdone, uint32_t transfer_control ){

	g_Transfer_Done = transferdone;

}

void DMA_init(void){
	//Inicializar DMAMUX
	DMAMUX_Init(DMAMUX0);
	DMAMUX_SetSource(DMAMUX0, DMA_CHANNEL, 45);//PAL DAC0
	DMAMUX_EnableChannel(DMAMUX0, DMA_CHANNEL);

	// eDMA INIT
	edma_config_t config;
	EDMA_GetDefaultConfig(&config);
	EDMA_Init(DMA0, &config);
	EDMA_CreateHandle(&g_DMA_Handler, DMA0, DMA_CHANNEL);
	EDMA_SetCallback(&g_DMA_Handler, DMA_Callback, NULL);

	NVIC_EnableIRQ(DMA0_IRQn);
	NVIC_SetPriority(DMA0_IRQn,2);
}


void DMA_config(uint32_t *src_buffer, uint32_t buffer_length){
	EDMA_PrepareTransfer(&transferConfig,
						    (void *) DMA_SOURCE_ADDR(src_buffer),
							sizeof (uint16_t),
							(void *)(uintptr_t)DMA_DAC_DEST_ADDR,
							sizeof(uint16_t),
							sizeof(uint16_t),
							buffer_length * sizeof(uint16_t),
							kEDMA_MemoryToMemory);

}


void DMA_StartTransfer(void){
//	edma_transfer_config_t transferConfig;
//	uint16_t *signal_buffer = Signals_GetActiveSignalBuffer();
//
//	EDMA_PrepareTransfer(&transferConfig, (void *)signal_buffer, sizeof(uint16_t),
//						(void*)&DAC0->DAT[0].DATL, sizeof(uint16_t), sizeof(uint16_t)* BUFFER_SIZE,
//						 sizeof(uint16_t) * BUFFER_SIZE, kEDMA_MemoryToMemory);
//	g_Transfer_Done = false;
//	EDMA_SubmitTransfer(&g_DMA_Handler, &transferConfig);
	EDMA_StartTransfer(&g_DMA_Handler);
}




