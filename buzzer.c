#include <LPC17xx.H>
#include "buzzer.h"

void buz_delay(unsigned int j)
{
		unsigned int i;
		for(i=0;i<j;i++);
}

void ring_buzzer(void)
{
	// CONFIG
	LPC_PINCON->PINSEL1 &= 0xFFFCFFFF;	//P0.24 is GPIO
	LPC_GPIO0->FIODIR |= 0x01000000;	//P0.24 is output
	
	// RING BUZZER
	LPC_GPIO0->FIOSET = 0x01000000;	//P0.24 made high to turn ON the buzzer
	buz_delay(650000);
	LPC_GPIO0->FIOCLR = 0x01000000;	//P0.24 made low to turn OFF the buzzer
	buz_delay(650000);
}
