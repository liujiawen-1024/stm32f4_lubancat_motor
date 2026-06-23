#include "app_uart.h"
#include "bsp_uart.h"
#include "gpio_config.h"

static uint8_t rx_buffer[UART_RX_BUFFER_SIZE];

void APP_UART_Init(uint32_t baud)
{
	BSP_UART_Init(baud);
}

// 启动中断接收函数
void APP_UART_StartReceive(void)
{
	// 清空缓冲区
		memset(rx_buffer,0,UART_RX_BUFFER_SIZE);
	
	HAL_UART_Receive_IT(&huart1,rx_buffer,UART_RX_BUFFER_SIZE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef	*huart)
{
	if(huart -> Instance == USART1)
	{
		// 要做的事情
		BSP_UART_SendString("Receive:");
		BSP_UART_SendData(rx_buffer,UART_RX_BUFFER_SIZE);
		BSP_UART_SendString("\r\n");
		//重启中断接收函数
		APP_UART_StartReceive();
	}
}

void APP_UART_SendData(uint8_t *data,uint16_t len)
{
		BSP_UART_SendData(data,len);
}

void APP_UART_SendString(char *str)
{
		BSP_UART_SendString(str);
}
