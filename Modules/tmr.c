/*
 * tmr.c
 *
 *  Created on: Jun 26, 2026
 *      Author: Alex
 */

#include "stm32f4xx.h"

void tmr_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// APB1 Timer Clock Freq / Desired Tim Freq = Prescaler * ARR
	// for 0.5s period (2Hz)
	TIM2->PSC = 4200 - 1;
	TIM2->ARR = 10000 - 1; // datasheet says this is 32 bits

	// clear pending flags if any
	TIM2->SR &= ~TIM_SR_UIF;

	// better to enable timer after setting parameters
	TIM2->CR1 |= TIM_CR1_CEN;


}

void setup_interrupt(void) {
	// use TIM2_CH4 (PA3 from datasheet)

	// enable GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// set PA3 to AF mode
	GPIOA->MODER &= ~GPIO_MODER_MODE3;
	GPIOA->MODER |= 2 << GPIO_MODER_MODE3_Pos;

	GPIOA->AFR &= ~GPIO_AFR_AFRL3;
	// set PA3's AF mode to AF1 (TIM2_CH4)
	GPIOA->AFR |= 1 << GPIO_AFR_AFRL3_Pos;


}
