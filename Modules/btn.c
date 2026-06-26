/*
 * btn.c
 *
 *  Created on: Jun 24, 2026
 *      Author: Alex
 */

#include "stm32f4xx_ll_rcc.h"
#include "btn.h"
#include "led.h"

void btn_init(void) {
	// enable GPIOC clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	// set GPIOC MODER to Input
	GPIOC->MODER &= ~GPIO_MODER_MODE13;

	// INTERRUPT
	// enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Route Pin 13 to be Port C
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// remove interrupt mask
	EXTI->IMR |= EXTI_IMR_IM13;
	// active Falling and Rising trigger selection register (detects H -> L, L -> H)
	EXTI->FTSR |= EXTI_FTSR_TR13;
	EXTI->RTSR |= EXTI_RTSR_TR13;

	// clear ghost H -> L switch
	EXTI->PR = 1 << EXTI_PR_PR13_Pos;

	NVIC_SetPriority(EXTI15_10_IRQn, 5);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void) {
	EXTI->PR = 1 << EXTI_PR_PR13_Pos;

	if (GPIOC->IDR == 0) {
		GPIOA->BSRR = GPIO_ODR_OD5;
	} else {
		GPIOA->ODR = ~GPIO_ODR_OD5;
	}
//	LED_Toggle();
}
