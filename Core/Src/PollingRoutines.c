/*
 * PollingRoutines.c
 *
 *  Created on: Apr 5, 2021
 *      Author: karl.yamashita
 */


#include "main.h"
#include "PollingRoutines.h"

// defined in main.c
extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart1;

// defined in UartBuffer.c
extern uint8_t uartCharBuffer[8]; // only need 1 character to hold

extern uint8_t uartRxCharBuffer[MAX_UART_RX_CHAR_BUFFER];
extern RING_BUFF_INFO uartRxCharBufferPointer;

extern RING_BUFF_INFO uartRxMsgBufferPointer;
extern UartCharBufferRxStruct uartRxMessageBuffer[MAX_UART_RX_MESSAGE_BUFFER];


/*
 * Description: Initialize peripherals or whatever.
 *
 * Input:
 * Output:
 *
 */
void PollingInit(void){
	UartCharBufferTxStruct uartTx;
	char str[64] = {0};
//	SetCanFilter();

	TimerCallbackRegister(ToggleLED, 100, REPEAT_TIMER);
	TimerCallbackEnable(ToggleLED, DISABLE);
//	TimerCallbackRegister(EnableReceiveDMA, 1, REPEAT_TIMER);
//	TimerCallbackEnable(EnableReceiveDMA, NO_REPEAT_TIMER);
//	TimerCallbackRegister(SendCAN_Message, 1000, REPEAT_TIMER);
//	TimerCallbackRegister(SendUart_Message, 1000, REPEAT_TIMER);

	EnableUartRecInterrupt(&huart1, uartCharBuffer);

	strcpy(str, "Hello from Blue Pill!\r\n");
	UartCopyStrToCharBufferTxStruct(UART_PORT_1, &uartTx, str);
	UartAddTxMessageBuffer(&uartTx);
}

/*
 * Description: Main loop, called from main.c
 *
 * Input:
 * Output:
 *
 */
void PollingRoutine(void){

	TimerCallbackCheck();
	asm (" nop");

	// uart routines
	UartCheckForPendingChar();
	UartParseRxCharBuffer(); // parse character buffer
	ParseUartRxMessageBuffer();
	UartSendMessage();

	/*
	CheckCAN_Message();
	SendCanTxMessage1(&hcan);
	 */

}

/*
 * Description: Checks if any CAN message available.
 *
 * Input:
 * Output:
 *
 */
void CheckCAN_Message(void){
	static CanRxMsgTypeDef canRxMsg;
	UartCharBufferTxStruct uartTx;

	if(Can1DataAvailable(&canRxMsg)){
		if(canRxMsg.CAN_RxHeaderTypeDef.IDE == CAN_ID_STD){
			switch(canRxMsg.CAN_RxHeaderTypeDef.StdId){
			case 0x200:
			default:
				uartTx.uartPort = UART_PORT_1;
				uartTx.data[0] = 0x30 + canRxMsg.Data[0];
				uartTx.data[1] = '\r';
				uartTx.data[2] = '\n';
				UartAddTxMessageBuffer(&uartTx);
				break;
			}
		}
		else if(canRxMsg.CAN_RxHeaderTypeDef.IDE == CAN_ID_EXT){
			switch (canRxMsg.CAN_RxHeaderTypeDef.ExtId) {

			}
		}

	}
}

void ToggleLED(void){
	HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
}

void SendCAN_Message(void){
	static CanTxMsgTypeDef canTxMsg;
	static uint8_t counter = 0;

	canTxMsg.CAN_TxHeaderTypeDef.IDE = CAN_ID_STD;
	canTxMsg.CAN_TxHeaderTypeDef.StdId = 0x200;
	canTxMsg.CAN_TxHeaderTypeDef.DLC = 8;
	canTxMsg.Data[0] = counter;
	canTxMsg.Data[1] = 0x20;
	canTxMsg.Data[2] = 0x30;
	canTxMsg.Data[3] = 0x40;
	canTxMsg.Data[4] = 0x50;
	canTxMsg.Data[5] = 0x60;
	canTxMsg.Data[6] = 0x70;
	canTxMsg.Data[7] = 0x80;

	AddCanTxBuffer1(&canTxMsg);

	if(++counter > 9){
		counter = 0;
	}
}

