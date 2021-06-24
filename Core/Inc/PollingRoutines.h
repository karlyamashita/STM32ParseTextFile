/*
 * PollingRoutines.h
 *
 *  Created on: Apr 5, 2021
 *      Author: karl.yamashita
 */

#ifndef INC_POLLINGROUTINES_H_
#define INC_POLLINGROUTINES_H_

void PollingInit(void);
void PollingRoutine(void);
void ToggleLED(void);
void UartParseRxCharBuffer(void);
void SendCAN_Message(void);
void CheckCAN_Message(void);
void ParseUartRxMessageBuffer(void);
void SendUart_Message(void);
void CheckUartState(void);

#endif /* INC_POLLINGROUTINES_H_ */
