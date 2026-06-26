/*
 * tmr.c
 *
 *  Created on: Jun 26, 2026
 *      Author: Alex
 */

#include "stm32f4xx.h"

void tmr_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;


}
