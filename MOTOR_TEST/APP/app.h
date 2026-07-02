#ifndef __APP_H_
#define __APP_H_

#include "main.h"

void APP_ALL_Init(void);
void APP_ALL_Start(void);
void APP_DMA_LED_Ctrl(void);
int32_t APP_GetEncoder(void);
float APP_SetSpeed(void);
void APP_MOTOR_Ctrl(uint8_t driection,uint8_t speed);
#endif /* __APP_H_ */
