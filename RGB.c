#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_pit.h"
#include "RGB.h"


/*******************************************************************************
 * Definitions PIT
 ******************************************************************************/

#define DEMO_PIT_BASEADDR PIT
#define DEMO_PIT_CHANNEL  kPIT_Chnl_0
#define PIT_LED_HANDLER   PIT0_IRQHandler
#define PIT_IRQ_ID        PIT0_IRQn
// Get source clock for PIT driver
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define LED_INIT()       LED_RED_INIT(LOGIC_LED_ON)
#define LED_TOGGLE()     LED_RED_TOGGLE()

volatile bool pitIsrFlag = false;

struct Estados Estado = {uno};

/******ISR******/
//ISR PORT A sw3
void PORTA_IRQHandler(void)
{
	//Limpieza del valor de la interrupción en el registro SFR
    GPIO_PortClearInterruptFlags(GPIOA, 1U << PIN4);
    switch(Estado.Valor)
    {
    case uno: Estado.Valor=uno;
    break;
    case dos: Estado.Valor=tres;
    break;
    case tres: Estado.Valor=cuatro;
    break;
    case cuatro: Estado.Valor=cuatro;
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
    switch(Estado.Valor)
    {
    case uno: Estado.Valor=dos;
    break;
    case dos: Estado.Valor=dos;
    break;
    case tres: Estado.Valor=tres;
    break;
    case cuatro: Estado.Valor=uno;
    break;
    }
    SDK_ISR_EXIT_BARRIER;
}

//ISR PIT
void PIT_LED_HANDLER(void)
{
    // Clear interrupt flag.
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerFlag);
    PRINTF("\r\n Interrupcion !");
    pitIsrFlag = true;
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */

    __DSB();
}
/**************/

void RGB(void)
{
	    /* Structure of initialize PIT */
	    pit_config_t pitConfig; //Variable boleana: true: Timers run in debug mode; false: Timers stop in debug mode

	    /* Board pin, clock, debug console init */
	    BOARD_InitPins();
	    BOARD_BootClockRUN();
	    BOARD_InitDebugConsole();

	    /* Initialize and enable LED */
	    LED_INIT();

	    /*
	     * pitConfig.enableRunInDebug = false;
	     */
	    PIT_GetDefaultConfig(&pitConfig);

	    /* Init pit module */
	    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);

	    /* Set timer period for channel 0 */
	    PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK));

	    /* Enable timer interrupts for channel 0 */
	    PIT_EnableInterrupts(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerInterruptEnable);

	    /* Enable at the NVIC */
	    EnableIRQ(PIT_IRQ_ID);

	    /* Start channel 0 */
	    PIT_StartTimer(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL);
	while(Estado.Valor==uno)
	{
		//Amarillo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOE, 1u << PIN26);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Rojo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN22);
		//Morado
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOB, 1u << PIN21);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOB, 1u << PIN21);
	}

	while(Estado.Valor==dos)
	{
		//Verde
		GPIO_PortClear(GPIOE, 1u << PIN26);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Azúl
		GPIO_PortClear(GPIOB, 1u << PIN21);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN21);
		//Rojo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;;
		GPIO_PortSet(GPIOB, 1u << PIN22);
	}

	while(Estado.Valor==tres)
	{
		//Azúl
		GPIO_PortClear(GPIOB, 1u << PIN21);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN21);
		//Blanco
    	GPIO_PortClear(GPIOB, 1u << PIN22);
    	GPIO_PortClear(GPIOB, 1u << PIN21);
    	GPIO_PortClear(GPIOE, 1u << PIN26);
    	while (false == pitIsrFlag){}
    	pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOB, 1u << PIN21);
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Rojo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN22);
	}

	while(Estado.Valor==cuatro)
	{
		//Verde
		GPIO_PortClear(GPIOE, 1u << PIN26);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOE, 1u << PIN26);
		//Morado
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOB, 1u << PIN21);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOB, 1u << PIN21);
		//Amarillo
		GPIO_PortClear(GPIOB, 1u << PIN22);
		GPIO_PortClear(GPIOE, 1u << PIN26);
		while (false == pitIsrFlag){}
		pitIsrFlag=false;
		GPIO_PortSet(GPIOB, 1u << PIN22);
		GPIO_PortSet(GPIOE, 1u << PIN26);
	}
}
