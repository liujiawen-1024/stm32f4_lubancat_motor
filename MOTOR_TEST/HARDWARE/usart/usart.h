#ifndef __USART_H_
#define __USART_H_

#include "main.h"

#define	USART_PORT									GPIOA
#define USART_USART1								USART1
#define USART_GPIO_RCC_ON()					__HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_UART_RCC_ON()					__HAL_RCC_USART1_CLK_ENABLE()
#define	USART_RX_PIN								GPIO_PIN_10
#define	USART_TX_PIN								GPIO_PIN_9
#define USART_DMA_RCC_ON()					__HAL_RCC_DMA2_CLK_ENABLE()

// DMA流和通道定义
#define USART1_TX_DMA_STREAM 				DMA2_Stream7
#define USART1_RX_DMA_STREAM 				DMA2_Stream5

#define USART1_TX_DMA_CHANNEL				DMA_CHANNEL_4
#define USART1_RX_DMA_CHANNEL				DMA_CHANNEL_4

#define DMA_TX_IRQ_PRIO							1
#define DMA_RX_IRQ_PRIO							1

// 缓冲区大小
#define USART_DMA_BUFFER_SIZE				256

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;

// 环形接收缓冲区（用于DMA+空闲中断）
extern uint8_t usart_rx_dma_buffer[USART_DMA_BUFFER_SIZE];
extern uint8_t usart_rx_data[USART_DMA_BUFFER_SIZE];
extern uint16_t usart_rx_len;

void USART_Init(uint32_t baud);
void USART_SendString(char *str);
void USART_SendData(uint8_t *data,uint16_t len);

void USART_DMA_Transmit(uint8_t *data,uint16_t len);
void USART_DMA_Receive_IT(void);
void USART_IDLE_Handler(void);
#endif /* __USART_H_ */
