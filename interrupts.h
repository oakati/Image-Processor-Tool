/*
 * interrupts.h
 *
 *  Created on: Jan 18, 2022
 *      Author: EmreDemir
 */
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#define LED_BASE	0xFF200000
#define KEY_BASE 	0xFF200050
#define HEX0_BASE 	0xFF200020
#define HEX4_BASE   0xFF200030

void set_A9_IRQ_stack (void);
void config_GIC (void);
void config_KEYs (void);
void enable_A9_interrupts (void);


void pushbutton_ISR (void);

extern volatile int upperThreshold;
extern volatile int lowerThreshold;
extern uint8_t changeCannySignal;


#endif //INTERRUPTS_H