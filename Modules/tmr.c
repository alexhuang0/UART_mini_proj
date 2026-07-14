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
