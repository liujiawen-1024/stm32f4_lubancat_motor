#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include "main.h"

#define	UART_PORT									GPIOA
#define UART_USART1								USART1
#define UART_GPIO_RCC_ON()				__HAL_RCC_GPIOA_CLK_ENABLE()
#define UART_UART_RCC_ON()				__HAL_RCC_USART1_CLK_ENABLE()
#define	UART_RX_PIN								GPIO_PIN_10
#define	UART_TX_PIN								GPIO_PIN_9

extern UART_HandleTypeDef huart1;

void BSP_UART_Init(uint32_t baud);
void BSP_UART_SendString(char *str);
void BSP_UART_SendData(uint8_t *data,uint16_t len);

#endif /* _BSP_UART_H_ */
