#include <LPC17xx.h>
#include <stdio.h>
#include "led.h"

#define LED_TOGGLE (1<<0)							//0th bit is made high
#define LED_ON() LPC_GPIO2->FIOSET = LED_TOGGLE	//Set P2.0 bit
#define LED_OFF() LPC_GPIO2->FIOCLR =  LED_TOGGLE	//clear P2.0 bit

unsigned int led_delay(int r1){
 	int r2;
	for(r2=0;r2<r1;r2++);
	return 0;
}

void blink_led(void){
	// CONFIG
	
	LPC_PINCON->PINSEL4 = 0x00000000;	 //P2 is made as GPIO
	LPC_GPIO2->FIODIR = 0x00000001;		 //p2.0 is output
	LPC_GPIO2->FIOCLR = 0x00000001;	 //Clear the P2.0 bit

	// BLINK LEDs
	LED_ON();						//On the LED
	led_delay(650000);
	LED_OFF();						//Off the LED
	led_delay(650000);	
}