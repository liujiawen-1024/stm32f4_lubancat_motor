#ifndef _ALG_PID_H_
#define _ALG_PID_H_

#include <stdint.h>

typedef struct{
	//	PID参数
	float Kp;
	float Ki;
	float	Kd;
	
	//	目标值和反馈值
	float target;
	float feedback;
	float	error;
	
	//	历史值
	float	integral;
	float	prev_error;
	
	//	输出
	float output;
	float	output_max;
	float output_min;
}PID_Handle_t;

void 	ALG_PID_Init(PID_Handle_t *pid,float Kp,float Ki,float Kd,float output_max);
void 	ALG_PID_Reset(PID_Handle_t *pid);
void 	ALG_PID_SetTarget(PID_Handle_t *pid, float target);
float ALG_PID_Update(PID_Handle_t *pid, float feedback);
float ALG_PID_GetOutput(PID_Handle_t *pid);

#endif /* _ALG_PID_H_ */
