/*
 * interrupts.c
 *
 *  Created on: Jan 18, 2022
 *      Author: EmreDemir
 */
#include "interrupts.h"

/* Initialize the banked stack pointer register for IRQ mode */
void set_A9_IRQ_stack(void)
{
	int stack, mode;
	stack = 0xFFFFFFFF - 7; // top of A9 on-chip memory, aligned to 8 bytes
	/* change processor to IRQ mode with interrupts disabled */
	mode = 0b11010010;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r" (stack));
	/* go back to SVC mode before executing subroutine return! */
	mode = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

/* Turn on interrupts in the ARM processor */
void enable_A9_interrupts(void)
{
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}
/* Configure the Generic Interrupt Controller (GIC) */
void config_GIC(void)
{
	/* configure the FPGA interval timer and KEYs interrupts */
	*((int *) 0xFFFED848) = 0x00000101;
	*((int *) 0xFFFED108) = 0x00000300;
	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all priorities
	*((int *) 0xFFFEC104) = 0xFFFF;
	// Set CPU Interface Control Register (ICCICR). Enable signaling of interrupts
	*((int *) 0xFFFEC100) = 1; // enable = 1
	// Configure the Distributor Control Register (ICDDCR) to send pending interrupts to CPUs
	*((int *) 0xFFFED000) = 1; // enable = 1
}

/* setup the KEY interrupts in the FPGA */
void config_KEYs()
{
	volatile int * KEY_ptr = (int *) 0xFF200050; // pushbutton KEY address
	*(KEY_ptr + 2) = 0xF; // enable interrupts for all four KEYs
}

/* Define the IRQ exception handler */
void __attribute__ ((interrupt)) __cs3_isr_irq (void)
{
	// Read the ICCIAR from the processor interface
	int int_ID = *((int *) 0xFFFEC10C);
	if(int_ID == 73) // check if interrupt is from the KEYs
		pushbutton_ISR ();
	
	// Write to the End of Interrupt Register (ICCEOIR)
	*((int *) 0xFFFEC110) = int_ID;
}
// Define the remaining exception handlers */
void __attribute__ ((interrupt)) __cs3_isr_undef (void)
{
	while (1);
}
void __attribute__ ((interrupt)) __cs3_isr_swi (void)
{
	while (1);
}
void __attribute__ ((interrupt)) __cs3_isr_pabort (void)
{
	while (1);
}
void __attribute__ ((interrupt)) __cs3_isr_dabort (void)
{
	while (1);
}
void __attribute__ ((interrupt)) __cs3_isr_fiq (void)
{
	while (1);
}



void pushbutton_ISR( void )
{
	uint32_t szMap[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
	volatile int * KEY_ptr = (int *) KEY_BASE;
	volatile int * KEY_data = (int *) 0xFF20005C;
	int press;
	press = *(KEY_ptr + 3); // read the pushbutton interrupt register
	*KEY_data = press; // clear the interrupt
	if (press & 0x1) // KEY0
		upperThreshold += 1;
	else if (press & 0x2) // KEY1
		upperThreshold -= 1;
	else if (press & 0x4) // KEY2
		lowerThreshold +=1;
	else // press & 0x8, which is KEY3
		lowerThreshold -=1;
		
	volatile int * hex0_ptr = (int *)HEX0_BASE;
	*hex0_ptr = szMap[(int)(upperThreshold) % 10]        | (szMap[(int)((upperThreshold) / 10)]) << 8 |
			   (szMap[(int)(lowerThreshold) % 10]) << 16 | (szMap[(int)((lowerThreshold) / 10)]) << 24;
			  
	changeCannySignal = 1;		  
}



