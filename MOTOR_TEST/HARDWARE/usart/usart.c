#include "usart.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

// 环形接收缓冲区
uint8_t usart_rx_dma_buffer[USART_DMA_BUFFER_SIZE] = {0};
uint8_t usart_rx_data[USART_DMA_BUFFER_SIZE] = {0};
uint16_t usart_rx_len = 0;

/**
	*	@brief 	串口初始化
	*	@param	baud：波特率	
	*/
void USART_Init(uint32_t baud)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = baud;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
	
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    USART_UART_RCC_ON();
		USART_GPIO_RCC_ON();
		USART_DMA_RCC_ON();
		
		// GPIO配置
    GPIO_InitStruct.Pin = USART_RX_PIN|USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(USART_PORT, &GPIO_InitStruct);
  
		// DMA TX配置（内存 -> 外设）
		hdma_usart1_tx.Instance = USART1_TX_DMA_STREAM;
		hdma_usart1_tx.Init.Channel = USART1_TX_DMA_CHANNEL;
		hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;						// 外设地址不递增
		hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;								// 内存地址递增
		hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_tx.Init.Mode = DMA_NORMAL;
		hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
		hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;				// 不用FIFO，简化配置
		
		HAL_DMA_Init(&hdma_usart1_tx);
		
		// 将DMA句柄与UART句柄关联（HAL库自动管理）
		__HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);
		
		// DMA RX配置（外设 -> 内存）
		hdma_usart1_rx.Instance = USART1_RX_DMA_STREAM;
		hdma_usart1_rx.Init.Channel = USART1_RX_DMA_CHANNEL;
		hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;						// 外设地址不递增
		hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;								// 内存地址递增
		hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;										// 循环模式（配合空闲中断）
		hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;				// 不用FIFO，简化配置
		
		HAL_DMA_Init(&hdma_usart1_rx);
		__HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);
		
		// NVIC配置
		// DMA TX 中断
		HAL_NVIC_SetPriority(DMA2_Stream7_IRQn,DMA_TX_IRQ_PRIO,0);
		HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
		
		// DMA RX 中断
		HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,DMA_RX_IRQ_PRIO,0);
		HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
		
		// 串口中断（启用空闲中断需要）
		HAL_NVIC_SetPriority(USART1_IRQn,0,0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		
		// 使能串口空闲中断
		__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    USART_UART_RCC_ON();
		
    HAL_GPIO_DeInit(USART_PORT, USART_RX_PIN|USART_TX_PIN);
		
		HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
}

/**
 * @brief 重定向 printf
 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void USART_SendString(char *str)
{
		HAL_UART_Transmit(&huart1,(uint8_t *)str,strlen(str),100);
}

void USART_SendData(uint8_t *data,uint16_t len)
{
		HAL_UART_Transmit(&huart1,data,len,100);
}

/**
	*	@brief	使用 DMA 发送数据（非阻塞）
	*/
void USART_DMA_Transmit(uint8_t *data,uint16_t len)
{
	HAL_UART_Transmit_DMA(&huart1,data,len);
}

/**
	*	@brief	启用 DMA 接收（循环模式 + 空闲中断）
	*/
void USART_DMA_Receive_IT(void)
{
	// 先清空缓冲区
	memset(usart_rx_dma_buffer,0,USART_DMA_BUFFER_SIZE);
	usart_rx_len = 0;
	
	// 启用 DMA 接收（循环模式，一直往里填数据）
	HAL_UART_Receive_DMA(&huart1,usart_rx_dma_buffer,USART_DMA_BUFFER_SIZE);
}

/**
	*	@brief	串口空闲中断
	*/
void USART_IDLE_Handler(void)
{
	// 检查是否是空闲中断
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET))
	{
		// 清除标志位
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		
		// 停止DMA，获取已接收长度
		
		HAL_UART_DMAStop(&huart1);
		// 计算接收到的数据长度：总缓冲区大小 - 剩余未传输的数量
		usart_rx_len = USART_DMA_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		
		// 复制数据到用户缓冲区（防止下次 DMA 覆盖）
		if(usart_rx_len > 0 && usart_rx_len < USART_DMA_BUFFER_SIZE)
		{
			memcpy(usart_rx_data,usart_rx_dma_buffer,usart_rx_len);
			// 字符串结束符
			usart_rx_data[usart_rx_len] = '\0';
		}
		
		// 重启 DMA 接收（为下一帧做准备）
		HAL_UART_Receive_DMA(&huart1,usart_rx_dma_buffer,USART_DMA_BUFFER_SIZE);
	}
}
