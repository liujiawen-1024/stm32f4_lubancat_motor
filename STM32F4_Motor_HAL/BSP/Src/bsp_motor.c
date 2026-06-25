#include "bsp_motor.h"

/**
	*	@brief		电机初始化
	*/
void BSP_MOTOR_Init(void)
{
	GPIO_InitTypeDef gpio = {0};
	
	// 使能	GPIOE	时钟
	MOTOR_GPIO_CLK_EN();
	
	// 配置PWM输出引脚	PE9 -- AF1 TIM1-CH1
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Pin = MOTOR_PWMA_GPIO_PIN;
	gpio.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(MOTOR_PWM_GPIO_PORT,&gpio);
	
	// 配置方向控制引脚	PE14,PE13 -- 推挽输出
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	gpio.Alternate = 0;
	
	gpio.Pin = MOTOR_AIN1_GPIO_PIN;
	HAL_GPIO_Init(MOTOR_PWM_GPIO_PORT,&gpio);
	gpio.Pin = MOTOR_AIN2_GPIO_PIN;
	HAL_GPIO_Init(MOTOR_PWM_GPIO_PORT,&gpio);
	
	// 配置使能引脚	PE8 -- 推挽输出
	gpio.Pin = MOTOR_STBY_GPIO_PIN;
	HAL_GPIO_Init(MOTOR_PWM_GPIO_PORT,&gpio);
	
	// 设置默认安全状态	--	低电平全部关闭
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_STBY_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN2_GPIO_PIN,GPIO_PIN_RESET);
}

