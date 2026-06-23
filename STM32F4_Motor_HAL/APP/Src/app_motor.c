#include "app_motor.h"
#include "if_motor.h"

// 电机相关状态标志
uint8_t motor_speed = 0;				// 0 ~ 100
uint8_t motor_status = 0;			// 0 - 关闭 -- 1 - 启动
uint8_t motor_direction = 0;		// 1 - 正转 -- 0 - 反转

/**
	* @brief	舵机初始化
	*/
void APP_MOTOR_Init(void)
{
	IF_MOTOR_Init();
	IF_MOTOR_Forward();
	motor_status = MOTOR_STOP;
	motor_speed = 0;
	motor_direction = MOTOR_FORWARD;
}

/**
	* @brief	舵机启动
	*/
void APP_MOTOR_Start(void)
{
	IF_MOTOR_Start();
	motor_status = MOTOR_START;
}

/**
	* @brief	舵机关闭
	*/
void APP_MOTOR_Stop(void)
{
	IF_MOTOR_Stop();
	motor_status = MOTOR_STOP;
	motor_speed = 0;
}

/**
	* @brief	舵机控制
	*	@param 	speed：速度
	*	@param	directio：方向
	*/
void APP_MOTOR_Ctrl(uint8_t speed,uint8_t direction)
{
	motor_speed = speed;
	motor_status = MOTOR_START;
	
	// 如果方向是0，则反转，否则正转
	if(direction == 0)
	{
		if(motor_direction != MOTOR_REVERSAL)
		{
			motor_direction = MOTOR_REVERSAL;
			IF_MOTOR_Reversal();
		}
	}
	else
	{
		if(motor_direction != MOTOR_FORWARD)
		{
			motor_direction = MOTOR_FORWARD;
			IF_MOTOR_Forward();
		}
	}
	
	IF_MOTOR_SetSpeed(motor_speed);
	IF_MOTOR_Start();
}
