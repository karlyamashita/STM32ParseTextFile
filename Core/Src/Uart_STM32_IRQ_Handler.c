/*
 * IRQ_Handlers.c
 *
 *  Created on: Jun 15, 2021
 *      Author: karl.yamashita
 */

#include "main.h"
#include "Uart_STM32_IRQ_Handler.h"


/*
 * Defines and Functions for STM32
 *
 *
 */
#if defined HAL_UART_MODULE_ENABLED
extern UART_HandleTypeDef huart1;
uint8_t uartCharBuffer[8];
UartPendingCharStruct uartPendingCharStruct;

/*
 * Description: This overrides the HAL_UART_RxCpltCallback()
 * 				Saves received uart data to character buffer.
 *
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_StatusTypeDef HAL_Status;
	uint8_t *pData = uartCharBuffer;
	if(huart->Instance == huart1.Instance){
		HAL_Status = UartAddCharToBuffer(UART_PORT_1, (char*)pData);
		if(HAL_Status == HAL_OK){
			EnableUartRecInterrupt(huart, uartCharBuffer);
		}
		else
		{
			uartPendingCharStruct.uartPendingCharFlag = 1;
			uartPendingCharStruct.huart = *huart;
			uartPendingCharStruct.uartData = *pData;
		}
	}
}

/*
 * Description: Checks for pending uart character in buffer. If HAL_UART_RxCpltCallback returns busy then this function will execute.
 * 				Call this from a polling routine.
 *
 * Input:
 * Output:
 *
 */
void UartCheckForPendingChar(void){
	HAL_StatusTypeDef HAL_Status;
	uint8_t *pData = uartCharBuffer;
	if(uartPendingCharStruct.uartPendingCharFlag == true){
		if (uartPendingCharStruct.huart.Instance == huart1.Instance) {
			HAL_Status = UartAddCharToBuffer(UART_PORT_1, (char*) pData);
			if (HAL_Status == HAL_OK) {
				uartPendingCharStruct.uartPendingCharFlag = false;
				EnableUartRecInterrupt(&uartPendingCharStruct.huart, uartCharBuffer);
			}
		}
	}
}

/*
 * Description: Enables interrupts for uart
 *
 * Input: The uart instance and pointer to data array
 * Output:
 */
void EnableUartRecInterrupt(UART_HandleTypeDef *huart, uint8_t *uartRxBuffer) {
#ifndef HAL_DMA_MODULE_ENABLED
	HAL_UART_Receive_DMA(huart, uartRxBuffer, MAX_UART_RX_CHAR_BUFFER_SINGLE); // set to receive one character at a time in DMA mode.
#else
	HAL_UART_Receive_IT(huart, uartRxBuffer, 1); // set to receive one character at a time in Interrupt mode.
#endif
}
#endif


/*
 * Description: Sends char array on uart.
 * 				This function name (UartTxMessage) should be the same across the different MCU for portability when called from UartBuffer.c file.
 *				Note: You will need to add case statements if using uart2+ ports
 * Input: Uart handle and reference to char array
 *  Output: HAL status
 *
 */
HAL_StatusTypeDef UartTxMessage(UartCharBufferTxStruct *uartBufferPointer){
	HAL_StatusTypeDef HAL_Status;
	HAL_UART_StateTypeDef HAL_State;
	UART_HandleTypeDef huart;
	uint8_t *pData = uartBufferPointer->data;

	uint8_t count = strlen((char*) uartBufferPointer->data);

	switch(uartBufferPointer->uartPort){
	case UART_PORT_1:
		huart = huart1;
		// check if Tx is busy
    	HAL_State = HAL_UART_GetState(&huart);
    	if(HAL_State == HAL_UART_STATE_BUSY_TX) return HAL_BUSY;
		break;
	}

#if defined HAL_DMA_MODULE_ENABLED
	HAL_Status = HAL_UART_Transmit_DMA(&huart, (uint8_t*) pData, count);
#else
	HAL_Status = HAL_UART_Transmit_IT(&huart, (uint8_t*) pData, count);
#endif
	return HAL_Status;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	//while(1);
}
