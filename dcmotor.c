#include <lpc17xx.h>
#include <stdio.h>

#define TRIG (1 << 0) // P0.0
#define ECHO (1 << 1) // P0.1

unsigned int x;

void Clock_Wise(void) {

	LPC_PINCON->PINSEL4 = 0x00000000; // P2.0 to P2.3 GPIO
    LPC_GPIO2->FIODIR = 0x0000000F;   // P2.0 to P2.3 output
    LPC_PINCON->PINSEL1 &= 0xFFCFFFFF; // P0.26 GPIO, P0.26 controls dir
    LPC_PINCON->PINSEL3 &= 0xFFFCFFFF; // P1.24 GPIO
    LPC_GPIO0->FIODIR |= 0x04000000;   // P0.26 output
    LPC_GPIO1->FIODIR |= 0x01000000;   // P1.24 output

    LPC_GPIO1->FIOCLR = 0x01000000; // P1.24 Kept low to turn off DC Motor
    for (x = 0; x < 1000; x++); // Delay to compensate for inertia
    LPC_GPIO0->FIOSET = 0x04000000; // P0.26 coil is on
    LPC_GPIO1->FIOSET = 0x01000000; // P1.24 motor is on
    for (x = 0; x < 500000; x++); // Delay to make the rotation visible
    return;
}

void AClock_Wise(void) {

	LPC_PINCON->PINSEL4 = 0x00000000; // P2.0 to P2.3 GPIO
    LPC_GPIO2->FIODIR = 0x0000000F;   // P2.0 to P2.3 output
    LPC_PINCON->PINSEL1 &= 0xFFCFFFFF; // P0.26 GPIO, P0.26 controls dir
    LPC_PINCON->PINSEL3 &= 0xFFFCFFFF; // P1.24 GPIO
    LPC_GPIO0->FIODIR |= 0x04000000;   // P0.26 output
    LPC_GPIO1->FIODIR |= 0x01000000;   // P1.24 output

    LPC_GPIO1->FIOCLR = 0x01000000; // P1.24 Kept low to turn off DC Motor
    for (x = 0; x < 1000; x++); // Delay to compensate for inertia
    LPC_GPIO0->FIOCLR = 0x04000000; // P0.26 coil is off
    LPC_GPIO1->FIOSET = 0x01000000; // P1.24 Motor is on
    for (x = 0; x < 500000; x++); // Delay to make the rotation visible
    return;
}
