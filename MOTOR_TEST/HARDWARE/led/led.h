#ifndef __LED_H_
#define __LED_H_

#include "main.h"

#define	LED_PORT				GPIOF
#define LED_RCC_ON()		__HAL_RCC_GPIOF_CLK_ENABLE()
#define	LED0_PIN				GPIO_PIN_9
#define LED1_PIN				GPIO_PIN_10

void LED_Init(void);

void LED_ALL_ON(void);
void LED_ALL_OFF(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED0_ON(void);
void LED0_OFF(void);

#endif /* __BSP_LED_H_ */
