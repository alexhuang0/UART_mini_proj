/*
 * uart.h
 *
 *  Created on: Jun 14, 2026
 *      Author: Alex
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "stm32f446xx.h"

#define UART_DBG(fmt, ...)
#ifdef DEBUG
#include <stdio.h>
#define UART_DBG(fmt, ...) printf(fmt, ##__VA_ARGS__);
#endif


#define BAUD_RATE 115200

void uart_init();
void UART2_SendChar(char c);
char UART2_ReadChar();


#endif /* UART_UART_H_ */
