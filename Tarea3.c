#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#define uno 0x01u
#define dos 0x02u
#define tres 0x03u
#define cuatro 0x04u
#define fin 0x05u
#define PIN22      22u
#define PIN26      26u
#define PIN21      21u
#define PIN6        6u
#define PIN4        4u
#define DELAY 1000000u
#define CORE_FREQ 21000000u

uint8_t Estado=uno;
void RGB(void);

/******ISR******/
//ISR PORT A sw3
void PORTA_IRQHandler(void)
{
	//Limpieza del valor de la interrupción en el registro SFR
    GPIO_PortClearInterruptFlags(GPIOA, 1U << PIN4);
    switch(Estado)
    {
    case uno: Estado=uno;
    break;
    case dos: Estado=tres;
    break;
    case tres: Estado=cuatro;
    break;
    case cuatro: Estado=cuatro;
    break;
    }
    //Macrofunción para habilitar un sistema de sincronización Periférico-Core
    SDK_ISR_EXIT_BARRIER;
}

//ISR PORT B sw2
void PORTC_IRQHandler(void)
{
	//Limpieza del valor de la interrupción en el registro SFR
    GPIO_PortClearInterruptFlags(GPIOC, 1U << PIN6);
    switch(Estado)
    {
    case uno: Estado=dos;
    break;
    case dos: Estado=dos;
    break;
    case tres: Estado=tres;
    break;
    case cuatro: Estado=uno;
    break;
    }
    SDK_ISR_EXIT_BARRIER;
}

/**************/

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


    while(Estado!=fin)
    {
       RGB();
    }
    return 0 ;
}



void RGB(void)
{
	while(Estado==uno)
	{
		//Amarillo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOE, 1u << PIN26);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Rojo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
		//Morado
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOB, 1u << PIN21);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOB, 1u << PIN21);
	}

	while(Estado==dos)
	{
		//Verde
		GPIO_PortClear(GPIOE, 1u << PIN26);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Azúl
		GPIO_PortClear(GPIOB, 1u << PIN21);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN21);
		//Rojo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
	}

	while(Estado==tres)
	{
		//Azúl
		GPIO_PortClear(GPIOB, 1u << PIN21);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN21);
		//Blanco
    	GPIO_PortClear(GPIOB, 1u << PIN22);
    	GPIO_PortClear(GPIOB, 1u << PIN21);
    	GPIO_PortClear(GPIOE, 1u << PIN26);
    	SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOB, 1u << PIN21);
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Rojo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
	}

	while(Estado==cuatro)
	{
		//Verde
		GPIO_PortClear(GPIOE, 1u << PIN26);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Morado
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOB, 1u << PIN21);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOB, 1u << PIN21);
		//Amarillo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOE, 1u << PIN26);
		SDK_DelayAtLeastUs(DELAY,CORE_FREQ);
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOE, 1u << PIN26);
	}
}









