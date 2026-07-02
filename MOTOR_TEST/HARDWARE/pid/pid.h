#ifndef __PID_H_
#define __PID_H_

#include "main.h"

typedef struct{
	// PID相关参数
	float Kp;
	float Ki;
	float Kd;
	// 目标值
	float target;
	// 反馈值
	float feedback;
	// 误差
	float error;
	// 积分和
	float integral;
	// 历史误差
	float prev_error;
	// 输出
	float output;
	float output_max;
	float output_min;
}PID_Handle_t;

void PID_Init(PID_Handle_t *pid,float Kp,float Ki,float Kd,float output_max);
void PID_Reset(PID_Handle_t *pid);
void PID_SetTarget(PID_Handle_t *pid,float target);
float PID_Update(PID_Handle_t *pid,float feedback);
float PID_GetOutput(PID_Handle_t *pid);

#endif /* __PID_H_ */
