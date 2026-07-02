#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "main.h"

extern TIM_HandleTypeDef htim1;

// PWM
#define PWM_PORT									GPIOE
#define PWM_PIN										GPIO_PIN_9
#define PWM_GPIO_RCC_ON()					__HAL_RCC_GPIOE_CLK_ENABLE()
#define PWM_GPIO_RCC_OFF()				__HAL_RCC_GPIOE_CLK_DISABLE()
#define PWM_PWM_RCC_ON()					__HAL_RCC_TIM1_CLK_ENABLE()
#define PWM_PWM_RCC_OFF()					__HAL_RCC_TIM1_CLK_DISABLE()

// DIR
#define MOTOR_AIN_PORT						GPIOE
#define MOTOR_AIN1_GPIO_PIN				GPIO_PIN_14
#define MOTOR_AIN2_GPIO_PIN				GPIO_PIN_13

// ENABLE
#define MOTOR_STBY_PORT						GPIOE
#define MOTOR_STBY_GPIO_PIN				GPIO_PIN_8

#define MOTOR_GPIO_CLK_EN()				__HAL_RCC_GPIOE_CLK_ENABLE()

void PWM_Init(void);
void PWM_Start(void);
void PWM_Stop(void);
void PWM_SetDuty(uint16_t duty);
void PWM_SetSeppd(uint8_t speed);

void MOTOR_Init(void);
void MOTOR_Start(void);
void MOTOR_Stop(void);
void MOTOR_Forward(void);
void MOTOR_Reversal(void);

#endif /* __MOTOR_H_ */
