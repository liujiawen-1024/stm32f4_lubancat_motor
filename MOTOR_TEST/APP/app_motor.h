#ifndef __APP_MOTOR_H_
#define __APP_MOTOR_H_

#include "main.h"

void APP_MOTOR_Init(void);
void APP_MOTOR_Ctrl(uint8_t driection,uint8_t speed);
void APP_MOTOR_Stop(void);
void APP_MOTOR_Start(void);
#endif /* __APP_MOTOR_H_ */
