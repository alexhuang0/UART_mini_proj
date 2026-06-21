/*
 * uart.c
 *
 *  Created on: Jun 14, 2026
 *      Author: Alex
 */

#include "uart.h"
#include <stdio.h>

// baud stuff
#include "stm32f4xx_ll_rcc.h"

void uart_init() {
//	UART_DBG("Init UART\n");
	// 1. enable GPIO and UART clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN_Msk;

    // 2. set PA2 and PA3 register modes to Alternate Function
	// 2.1 first reset the MODER bits to 0
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);

	// 2.2 then set them to AF mode (2 (10 in bits))
	GPIOA->MODER |= ((2 << GPIO_MODER_MODER2_Pos) | (2 << GPIO_MODER_MODER3_Pos));

	// 3. set Alternate function number to PA2 and PA3 (AF7 in this case)
	// clear AFR[0] (alternate function lower) at position AFSEL2 (PA2) and AFSEL3 (PA3)
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_Msk | GPIO_AFRL_AFSEL3_Msk);
	// set PA2 and PA3 to "7", aka AF7 is mapped to PA2 (AFSEL2_Pos), idem PA3
	GPIOA->AFR[0] |= ((7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos));


    // 4. Set Baud Rate
	// usart: ref manual p824, baud rate p787

	// populate rcc_clocks
	LL_RCC_ClocksTypeDef rcc_clocks;
	LL_RCC_GetSystemClocksFreq(&rcc_clocks);

	uint32_t apb1_frequency = rcc_clocks.PCLK1_Frequency;

	// over sampling multiplier
	uint32_t OVER8 = (USART2->CR1 & USART_CR1_OVER8_Msk) ? 1 : 0;
	float osr_multiplier = 8.0f * (2.0f - (float)OVER8);

	float usartdiv = apb1_frequency / (BAUD_RATE * osr_multiplier);
	uint32_t div_mantissa = (uint32_t) usartdiv;
	uint32_t div_fraction = (uint32_t) ((usartdiv - div_mantissa) * osr_multiplier + 0.5f);

	// edge case: rounding fraction gives extra carryover
	if (div_fraction >= osr_multiplier) {
		div_mantissa += 1;
		div_fraction = 0;
	}

	USART2->BRR &= ~(USART_BRR_DIV_Mantissa_Msk | USART_BRR_DIV_Fraction_Msk);
	USART2->BRR |= ((div_mantissa << USART_BRR_DIV_Mantissa_Pos) | (div_fraction << USART_BRR_DIV_Fraction_Pos));


    // 5. Control Registers (CR1)

	USART2->CR1 &= ~(USART_CR1_UE_Msk | USART_CR1_TE_Msk | USART_CR1_RE_Msk);
	USART2->CR1 |= 1 << USART_CR1_UE_Pos;
	USART2->CR1 |= ((1 << USART_CR1_TE_Pos) | (1 << USART_CR1_RE_Pos));
}

// CPU -> Transmit DR -> Transmit Shift Register -> TX Wire (to port)
void UART2_SendChar(char c) {
	// hang until previous char bits are fully transfered from Data Register (DR) to Transmit Shift Register
	// while TDR is full, aka TDR still sending bits to Transmit Shift Register
	while(!(USART2->SR & USART_SR_TXE_Msk));

	USART2->DR = c;
}

// RX Wire (from port) -> Receive Shift Register (cpu can't see yet) -> Receive DR (RXNE = 1) -> CPU software variable
char UART2_ReadChar() {
	// ORE:
	// RDR is never read, or new char is sent into RShift Register WHILE RDR isn't fully cleared yet (RXNE still 1)
	// the new char (and any new char) are lost, the RDR char is kept
	if (USART2->SR & USART_SR_ORE_Msk) {
		// seq to clear Overrun Error
		(void)USART2->SR;
		(void)USART2->DR;
//		printf("\nData lost due to ORE\n");
		return EOF;
	}

	// hang until received bits are fully transfered from Receive Shift Register to DR
	// while RDR is empty,
	while(!(USART2->SR & USART_SR_RXNE_Msk));

	return USART2->DR;
}

// overwrite weak functions required for printf
int __io_putchar(int ch) {
	UART2_SendChar((char)ch);
	return ch;
}

int __io_getchar(void) {
	uint32_t res = (int)UART2_ReadChar();
	if (res == EOF) {
		return ' ';
	}

	return res;
}
