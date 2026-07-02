#include "app_motor.h"
#include "motor.h"

/**
	*	@brief	电机初始化
	*/
void APP_MOTOR_Init(void)
{
	PWM_Init();
	MOTOR_Init();
}

/**
	*	@brief	电机启动
	*/
void APP_MOTOR_Start(void)
{
	PWM_Start();
	MOTOR_Start();
}

/**
	*	@brief	电机关闭
	*/
void APP_MOTOR_Stop(void)
{
	MOTOR_Stop();
}

/**
	* @brief	电机控制
	*	@param	direction：方向 - 0：正转 1：反转
	*	@param	seppd：0-100 对应 PWM 占空比  0-8400
	*/
void APP_MOTOR_Ctrl(uint8_t driection,uint8_t speed)
{
	if(driection != 1 && driection != 0)	return;

	if(driection == 0)
	{
		MOTOR_Forward();
	}
	else if(driection == 1)
	{
		MOTOR_Reversal();
	}
	PWM_SetSeppd(speed);
}
