#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "RGB.h"



gpio_pin_config_t sw_config = {
       kGPIO_DigitalInput,
       0,
   };



gpio_pin_config_t led_config = {
       kGPIO_DigitalOutput,
       1,
   };

int main(void) {
	//Estructura de configuraciones para el PIN a utilizar
	const port_pin_config_t porta_pin4_config = {
		    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
		    kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		    kPORT_HighDriveStrength,                                 /* High drive strength is configured */
		    kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
		    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
		  };
	//Habilitación de reloj
	CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortE);

	/***********CONFIFURACIÓN DE INTERRUPCIÓN EN SWITCH*******/

	//Configuración del PIN: Puerto/PIN/Estructura de configuración para...
	PORT_SetPinConfig(PORTA, PIN4, &porta_pin4_config);
	PORT_SetPinConfig(PORTC, PIN6, &porta_pin4_config);

	//Selección del tipo de interrupción para el PIN (en este caso un flanco de bajada)
	PORT_SetPinInterruptConfig(PORTA, PIN4, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTC, PIN6, kPORT_InterruptFallingEdge);
	/***********************************************************/

	//Activación del pin como GPIO en el registro PCR para el puerto PORTE y PORTC
	PORT_SetPinConfig(PORTE, PIN26, &porta_pin4_config);
    PORT_SetPinConfig(PORTB, PIN22, &porta_pin4_config);
	PORT_SetPinConfig(PORTB, PIN21, &porta_pin4_config);

	//Configuración del PIN con el registro del GPIO para seleccionar el comportamiento I/O
	GPIO_PinInit(GPIOE, PIN26, &led_config);
	GPIO_PinInit(GPIOB, PIN22, &led_config);
	GPIO_PinInit(GPIOB, PIN21, &led_config);
	GPIO_PinInit(GPIOC, PIN6, &sw_config);
	GPIO_PinInit(GPIOA, PIN4, &sw_config);

	//Inicialización de interrupción
	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTC_IRQn);
	//Nivel de prioridad (la prioridad más baja es 16)
    NVIC_SetPriority(PORTA_IRQn, 2);
    NVIC_SetPriority(PORTC_IRQn, 2);


    while(1)
    {
       RGB();
    }
    return 0 ;
}













