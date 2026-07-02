#include "speed_control.h"
#include "pid.h"
#include "app_motor.h"
#include "encoder.h"

static PID_Handle_t speed_pid = {0};
static int32_t last_encoder = 0;
static float current_rpm = 0.0f;
static float target_rpm = 0.0f;

/**
	*	@brief	速度控制初始化
	*/
void SPEED_Init(void)
{
	// PID 参数
	PID_Init(&speed_pid,0.3f,0.05f,0.01f,100.0f);
	PID_Reset(&speed_pid);
	last_encoder = ENCODER_Count();
}

/**
	*	@brief	设置目标速度
	*	@param	my_target_rpm：目标转速（转/分钟）
	*/
void SPEED_SetTarget(float my_target_rpm)
{
	target_rpm = my_target_rpm;
	PID_SetTarget(&speed_pid,target_rpm);
}

/**
	*	@brief	获取当前实际转速
	*/
float SPEED_GetCurrentRPM(void)
{
	return current_rpm;
}

/**
	*	@brief	获取目标转速
	*/
float SPEED_GetTargetRPM(void)
{
	return target_rpm;
}

/**
	*	@brief	速度环更新
	*	@note		在定时器中断或主循环里每10ms更新一次
	*/
void SPEED_Update(void)
{
	// 读取当前的编码器值
	int32_t current_encoder = ENCODER_Count();
	
	// 计算一个周期内的脉冲增量
	int32_t delta = current_encoder - last_encoder;
	last_encoder = current_encoder;
	
	// 计算当前转速
	// 脉冲增量 / （每圈脉冲数） = 转速（10ms内）
	// 转数 / 控制周期（秒）× 60 = 转/分钟
	float revolutions = (float)delta / PULSES_PER_REV;
	current_rpm = (revolutions / CONTROL_PERIOD) * 60.0f;
	
	// PID 计算，输出 PWM 占空比
	float pwm_output = PID_Update(&speed_pid,current_rpm);
	
	// 限制 PWM 范围
	if(pwm_output > 100.0f) pwm_output = 100.0f;
	if(pwm_output < -100.0f) pwm_output = -100.0f;
	
	// 根据方向控制电机
	if(pwm_output >= 0)
	{
		// 正转
		APP_MOTOR_Ctrl(0,(uint8_t)pwm_output);
	}
	else
	{
		// 反转
		APP_MOTOR_Ctrl(1,(uint8_t)(-pwm_output));
	}
}

float SEPPD_GetPWM(void)
{
		return PID_GetOutput(&speed_pid);
}

