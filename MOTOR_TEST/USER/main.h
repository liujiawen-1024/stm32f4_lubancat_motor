#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

#include "usart.h"
#include "key.h"
#include "led.h"

void SystemClock_Config(void);
void Error_Handler(void);

#endif /* __MAIN_H */
