/*
 * led.c
 *
 *  Created on: Jun 22, 2026
 *      Author: Alex
 */

#include "led.h";
#include "stm32f4xx_ll_rcc.h"

void led_init(void) {
	// enable GPIO clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// set PA5 to General purpose output mode
	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	GPIOA->MODER |= (1 << GPIO_MODER_MODE5_Pos);
}

void LED_Toggle(void) {
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
