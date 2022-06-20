/*
 * gpio.h
 *
 *  Created on: Jan 13, 2022
 *      Author: EmreDemir
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define LED_BASE 		0xFF200000
#define HEX3_HEX0_BASE 	0xFF200020
#define HEX5_HEX4_BASE 	0xFF200030
#define SSWITCH_BASE 	0xFF200040
#define KEY_BASE 		0xFF200050
#define JP1_BASE 		0xFF200060
#define JP2_BASE 		0xFF200070



/*
 *  Parallel Port GPIO Registers
 *
 */
typedef struct
{
	volatile uint32_t gpio_data;
	volatile uint32_t gpio_direction;
	volatile uint32_t gpio_interrupt_mask;
	volatile uint32_t gpio_edge_capture;
}gpio_t;



#endif /* GPIO_H_ */
