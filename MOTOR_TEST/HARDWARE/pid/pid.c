#include "pid.h"

/**
	*	@brief	PID初始化
	*	@param	pid：PID参数句柄
	*	@param	Kp：比例
	*	@param	Ki：积分
	*	@param	Kd：微分
	*	@param	output_max：最大输出值
	*/
void	PID_Init(PID_Handle_t *pid,float Kp,float Ki,float Kd,float output_max)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
	pid->output_max = output_max;
	pid->output_min = -output_max;
	
	PID_Reset(pid);
}

/**
	*	@brief	PID重置
	*	@param	pid：PID参数句柄
	*/
void PID_Reset(PID_Handle_t *pid)
{
	pid->feedback = 0;
	pid->target = 0;
	pid->error = 0;
	pid->integral = 0;
	pid->prev_error = 0;
	pid->output = 0;
}

/**
	*	@brief	PID设置目标值
	*	@param	pid：PID参数句柄
	*	@param	target：目标值
	*/
void PID_SetTarget(PID_Handle_t *pid,float target)
{
	pid->target = target;
}

/**
	*	@brief	PID核心计算
	*	@param	pid：PID参数句柄
	*	@param	feedback：反馈值
	*	@retval 输出值（PWM）
	*/
float PID_Update(PID_Handle_t *pid,float feedback)
{
	// 保存反馈值
	pid->feedback = feedback;
	
	// 计算误差
	pid->error = pid->target - pid->feedback;
	
	// 比例项（P）
	// 系数 × 误差
	// 误差越大出力越大
	float P = pid->Kp * pid->error;
	
	// 积分项（I）
	// 误差总和
	// 弥补误差
	pid->integral += pid->error;
	
	// 限幅
	float integral_max = pid->output_max / (pid->Ki + 0.001f);
	
	if(pid->integral > integral_max) 	pid->integral = integral_max;
	if(pid->integral < -integral_max) pid->integral = -integral_max;
	
	float I = pid->Ki * pid->integral;
	
	// 微分项（D）
	// 系数 × （本次误差 - 上次误差）
	// 缓冲
	float D = pid->Kd * (pid->error - pid->prev_error);
	pid->prev_error = pid->error;
	
	// 计算总输出
	pid->output = P + I + D;
	
	// 输出限幅
	if(pid->output > pid->output_max)	pid->output = pid->output_max;
	if(pid->output < pid->output_min)	pid->output = pid->output_min;
	
	return pid->output;
}

/**
	*	@brief	PID获取当前输出值
	*	@param	pid：PID参数句柄
	*	@retval 输出值
	*/
float PID_GetOutput(PID_Handle_t *pid)
{
	return pid->output;
}
