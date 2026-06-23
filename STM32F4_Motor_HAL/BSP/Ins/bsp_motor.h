#ifndef _BSP_MOTOR_H_
#define _BSP_MOTOR_H_

#include "main.h"

// 电机控制引脚映射

// PWM
#define MOTOR_PWM_TIM							TIM1
#define MOTOR_PWM_CH							TIM_CHANNEL_1
#define MOTOR_PWM_GPIO_PORT				GPIOE
#define MOTOR_PWMA_GPIO_PIN				GPIO_PIN_9
#define MOTOR_PWM_GPIO_AF   			GPIO_AF1_TIM1					// PE9 复用 TIM1 CH1

// DIR
#define MOTOR_AIN1_GPIO_PIN				GPIO_PIN_14
#define MOTOR_AIN2_GPIO_PIN				GPIO_PIN_13

// ENABLE
#define MOTOR_STBY_GPIO_PIN				GPIO_PIN_8

#define MOTOR_GPIO_CLK_EN()				__HAL_RCC_GPIOE_CLK_ENABLE()

void BSP_MOTOR_Init(void);

#endif	/*_BSP_MOTOR_H_*/