void ParseUartRxMessageBuffer(void){
	UartCharBufferTxStruct uartTx;
	uint8_t message[MAX_UART_RX_CHAR_BUFFER] = {0};
	char str[64] = {0};
	int errorCode = 0;
	char *strRet;

	if(uartRxMsgBufferPointer.iCnt_Handle){
		memcpy(&message, &uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexOUT].data, sizeof(message));

		if(strncmp((char*)message, "CMD0", 4) == 0){
			strcpy(str, "ACK CMD0\r\n");
			UartCopyStrToCharBufferTxStruct(UART_PORT_1 , &uartTx, str);
			UartAddTxMessageBuffer(&uartTx);
			TimerCallbackEnable(SendUart_Message, DISABLE);
		}
		else if(strncmp((char*)message, "CMD1", 4) == 0){
			strcpy(str, "ACK CMD1\r\n");
			UartCopyStrToCharBufferTxStruct(UART_PORT_1 , &uartTx, str);
			UartAddTxMessageBuffer(&uartTx);
			TimerCallbackEnable(SendUart_Message, ENABLE);
		}
		else if(strncmp((char*)message, "CMD5:1000", 9) == 0){
			strcpy(str, "ACK CMD5\r\n");
			UartCopyStrToCharBufferTxStruct(UART_PORT_1, &uartTx, str);
			UartAddTxMessageBuffer(&uartTx);
/*
			if(osTimerIsRunning(timerBlinkLEDHandle)){
				osTimerStop(timerBlinkLEDHandle);
			}
			osTimerStart(timerBlinkLEDHandle, 1000);
*/
		} else if (strncmp((char*) message, "CMD5:2000", 9) == 0) {
			strcpy(str, "ACK CMD5\r\n");
			UartCopyStrToCharBufferTxStruct(UART_PORT_1, &uartTx, str);
			UartAddTxMessageBuffer(&uartTx);
/*
			if(osTimerIsRunning(timerBlinkLEDHandle)){
				osTimerStop(timerBlinkLEDHandle);
			}
			osTimerStart(timerBlinkLEDHandle, 2000);
	*/
		}
		/*
		else if (strncmp((char*) message, "Hello World", 11) == 0) {
			strcpy(str, "Hello Back from STM32!\r\n");
			UartCopyStrToCharBufferTxStruct(UART_PORT_1, &uartTx, str);
			UartAddTxMessageBuffer(&uartTx);
		}
		*/
		else
		{
			strRet = strstr((char*) message, "PLLR1");
			if(strncmp(strRet, "PLLR1", 5) == 0){
				strcpy(str, "******** Found PLLR1 in text! ********\r\n");
				UartCopyStrToCharBufferTxStruct(UART_PORT_1, &uartTx, str);
				UartAddTxMessageBuffer(&uartTx);
				// blink LED when PLLR1 is found
				TimerCallbackEnable(ToggleLED, ENABLE);
			}
			else
			{
				errorCode = UartCopyStrToCharBufferTxStruct(UART_PORT_1, &uartTx, (char*) message);
				if (errorCode != 0) {
					return;
				}
				UartAddTxMessageBuffer(&uartTx);
			}
		}
		memset(&uartRxMessageBuffer[uartRxMsgBufferPointer.iIndexOUT].data, 0, MAX_UART_RX_CHAR_BUFFER); // clear data
		DRV_RingBuffPtr__Output(&uartRxMsgBufferPointer, MAX_UART_RX_MESSAGE_BUFFER);
	}
}

void SendUart_Message(void){
	UartCharBufferTxStruct uartTx;
	char str[32] = {0};
	int errorStatus = 0;
	strcpy(str, "Uart message from TIM4\r\n");
	errorStatus = UartCopyStrToCharBufferTxStruct(UART_PORT_1, &uartTx, str);
	if(errorStatus != 0){
		return;
	}
	UartAddTxMessageBuffer(&uartTx);
}


