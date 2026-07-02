#ifndef __KEY_H_
#define __KEY_H_

#include "main.h"

#define	KEY0_PORT					GPIOE
#define KEY_UP_PORT				GPIOA
#define KEY0_PIN					GPIO_PIN_4
#define KEY_UP_PIN				GPIO_PIN_0

#define KEY0_RCC_ON()			__HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY_UP_RCC_ON()		__HAL_RCC_GPIOA_CLK_ENABLE()

void KEY_Init(void);

#endif /* __KEY_H_ */
