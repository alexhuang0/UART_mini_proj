/*
 * tmr.c
 *
 *  Created on: Jun 26, 2026
 *      Author: Alex
 */

#include "stm32f4xx.h"
#include "led.h"
#include "tmr.h"

void setup_gpio_pin(void);

void tmr_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// APB1 Timer Clock Freq / Desired Tim Freq = Prescaler * ARR
	// for 0.5s period (2Hz)
	TIM2->PSC = 4200 - 1;
	TIM2->ARR = 10000 - 1; // datasheet says this is 32 bits

	// clear pending flags if any
	TIM2->SR &= ~TIM_SR_UIF;

	// enable Update interrupt
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM2_IRQn, 1);
	NVIC_EnableIRQ(TIM2_IRQn);

	// better to enable timer after setting parameters
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void) {
	// make sure interrupt was caused by Update event
	if (TIM2->SR & TIM_SR_UIF) {
		// clear update flag
		TIM2->SR &= ~TIM_SR_UIF;
		LED_Toggle();
	}
}

void tmr_init_PWM(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// APB1 Timer Clock Freq / Desired Tim Freq = Prescaler * ARR
	// for 0.5s period (2Hz)
	TIM2->PSC = 4200 - 1;
	TIM2->ARR = 2*(10000) - 1; // datasheet says this is 32 bits
	// double ARR to get CCRx register to have DUTY CYCLE of 0.5s


	// PWM STUFF (enabled bits based on ref manual)
	// Duty Cycle = CCR4 / ARR
	TIM2->CCR3 = 10000 - 1;

	TIM2->CCER |= TIM_CCER_CC3E;

	TIM2->CR1 |= TIM_CR1_ARPE;

	TIM2->CCMR2 &= ~TIM_CCMR2_OC3M_Msk;
	TIM2->CCMR2 |= 0b110 << TIM_CCMR2_OC3M_Pos;
	TIM2->CCMR2 |= TIM_CCMR2_OC3PE;

	// due to PWM having shadow register / buffering, enable Update Generation bit
	TIM2->EGR |= TIM_EGR_UG;
	// better to enable timer after setting parameters
	TIM2->CR1 |= TIM_CR1_CEN;

	setup_gpio_pin();
}

void setup_gpio_pin(void) {
	// use TIM2_CH4 (PB2 from datasheet)

	// enable GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// set PA3 to AF mode
	GPIOB->MODER &= ~GPIO_MODER_MODE10;
	GPIOB->MODER |= 2 << GPIO_MODER_MODE10_Pos;

	GPIOB->AFR[1] &= ~GPIO_AFRH_AFRH2;
	// set PA3's AF mode to AF1 (TIM2_CH4)
	GPIOB->AFR[1] |= GPIO_AFRH_AFRH2_0;
}
