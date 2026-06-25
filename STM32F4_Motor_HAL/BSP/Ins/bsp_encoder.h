#ifndef _BSP_ENCODER_H_
#define _BSP_ENCODER_H_

#include "main.h"

#define ENCODER_RCC_ON()							__HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_RCC_OFF()							__HAL_RCC_GPIOA_CLK_DISABLE()

#define	ENCODER_TIM3_RCC_ON()				  __HAL_RCC_TIM3_CLK_ENABLE()
#define	ENCODER_TIM3_RCC_OFF()				__HAL_RCC_TIM3_CLK_DISABLE()

#define ENCODER_PORT									GPIOA
#define ENCODER_A											GPIO_PIN_6
#define ENCODER_B											GPIO_PIN_7

#define ENCODER_LINES           			13      											// ±àÂëÆ÷ÏßÊý
#define ENCODER_PULSES_PER_REV  			(ENCODER_LINES * 4)  					// 52

void BSP_ENCODER_Init(void);
void BSP_ENCODER_Start(void);
int32_t BSP_ENCODER_Count(void);
void BSP_ENCODER_Clear(void);

#endif /* _BSP_ENCODER_H_ */
