#ifndef RGB_H_
#define RGB_H_
#include <stdio.h>

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

struct Estados
{
	uint8_t Valor;
};

void RGB(void);
void PORTA_IRQHandler(void);
void PORTC_IRQHandler(void);


#endif /* RGB_H_ */
