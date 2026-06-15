/*
 * uart.c
 *
 *  Created on: Jun 14, 2026
 *      Author: Alex
 */

/**
 * void UART2_Init_Raw(void) {

    // 1. Enable Clocks for GPIOA and USART2

    // Hint: Use the RCC register block pointers, e.g., RCC->AHB1ENR |= ...



    // 2. Configure PA2 (TX) and PA3 (RX) Modes

    // Hint: Clear bits first, then set them to Alternate Function mode (0b10)



    // 3. Configure Alternate Function Muxing

    // Hint: Are PA2/PA3 in AFR[0] (low) or AFR[1] (high)? Each pin gets 4 bits.



    // 4. Set Baud Rate

    // Hint: Write your calculated hex value directly to USART2->BRR



    // 5. Control Registers (CR1)

    // Hint: Enable UE (USART Enable), TE (Transmitter Enable), and RE (Receiver Enable)

}



void UART2_SendChar(char c) {

    // Write this function.

    // 1. Wait until the Transmit Data Register is empty (Check TXE bit in USART2->SR)

    // 2. Write 'c' to USART2->DR

}



char UART2_ReadChar(void) {

    // Write this function.

    // 1. Wait until data is received (Check RXNE bit in USART2->SR)

    // 2. Return the value from USART2->DR

}



int main(void) {

    UART2_Init_Raw();



    while(1) {

        // Read a character, modify it (e.g., c + 1), and send it back

    }

}
 */

#include "uart.h"

void uart_init() {
	UART_DBG("Init UART\n");

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
	// usart: data sheet p824, baud rate p787
	// LL_RCC_GetAPB1Prescaler() and get SysClock for APB1 clock speed

    // Hint: Write your calculated hex value directly to USART2->BRR



    // 5. Control Registers (CR1)

    // Hint: Enable UE (USART Enable), TE (Transmitter Enable), and RE (Receiver Enable)

}


