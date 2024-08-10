#include <RTL.h>           
#include <LPC17xx.h>                  	                
#include <stdio.h>
#include "buzzer.h"
#include "led.h"
#include "lcd.h"
#include "dcmotor.h"

#define MAX_CAP 10
#define TOTAL_CARTONS 10
#define PRESCALE (25000-1)

OS_TID tsk1, tsk2, tsk3;
OS_RESULT ret, RE1, RE2;
OS_SEM semaphore1;	// Declaration of a semaphore

os_mbx_declare (MsgBox, 100); // Declare an mailbox for IPC : 100 msgs
_declare_box(mpool,20,32);	 // Reserve a memory for 32 blocks of 20 bytes

char msg[10];
unsigned int cartons[TOTAL_CARTONS] = {10,9,8,3,1,0,1,2};
unsigned int allowed_flag = 1;
unsigned int i;
unsigned int index = 0;
unsigned int temp;
unsigned int temp4;
unsigned int j;
unsigned char print_msg[14] = {"MOTOR STOPPED!"};

__task void task1(void);
__task void task2(void);
__task void task3(void);

void main_delay(unsigned int i){
	unsigned int x;
	for(x=0; x<i; x++);
}

void initTimer0(void)
{
	// Assuming that PLL0 has been setup with CCLK = 100Mhz and PCLK = 25Mhz.
	LPC_SC->PCONP |= (1<<1); //Power up TIM0. By default TIM0 and TIM1 are enabled.
	LPC_SC->PCLKSEL0 &= ~(0x3<<3); //Set PCLK for timer = CCLK/4 = 100/4 (default)
	
	LPC_TIM0->CTCR = 0x0;
	LPC_TIM0->PR = PRESCALE; //Increment LPC_TIM0->TC at every 24999+1 clock cycles
	//25000 clock cycles @25Mhz = 1 mS
	
	LPC_TIM0->TCR = 0x02; //Reset Timer
}


void delayMS(unsigned int milliseconds) //Using Timer0
{
	LPC_TIM0->TCR = 0x02; //Reset Timer
	LPC_TIM0->TCR = 0x01; //Enable timer
	
	while(LPC_TIM0->TC < milliseconds); //wait until timer counter reaches the desired delay
	
	// LPC_TIM0->TCR = 0x00; //Disable timer
}

void serial(){
	LPC_SC->PCONP |= 0x00000008;				// UART0 peripheral enable
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;			// P0.2 - TXD0 and P0.3 - RXD0
	LPC_UART0->LCR = 0x00000083;				// enable divisor latch, parity disable, 1 stop bit, 8bit word length
	LPC_UART0->DLM = 0X00; 
	LPC_UART0->DLL = 0xA2;      				// select baud rate 9600 bps
	LPC_UART0->LCR = 0X00000003;
	LPC_UART0->FCR = 0x07;
	LPC_UART0->IER = 0X03;	   					// select Transmit and receive interrupt	
}

void send_string(char *s){
	while(*s!='\0')
	{
		while(!(LPC_UART0->LSR&0x20));
		LPC_UART0->THR=*s;
		s++;
	}
}
void task1(void){
// TASK 1 : To run the DC Motor as long as carton has valid soaps
	tsk1 = os_tsk_self();
	tsk2 = os_tsk_create(task2,10); 

	while(1){
		if(allowed_flag == 1){
			for(i=0; i<TOTAL_CARTONS; i++){
				if(cartons[i] == 0){
					// If empty carton detected

					// Stop the motor by turning off the coils and setting P1.24 low
				    LPC_GPIO0->FIOCLR = 0x04000000; // P0.26 coil is off
				    LPC_GPIO1->FIOCLR = 0x01000000; // P1.24 Motor is off

					os_dly_wait(50);
					os_evt_set(0x001 , tsk2);
					os_tsk_prio_self(0);
					os_dly_wait(20);
				}else{
					sprintf(msg, "CARTON SIZE: %d\r\n", cartons[i]);
					main_delay(100000);
					initTimer0();
					delayMS(1);

					Clock_Wise();
			        for (j = 0; j < 2500; j++); // Reduced delay
			        AClock_Wise();
			        for (j = 0; j < 2500; j++); // Reduced delay

					// Stop the motor by turning off the coils and setting P1.24 low
				    LPC_GPIO0->FIOCLR = 0x04000000; // P0.26 coil is off
				    LPC_GPIO1->FIOCLR = 0x01000000; // P1.24 Motor is off

					send_string(msg);
				}
			}
		}else{
			// Send task to ready queue
			os_tsk_prio_self(0);
		}
	}
}

void task2(void){
	// TASK 2 : To log the info of the faulty carton
	U32 *mptr;
	os_mbx_init(MsgBox, sizeof(MsgBox));
	mptr = _alloc_box (mpool);

	while(1){
		RE1=os_evt_wait_and(0x0001, 0x0001); 
		if(RE1== OS_R_EVT){
			temp1 = 0x80;
			lcd_com();
			delay_lcd(800);
			lcd_puts(&print_msg[0]);


			mptr[0] = temp; 
			os_mbx_send(MsgBox, mptr, 0xffff);
			os_tsk_prio_self(0);
			tsk3 = os_tsk_create(task3,10);			
		}
	}
}

void task3(void){
	// TASK 3 : To ring buzzer when fault carton
	U32 *rptr;
	os_mbx_wait(MsgBox, (void*)&rptr, 0xffff);
		
	temp4 = rptr[0];

	while(1){
		sprintf(msg, "Faulty CARTON: %d\r\n", i);
		send_string(msg);
		blink_led();
		main_delay(1000);
		allowed_flag =0;

		os_tsk_prio_self(11);
	}
}


int main(){
	LPC_SC->PCONP |= (1<<15);					// Power for GPIO block
	LPC_PINCON->PINSEL1 &= 0xFFF3FFFF;			// P0.25 GPIO
	LPC_GPIO0->FIODIR |= 0x002000000;			// P0.25 output
	
	initTimer0();
    lcd_init();
	serial();
	
	os_sys_init_prio(task1,10);
}