#include "alg_pid.h"

#include "alg_pid.h"

/**
 * @brief 初始化 PID 控制器
 * @param pid          PID 句柄
 * @param Kp           比例系数
 * @param Ki           积分系数
 * @param Kd           微分系数
 * @param output_max   输出限幅（正值，对称）
 */
void ALG_PID_Init(PID_Handle_t *pid, float Kp, float Ki, float Kd, float output_max)
{
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
	pid->output_max = output_max;
	pid->output_min = -output_max;
	
	ALG_PID_Reset(pid);
}

/**
	*	@brief	重置 PID 状态（清空历史值）
	*/
void ALG_PID_Reset(PID_Handle_t *pid)
{
	pid->target = 0;
	pid->feedback = 0;
	pid->error = 0;
	pid->integral = 0;
	pid->prev_error = 0;
	pid->output = 0;
}

/**
	*	@brief	设置目标值
	*/
void ALG_PID_SetTarget(PID_Handle_t *pid, float target)
{
	pid->target = target;
}

/**
	*	@brief	PID 核心计算
	* @param	pid	PID句柄
	*	@param	feedback	当前反馈值（如编码器位置）
	*	@terval	PID输出值（PWM）
	*/
float ALG_PID_Update(PID_Handle_t *pid, float feedback)
{
	//	保存反馈值
	pid->feedback = feedback;
	
	//	计算误差
	pid->error = pid->target - feedback;
	
	//	比例项（P）
	//	系数 × 误差
	//	误差越大出力越大
	float P = pid->Kp * pid->error;
	
	//	积分项（I）+ 防积分饱和
	pid->integral += pid->error;
	
	//	限幅：防止积分过大
	float integral_max = pid->output_max / (pid->Ki + 0.001f);
	
	if(pid->integral > integral_max) 	pid->integral = integral_max;
	if(pid->integral < -integral_max) pid->integral = -integral_max;
	
	float I = pid->Ki * pid->integral;
	
	//	微分项（D）
	float	D = pid->Kd * (pid->error - pid->prev_error);
	pid->prev_error = pid->error;
	
	//	计算总输出
	pid->output = P + I + D;
	
	//	输出限幅
	if(pid->output > pid->output_max)	pid->output = pid->output_max;
	if(pid->output < pid->output_min)	pid->output = pid->output_min;
	
	return pid->output;
}

/**
	*	@brief	获取当前输出值
	*/
float ALG_PID_GetOutput(PID_Handle_t *pid)
{
	return pid->output;
}
