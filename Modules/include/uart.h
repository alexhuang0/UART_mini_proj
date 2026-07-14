/*
 * uart.h
 *
 *  Created on: Jun 14, 2026
 *      Author: Alex
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "stm32f446xx.h"
#include "stm32f4xx_ll_rcc.h"

#define UART_DBG(fmt, ...)
#ifdef DEBUG
#include <stdio.h>
#define UART_DBG(fmt, ...) printf(fmt, ##__VA_ARGS__);
#endif


#define BAUD_RATE 115200
#define UART_RX_BUF_SIZE 64
#define UART_TX_BUF_SIZE 1024

extern volatile uint16_t software_char_drop_ctr;

void uart_init(void);
void UART2_SendChar(char c);
char UART2_ReadChar(void);
int UART_Has_RxBuffer_New_Data(void);
char UART_Get_Buffered_Char(void);

#endif /* UART_UART_H_ */
