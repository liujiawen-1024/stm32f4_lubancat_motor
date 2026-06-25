#ifndef _APP_MOTOR_H_
#define _APP_MOTOR_H_

#include "main.h"

extern uint8_t motor_speed;			
extern uint8_t motor_status;			
extern uint8_t motor_direction;

#define MOTOR_START				1
#define	MOTOR_STOP				0
#define MOTOR_FORWARD			1
#define MOTOR_REVERSAL		0

void APP_MOTOR_Init(void);
void APP_MOTOR_Start(void);
void APP_MOTOR_Stop(void);
void APP_MOTOR_Ctrl(uint8_t speed,uint8_t direction);

void APP_MOTOR_PID_Init(void);
void APP_MOTOR_RunToPosition(int32_t target_pulses);
void APP_MOTOR_Go2Revolutions(float rev);
float APP_MOTOR_GetPIDOutput(void);

#endif /* _APP_MOTOR_H_ */
