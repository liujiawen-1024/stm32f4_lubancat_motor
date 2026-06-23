#ifndef _APP_UART_H_
#define _APP_UART_H_

#include "main.h"

void APP_UART_Init(uint32_t baud);
void APP_UART_StartReceive(void);
void APP_UART_SendData(uint8_t *data,uint16_t len);
void APP_UART_SendString(char *str);

#endif /* _APP_UART_H_ */
