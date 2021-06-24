/*
 * IRQ_Handlers.h
 *
 *  Created on: Jun 15, 2021
 *      Author: karl.yamashita
 */

#ifndef INC_UART_STM32_IRQ_HANDLER_H_
#define INC_UART_STM32_IRQ_HANDLER_H_

typedef struct{
	bool uartPendingCharFlag;
	UART_HandleTypeDef huart;
	uint8_t uartData;
}UartPendingCharStruct;

void EnableUartRecInterrupt(UART_HandleTypeDef *huart, uint8_t *uartRxBuffer);
HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *data);
void UartCheckForPendingChar(void);

#endif /* INC_UART_STM32_IRQ_HANDLER_H_ */
