#include "app_motor.h"
#include "if_motor.h"
#include "alg_pid.h"

// 电机相关状态标志
uint8_t motor_speed = 0;				// 0 ~ 100
uint8_t motor_status = 0;			// 0 - 关闭 -- 1 - 启动
uint8_t motor_direction = 0;		// 1 - 正转 -- 0 - 反转

#define PULSES_PER_REV	52

static PID_Handle_t pid_position;

/**
	* @brief	电机初始化
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
	* @brief	电机启动
	*/
void APP_MOTOR_Start(void)
{
	IF_MOTOR_Start();
	motor_status = MOTOR_START;
}

/**
	* @brief	电机关闭
	*/
void APP_MOTOR_Stop(void)
{
	IF_MOTOR_Stop();
	motor_status = MOTOR_STOP;
	motor_speed = 0;
}

/**
	* @brief	电机控制
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

/**
	* @brief	初始化电机 PID 控制
	*/
void APP_MOTOR_PID_Init(void)
{
	// Kp,Ki,Kd,输出最大值
	ALG_PID_Init(&pid_position, 2.0f, 0.1f, 1.2f, 100.0f);
	ALG_PID_Reset(&pid_position);
}

/**
	* @brief	控制电机转到目标位置
	*	@param	target_pulses：目标脉冲数
	*/
void APP_MOTOR_RunToPosition(int32_t target_pulses)
{
    ALG_PID_SetTarget(&pid_position, (float)target_pulses);
    float current = (float)APP_ENCODER_Count();
    float output = ALG_PID_Update(&pid_position, current);
    
    if(output > 5.0f)
    {
        uint8_t speed = (uint8_t)output;
        IF_MOTOR_Forward();
        IF_MOTOR_SetSpeed(speed);
        IF_MOTOR_Start();
    }
    else if(output < -5.0f)
    {
        uint8_t speed = (uint8_t)(-output);       
        IF_MOTOR_Reversal();
        IF_MOTOR_SetSpeed(speed);
        IF_MOTOR_Start();
    }
    else
    {
        IF_MOTOR_Stop();
    }
}

/**
	* @brief	转到指定圈数
	*	@param	rev：圈数
	*/
void APP_MOTOR_Go2Revolutions(float rev)
{
	int32_t target = (int32_t)(rev * PULSES_PER_REV);
	APP_MOTOR_RunToPosition(target);
}



float APP_MOTOR_GetPIDOutput(void)
{
    return ALG_PID_GetOutput(&pid_position);
}
