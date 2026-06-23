#ifndef _IF_MOTOR_H_
#define _IF_MOTOR_H_

#include "main.h"

void IF_MOTOR_Init(void);
void IF_MOTOR_Start(void);
void IF_MOTOR_Stop(void);
void IF_MOTOR_Forward(void);
void IF_MOTOR_Reversal(void);
void IF_MOTOR_SetSpeed(uint8_t speed);

#endif /*_IF_MOTRO_H_*/
