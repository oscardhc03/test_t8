#include "DAC.h"
#include "DMA.h"
#include "GPIO.h"
#include "NVIC.h"
#include "PIT.h"
#include "Senales.h"

int main(void) {
    // Initialize modules
    DAC_init();
    DMA_init();
    GPIO_config();
    PIT_init(500000); // Initialize PIT with a frequency of 500 kHz

    // Set initial signal and frequency
    DAC_Set_signal(SIGNAL_SINE, 1000);

    // Enable PIT
    PIT_enable();

    while (1) {
        // The rest of the program will be handled by interrupts
    }

    return 0;
}


















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
