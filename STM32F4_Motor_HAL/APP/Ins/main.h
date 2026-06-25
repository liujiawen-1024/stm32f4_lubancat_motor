#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "gpio_config.h"

#include "app_motor.h"
#include "app_uart.h"
#include "app_encoder.h"

#include "stdio.h"
#include "string.h"

void SystemClock_Config(void);
void Error_Handler(void);
#endif /* __MAIN_H */

