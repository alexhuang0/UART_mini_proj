/*
 * uart.h
 *
 *  Created on: Jun 14, 2026
 *      Author: Alex
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "stm32f446xx.h"

#ifdef DEBUG
#include <stdio.h>
#define UART_DBG(fmt, ...) printf(fmt, ##__VA_ARGS__);
#endif

void uart_init();


#endif /* UART_UART_H_ */
