/*
 * UartIncludes.h
 *
 *  Created on: Jun 15, 2021
 *      Author: karl.yamashita
 */

#ifndef INC_UARTINCLUDES_H_
#define INC_UARTINCLUDES_H_


#define MAX_UART_RX_CHAR_BUFFER_SINGLE 1 // this is for the uart IRQ array reference. For STM32 we are receiving one character at a time.
#define MAX_UART_RX_CHAR_BUFFER 90
#define MAX_UART_TX_CHAR_BUFFER 90
#define MAX_UART_RX_MESSAGE_BUFFER 4
#define MAX_UART_TX_MESSAGE_BUFFER 4


// for STM32
#if defined HAL_UART_MODULE_ENABLED
extern UART_HandleTypeDef huart1;
#endif



#endif /* INC_UARTINCLUDES_H_ */
