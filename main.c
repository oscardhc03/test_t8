#include "DAC.h"
#include "DMA.h"
#include "GPIO.h"
#include "NVIC.h"
#include "PIT.h"
#include "Senales.h"
#include "bits.h"

#define PIT_IRQ_PRIORITY 2

#define PIT_DEFAULT_FREQ 500000U
#define PIT_MAX_FREQ 1500000U
#define PIT_FREQ_STEP 500000U

volatile bool sw2_pressed = false;
volatile bool sw3_pressed = false;
volatile uint32_t pit_freq = PIT_DEFAULT_FREQ;
volatile uint8_t waveform_type = sine_wave;

/* DMA channel configuration */
//dma_handle_t g_dmaHandle;
//dma_transfer_config_t g_transferConfig;

/* DAC buffer */
//uint16_t dacBuffer[DAC_BUFFER_SIZE];

/* PIT interrupt handler */
void PIT_IRQHandler(void) {
    /* Clear interrupt flag */
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

    /* Update PIT period based on current frequency */
    uint32_t current_period = USEC_TO_COUNT(1000000U / pit_freq, CLOCK_GetFreq(kCLOCK_BusClk));
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, current_period);

    /* Transfer data to DAC */
    DMA_SubmitTransfer(&g_dmaHandle, &g_transferConfig);
    DMA_StartTransfer(&g_dmaHandle);
}

/* SW2 interrupt handler */
void GPIO_SW2_IRQHandler(void) {
    sw2_pressed = true;
    NVIC_ClearPendingIRQ(GPIO_SW2_IRQn);
}

/* SW3 interrupt handler */
void GPIO_SW3_IRQHandler(void) {
    sw3_pressed = true;
    NVIC_ClearPendingIRQ(GPIO_SW3_IRQn);
}


int main(void) {
    /* Board and clock initialization */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* DMA, PIT, DAC and GPIO initialization */
    DMA_Init();
    PIT_Init();
    DAC_Init();
    GPIO_Init();
    NVIC_SetPriority(PIT_IRQn, PIT_IRQ_PRIORITY);
    NVIC_SetPriority(GPIO_SW2_IRQn, 1);
    NVIC_SetPriority(GPIO_SW3_IRQn, 1);

    /* Generate waveforms in memory */
    generate_waveforms();

    /* Configure DMA channel */
    DMA_CreateHandle(&g_dmaHandle, DMA0, 0U);
    DMA_SetCallback(&g_dmaHandle, DMA_Callback, NULL);
    DMA_PrepareTransfer(&g_transferConfig, &dacBuffer, sizeof(dacBuffer[0]), (void *)&sine_wave, sizeof(sine_wave[0]), sizeof(sine_wave), kDMA_MemoryToPeripheral);
    DMA_EnableChannel(DMA0, 0U);

    /* Configure PIT channel */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U / pit_freq, CLOCK_GetFreq(kCLOCK_BusClk)));
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    while (1) {
        if (sw2_pressed) {
            /* Update frequency */
            pit_freq += PIT_FREQ_STEP;
            if (pit_freq > PIT_MAX_FREQ) {
                pit_freq = PIT_DEFAULT_FREQ;
            }

            /* Clear flag */
            sw2_pressed = false;
        }

        if (sw3_pressed) {
            /* Update waveform */
            waveform_type++;
            if (waveform_type > WAVEFORM_TYPE_SAWTOOTH) {
                waveform_type = WAVEFORM_TYPE_SINE;
            }

            /* Update DAC buffer */
            switch (waveform_type) {
                case WAVEFORM_TYPE_SINE:
                    for (int i = 0; i < DAC_BUFFER_SIZE; i++) {
                        dacBuffer[i] = sine_wave[i];
                    }
                    break;
                case WAVEFORM_TYPE_TRIANGLE:
                    for (int i = 0; i < DAC_BUFFER_SIZE; i++) {
                        dacBuffer[i] = triangle_wave[i];
                    }
                    break;
                case WAVEFORM_TYPE_SAWTOOTH:
                    for (int i = 0; i < DAC_BUFFER_SIZE; i++) {
                        dacBuffer[i] = sawtooth_wave[i];
                    }
                    break;
                default:
                    break;
            }

            /* Clear flag */
            sw3_pressed = false;
        }
    }
}

























