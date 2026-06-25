#include "drv_pwm.h"
#include "bsp_motor.h"

TIM_HandleTypeDef htim1;

/**
	*	@brief TIM1的PWM输出
	*	@note	PWM频率 = 168KHz/(Prescaler+1)/(Prescaler+1)
	*	当前配置：168KHz / 1 / 8400 = 20KHz
	*	占空比精度：1 / 8400 ≈ 0.012%
	*/
void DRV_PWM_Init(void)
{
		// 高级定时器主模式配置结构体
		TIM_MasterConfigTypeDef sCongifMaster = {0};
		// 输出比较（PWM）配置结构体
		TIM_OC_InitTypeDef sConfigOC = {0};
		// 刹车和死区配置结构体
		TIM_BreakDeadTimeConfigTypeDef sConfigBD = {0};
		
		DRV_PWM_RCC_ON();
		
		// 配置定时器时基
		htim1.Instance = TIM1;
		htim1.Init.Prescaler = 0;																							// 预分配器：不分频，直接使用168KHz
		htim1.Init.CounterMode = TIM_COUNTERMODE_UP;													// 向上计数
		htim1.Init.Period = 8400 - 1;																					// 自动重装载值：8400个计数周期		
		htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;										// 数字滤波器不分频
		htim1.Init.RepetitionCounter = 0;																			// 高级定时器重复计数器
		
		if(HAL_TIM_PWM_Init(&htim1) != HAL_OK)
		{
			Error_Handler();
		}
		
		// 配置定时器主模式
		sCongifMaster.MasterOutputTrigger = TIM_TRGO_RESET;										// 关闭主模式输出
		sCongifMaster.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;					// 禁用主从模式
		
		if(HAL_TIMEx_MasterConfigSynchronization(&htim1,&sCongifMaster) != HAL_OK)
		{
			Error_Handler();
		}
		
		// 配置PWM输出通道
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = 0;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
		sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		
		if(HAL_TIM_PWM_ConfigChannel(&htim1,&sConfigOC,TIM_CHANNEL_1) != HAL_OK)
		{
			Error_Handler();
		}
		
		// 配置刹车和四区
		sConfigBD.OffStateRunMode = TIM_OSSR_DISABLE;
		sConfigBD.OffStateIDLEMode = TIM_OSSI_DISABLE;
		sConfigBD.LockLevel = TIM_LOCKLEVEL_OFF;
		sConfigBD.DeadTime = 0;
		sConfigBD.BreakState = TIM_BREAK_DISABLE;
		sConfigBD.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
		sConfigBD.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
		
		if(HAL_TIMEx_ConfigBreakDeadTime(&htim1,&sConfigBD) != HAL_OK)
		{
			Error_Handler();
		}	
}

/**
	* @brief	设置PMM占空比
	*	@param	duty：占空比值 0 ~ 8400
	*	@note		0 -> 0%
	*					4200 -> 50%
	*					8400 -> 100%
	*/
void DRV_PWM_SetDuty(uint16_t duty)
{
		// 限幅保护
	if(duty > 8400)
	{
		duty = 8400;
	}
	
	// 设置比较值
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,duty);
}

/**
	* @brief	启动PWM输出
	*/
void DRV_PWM_Start(void)
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
}

/**
	* @brief	关闭PWM输出
	*/
void DRV_PWM_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
}
