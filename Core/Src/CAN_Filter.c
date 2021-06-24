
#include "main.h"
#include "CAN_Filter.h"

// this file needs to include CAN_Buffer.h to save Rx messages to ring buffer
#include "CAN_Buffer.h"

CanRxMsgTypeDef	RxMessage1;
CanRxMsgTypeDef	RxMessage2;

#if defined STM32F042x6 || defined STM32F1 // STM32F042x6 only has one CAN controller defined as hcan not hcan1
extern CAN_HandleTypeDef hcan;
#else
extern CAN_HandleTypeDef hcan1;
#endif


#ifdef USE_CAN_BUFFER_2
static CanRxMsgTypeDef	RxMessage2;
extern CAN_HandleTypeDef hcan2;
#endif // USE_CAN_BUFFER_2



/*
 * Note: STM32F4xx needs HAL_CAN_ActivateNotification() to activate interrupts.
 */

/*
function: Set CAN filter to pass all ID's
input: none
output: none
*/
void SetCanFilter(void) {
	static CAN_FilterTypeDef sFilterConfig;

	//sFilterConfig.FilterNumber           = 0;
	sFilterConfig.FilterMode             = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale            = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh           = 0x0000;
	sFilterConfig.FilterIdLow            = 0x0000;
	sFilterConfig.FilterMaskIdHigh       = 0x0000;
	sFilterConfig.FilterMaskIdLow        = 0x0000;
	sFilterConfig.FilterFIFOAssignment   = 0;
	sFilterConfig.FilterActivation       = ENABLE;
#if defined STM32F042x6 || defined STM32F1 // this device defines the single CAN controller as hcan, not hcan1
	HAL_CAN_ConfigFilter(&hcan,&sFilterConfig);
	HAL_CAN_Start(&hcan);
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) // enables CAN notification. This is not the same as enabling CAN reception.
	{
		/* Notification Error */
		Error_Handler();
	}
#else
	HAL_CAN_ConfigFilter(&hcan1,&sFilterConfig);
	HAL_CAN_Start(&hcan1);
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) // enables CAN notification. This is not the same as enabling CAN reception.
	{
		/* Notification Error */
		Error_Handler();
	}
#endif
	// init same filter for hcan2
#ifdef USE_CAN_BUFFER_2
	HAL_CAN_ConfigFilter(&hcan2,&sFilterConfig);
	HAL_CAN_Start(&hcan2);
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}
#endif // CAN2
}


/*
function: Callback when message is available. Devices with 1 CAN controller is defined as CAN instead of CAN1.
input: CanHandle
output: none
note: Use CallbackTimer to clear sleep timer for "GotoSleepCallbackFunc()"
*/

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
#ifdef USE_MTIMER_CALLBACK
	// you should have already defined a callback using "GotoSleep" variable or create your own variable and replace in function in next line.
	ClearTimerCallbackTimer(GotoSleep);
#endif// USE_MTIMER_CALLBACK

#if defined STM32F042x6
	if(hcan->Instance == CAN) {
#else // STM32F1 = CAN1
	if(hcan->Instance == CAN1) {
#endif//
		HAL_CAN_GetRxMessage(hcan, (uint32_t) CAN_RX_FIFO0, &RxMessage1.CAN_RxHeaderTypeDef, RxMessage1.Data); // enable CAN reception again
		AddCanRxBuffer1(&RxMessage1);
	}
	
#ifdef USE_CAN_BUFFER_2
	else if(hcan->Instance == CAN2) {
		HAL_CAN_GetRxMessage(hcan, (uint32_t) CAN_RX_FIFO0, &RxMessage2.CAN_RxHeaderTypeDef, RxMessage2.Data); // enable CAN reception again
		AddCanRxBuffer2(&RxMessage2);
	}
#endif
#ifdef USE_CAN_BUS_ACTIVITY_STATUS
#include "CanBusActivity.h"
	CanBusActivityStatus(1); // include header file where you placed this function
#endif// USE_CAN_BUS_ACTIVITY_STATUS
}


void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
#ifdef USE_MTIMER_CALLBACK
	// you should have already defined a callback using "GotoSleep" variable or create your own variable and replace in function in next line.
	ClearTimerCallbackTimer(GotoSleep);
#endif// USE_MTIMER_CALLBACK

#if defined STM32F042x6
	if(hcan->Instance == CAN) {
#else // STM32F1 = CAN1
	if(hcan->Instance == CAN1) {
#endif//
		HAL_CAN_GetRxMessage(hcan, (uint32_t) CAN_RX_FIFO0, &RxMessage2.CAN_RxHeaderTypeDef, RxMessage2.Data); // enable CAN reception again
		AddCanRxBuffer1(&RxMessage2);
	}

#ifdef USE_CAN_BUFFER_2
	else if(hcan->Instance == CAN2) {
		HAL_CAN_GetRxMessage(hcan, (uint32_t) CAN_RX_FIFO0, &RxMessage2.CAN_RxHeaderTypeDef, RxMessage2.Data); // enable CAN reception again
		AddCanRxBuffer2(&RxMessage2);
	}
#endif
#ifdef USE_CAN_BUS_ACTIVITY_STATUS
#include "CanBusActivity.h"
	CanBusActivityStatus(1); // include header file where you placed this function
#endif// USE_CAN_BUS_ACTIVITY_STATUS
}


// We should never need this callback as the CAN buffer should take care of messages received.
// void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)


