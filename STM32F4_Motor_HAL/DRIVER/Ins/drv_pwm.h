#ifndef _DRV_PWM_H_
#define _DRV_PWM_H_

#include "main.h"

#define DRV_PWM_RCC_ON()			__HAL_RCC_TIM1_CLK_ENABLE()

void DRV_PWM_Init(void);
void DRV_PWM_SetDuty(uint16_t duty);
void DRV_PWM_Start(void);
void DRV_PWM_Stop(void);

#endif /*_DRV_PWM_H_*/