//#include "DAC.h"
//#include "DMA.h"
//#include "GPIO.h"
//#include "NVIC.h"
//#include "PIT.h"
//#include "Senales.h"
//
//int main(void) {
//    // Initialize modules
//    DAC_init();
//    DMA_init();
//    GPIO_config();
//    PIT_init(500000); // Initialize PIT with a frequency of 500 kHz
//
//    // Set initial signal and frequency
//    DAC_Set_signal(SIGNAL_SINE, 1000);
//
//    // Enable PIT
//    PIT_enable();
//
//    while (1) {
//        // The rest of the program will be handled by interrupts
//    }
//
//    return 0;
//}
//
//
//
//














//#include "DAC.h"
//#include "DMA.h"
//#include "GPIO.h"
//#include "NVIC.h"
//#include "PIT.h"
//#include "Senales.h"
//
// void switch_callback(PORT_Type *base, uint32_t pin) {
//    if (base == PORTA && pin == 4) {
//        // Cambiar la frecuencia
//    	Signal_NexFreq();
//    } else if (base == PORTC && pin == 6) {
//        // Cambiar señal
//    	Signal_NexType();
//    }
//}
//
//int main(void) {
//    // Inicializar los módulos
//    DAC_init();
//    DMA_init();
//    GPIO_config();
//    PIT_init(500000); // frecuencia inicial de 500 kHz
//    NVIC_enable_interrupt_and_priotity(PORTA_IRQn, 1);
//    NVIC_enable_interrupt_and_priotity(PORTC_IRQn, 1);
//
//    // Configurar la señal inicial
////    Signal_Init();
//    DAC_Set_signal(SIGNAL_SINE, 500000);
//
//    // Configurar DMA para transferencia de señal
//    uint16_t *signal_buffer = Signals_GetActiveSignalBuffer();
//    uint32_t buffer_length = BUFFER_SIZE;
//    DMA_config(signal_buffer, buffer_length);
//
//    // Establecer el callback de los switches
//    GPIO_callbck(switch_callback);
//
//    // Habilitar el PIT
//    PIT_enable();
//
//    while (1) {
//        // Actualizar la señal y la frecuencia si es necesario
//        signal_type_t signal_type = Signal_GetCurrentType();
//        uint32_t frequency = Signal_GetCurrentFreq();
//        DAC_Set_signal(signal_type, frequency);
//        PIT_set_freq(frequency);
//
//        // Verificar si se ha cambiado el tipo de señal
//        if (Signal_IsSignalChanged()) {
//            signal_buffer = Signals_GetActiveSignalBuffer();
//            DMA_config(signal_buffer, buffer_length);
//            Signal_ClearSignalChangedFlag();
//        }
//    }
//
//    return 0;
//}
//
//





























//#include "board.h"
//#include "pin_mux.h"
//#include "clock_config.h"
////#include "fsl_debu"
//
//#include "GPIO.h"
//#include "DAC.h"
//#include "PIT.h"
//#include "DMA.h"
//#include "Senales.h"
//volatile bool sw2_push =false;
//volatile bool sw3_push =false;
//
//void switch_callback(PORT_Type *base, uint32_t pin){
//	if(base== PORTA && pin ==4){
//		sw2_push ==true;
//	}else if(base== PORTC && pin ==6){
//		sw3_push = true;
//	}
//}
//
//int main(void){
//
//	BOARD_InitPins();
//	BOARD_BootClockRUN();
////	BOARD_InitDebugConsole();
//	//Modulos
//
//	DAC_init();
//	DMA_init();
//	GPIO_config();
//	NVIC_in
//	EnableIRQ(PIT0_IRQn);
//
//	//Config señal inicial y freq inicial
//	Signal_NexType();
//	Signal_NexFreq();
//	while(1)
//	{
//		if(!GPIO_PinRead(GPIOA, 4))
//		{
//			Signal_NexFreq();//Change freq
//
//			//Esperar a que el boton se suelte y agregamos un delay
//			while(!GPIO_PinRead(GPIOA, 4))
//			{
//			SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
//			}
//		}
//
//		//Verificamos el SW3
//		if(!GPIO_PinRead(GPIOC, 6))
//		{
//			Signal_NexType();
//
//			while(!GPIO_PinRead(GPIOC, 6)){
//				SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
//			}
//
//		}
//	}
//	return 0;
//
//}
