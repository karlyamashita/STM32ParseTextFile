# STM32ParseTextFile

Development board: STM32 Blue Pill.
Uart1: (115200, None, 8, 1)

Description: This will read uart string and save each character in a Character buffer from the HAL_UART_RxCpltCallback() which is set to interrupt on each character received.
              From a polling routine the code will parse the Character buffer and start saving the characters in a Message buffer while looking for '\n' character. Once the '\n' is received, 
              the Message buffer pointer will increment to indicate a new message is ready to be parsed.
              Also from the Polling routine, the code will parse the Message buffer. 
              For this project the code is looking for the string "PLLR1" in the text file. It will return a message indicating it found "PLLR1" in the text file and start blinking the user LED.
              
              
              
              

