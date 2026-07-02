#ifndef __SPEED_CONTROL_H_
#define	__SPEED_CONTROL_H_

#include "main.h"
	
#define PULSES_PER_REV				1040				// 输出轴转一圈
#define CONTROL_PERIOD				0.01f				// 控制周期 10ms

void SPEED_Init(void);
void SPEED_SetTarget(float my_target_rpm);
void SPEED_Update(void);
float SPEED_GetCurrentRPM(void);
float SPEED_GetTargetRPM(void);
float SEPPD_GetPWM(void);

#endif	/* __SPEED_CONTROL_H_ */
