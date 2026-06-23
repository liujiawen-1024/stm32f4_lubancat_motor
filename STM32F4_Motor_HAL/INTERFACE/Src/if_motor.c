#include "if_motor.h"
#include "drv_pwm.h"
#include "bsp_motor.h"

/**
* @brief	舵机初始化
	*/
void IF_MOTOR_Init(void)
{
	BSP_MOTOR_Init();
	DIR_PWM_Init();
}


/**
* @brief	舵机启动
	*/
void IF_MOTOR_Start(void)
{
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_STBY_GPIO_PIN,GPIO_PIN_SET);
	DIR_PWM_Start();
}

/**
* @brief	舵机停止
	*/
void IF_MOTOR_Stop(void)
{
	DIR_PWM_Stop();
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_STBY_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN2_GPIO_PIN,GPIO_PIN_RESET);
}

/**
	* @brief	舵机正转
	* @note		AIN1 = 1
	*					AIN2 = 0
	*/
void IF_MOTOR_Forward(void)
{
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN2_GPIO_PIN,GPIO_PIN_RESET);
}

/**
	* @brief	舵机反转
	* @note		AIN1 = 0
	*					AIN2 = 1
	*/
void IF_MOTOR_Reversal(void)
{
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_PWM_GPIO_PORT,MOTOR_AIN2_GPIO_PIN,GPIO_PIN_SET);
}

/**
	* @brief	速度设置
	* @param	seppd：速度设置
	*	@note		seppd：0 ~ 100 转 0 ~ 8400
	*/
void IF_MOTOR_SetSpeed(uint8_t speed)
{
	// 限幅
	if(speed > 100)
	{
		speed = 100;
	}
	
	uint16_t setspeed = (speed * 8400) / 100;
	DIR_PWM_SetDuty(setspeed);
}
