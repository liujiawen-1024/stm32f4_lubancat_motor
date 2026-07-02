#include "app.h"
#include "key.h"
#include "led.h"
#include "usart.h"
#include "encoder.h"
#include "app_motor.h"
#include "speed_control.h"
#include "pid.h"

uint8_t key_value = 0;
float speed_value = 100.0f;

/**
	*	@brief	相关初始化
	*/
void APP_ALL_Init(void)
{
	LED_Init();
	USART_Init(115200);
	ENCODER_Init();
	APP_MOTOR_Init();
	SPEED_Init();
}

/**
	*	@brief	相关启动
	*/
void APP_ALL_Start(void)
{
	ENCODER_Start();
	APP_MOTOR_Start();
}

/**
	*	@brief	根据 DMA 控制 LED
	*/
void APP_DMA_LED_Ctrl(void)
{
	if(usart_rx_len > 1)
	{
    // 判断是否为 "ON"（2个字节）
    if(usart_rx_len == 2 && usart_rx_data[0] == 'O' && usart_rx_data[1] == 'N')
    {
        LED_ALL_ON();
        USART_SendString("LED ON\r\n");
				usart_rx_len = 0;
    }
    // 判断是否为 "OFF"（3个字节）
    else if(usart_rx_len == 3 && usart_rx_data[0] == 'O' && 
            usart_rx_data[1] == 'F' && usart_rx_data[2] == 'F')
    {
        LED_ALL_OFF();
        USART_SendString("LED OFF\r\n");
				usart_rx_len = 0;
    }
    else
    {
        USART_SendString("Unknown command. Send 'ON' or 'OFF'\r\n");
				usart_rx_len = 0;
    }
	}
}

/**
	*	@brief 获取当前的编码值
	*/
int32_t APP_GetEncoder(void)
{
	return ENCODER_Count();
}

/**
	*	@brief 通过按键控制速度
	*	@retval 速度
	*/
float APP_SetSpeed(void)
{
	if(HAL_GPIO_ReadPin(KEY0_PORT,KEY0_PIN) == GPIO_PIN_SET)
	{
		HAL_Delay(150);
		if(HAL_GPIO_ReadPin(KEY0_PORT,KEY0_PIN) == GPIO_PIN_SET)
		{
			printf("KEY0,SPEED_VALUE: %.0f\r\n",speed_value);
			speed_value += 2.0f;
			if(speed_value >= 200.0f)
			{
				speed_value = 200.0f;
			}
		}
	}
	
	if(HAL_GPIO_ReadPin(KEY_UP_PORT,KEY_UP_PIN) == GPIO_PIN_SET)
	{
		HAL_Delay(200);
		if(HAL_GPIO_ReadPin(KEY_UP_PORT,KEY_UP_PIN) == GPIO_PIN_SET)
		{
			printf("KEY_UP,SPEED_VALUE: %.0f\r\n",speed_value);
			speed_value -= 2.0f;
			if(speed_value <= 0.0f)
			{
				speed_value = 0.0f;
			}
		}
	}
	
	return speed_value;
}
