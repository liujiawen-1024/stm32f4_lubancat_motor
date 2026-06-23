#include "bsp_uart.h"

UART_HandleTypeDef huart1;

/**
	*	@brief 	串口初始化
	*	@param	baud：波特率	
	*/
void BSP_UART_Init(uint32_t baud)
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
    UART_UART_RCC_ON();
		UART_GPIO_RCC_ON();
		
    GPIO_InitStruct.Pin = UART_RX_PIN|UART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(UART_PORT, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    UART_UART_RCC_ON();
		
    HAL_GPIO_DeInit(UART_PORT, UART_RX_PIN|UART_TX_PIN);
		
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

void BSP_UART_SendString(char *str)
{
		HAL_UART_Transmit(&huart1,(uint8_t *)str,strlen(str),100);
}

void BSP_UART_SendData(uint8_t *data,uint16_t len)
{
		HAL_UART_Transmit(&huart1,data,len,100);
}
