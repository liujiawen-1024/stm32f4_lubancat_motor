#include "motor.h"

TIM_HandleTypeDef htim1;

void PWM_Init(void)
{
	TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 8400;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
	
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
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
		
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_icHandle->Instance==TIM1)
  {
    PWM_GPIO_RCC_ON();

    PWM_PWM_RCC_ON();
    
    GPIO_InitStruct.Pin = PWM_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(PWM_PORT, &GPIO_InitStruct);
  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{

  if(tim_icHandle->Instance==TIM1)
  {
    PWM_PWM_RCC_OFF();
		
    HAL_GPIO_DeInit(PWM_PORT, PWM_PIN);
  }
}


/**
	* @brief	启动 PWM 输出
	*/
void PWM_Start(void)
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
}

/**
	* @brief	关闭 PWM 输出
	*/
void PWM_Stop(void)
{
	HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
}

/**
	* @brief	设置 PWM 占空比
	*	@param	duty：占空比 0 - 8400
	*/
void PWM_SetDuty(uint16_t duty)
{
	//	限幅保护
	if(duty > 8400)
	{
			duty = 8400;
	}
	
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,duty);
}

/**
	* @brief	设置 PWM 速度
	*	@param	speed：速度 对应占空比 0 - 8400
	*/
void PWM_SetSeppd(uint8_t speed)
{
	uint16_t duty = (speed * 8400) / 100;
	
	//	限幅保护
	if(duty > 8400)
	{
			duty = 8400;
	}
	
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,duty);
}

/**
	*	@brief		电机方向及使能初始化
	*	@note			默认全部关闭
	*/
void MOTOR_Init(void)
{
	GPIO_InitTypeDef gpio = {0};
	
	// 使能	GPIOE	时钟
	MOTOR_GPIO_CLK_EN();
	
	// 配置方向控制引脚	PE14,PE13 -- 推挽输出
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	gpio.Alternate = 0;
	
	gpio.Pin = MOTOR_AIN1_GPIO_PIN;
	HAL_GPIO_Init(MOTOR_AIN_PORT,&gpio);
	gpio.Pin = MOTOR_AIN2_GPIO_PIN;
	HAL_GPIO_Init(MOTOR_AIN_PORT,&gpio);
	
	// 配置使能引脚	PE8 -- 推挽输出
	gpio.Pin = MOTOR_STBY_GPIO_PIN;
	HAL_GPIO_Init(MOTOR_STBY_PORT,&gpio);
	
	// 设置默认安全状态	--	低电平全部关闭
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_STBY_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_STBY_PORT,MOTOR_AIN2_GPIO_PIN,GPIO_PIN_RESET);
}

/**
	*	@brief	电机启动
	*	@note		拉高 STBY
	*/
void MOTOR_Start(void)
{
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_STBY_GPIO_PIN,GPIO_PIN_SET);
}

/**
	*	@brief	电机关闭
	*	@note		拉低 STBY
	*					AIN 关闭
	*/
void MOTOR_Stop(void)
{
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_STBY_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_STBY_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_RESET);
}

/**
	*	@brief	电机正转
	*	@note		AIN1 = 1
	*         AIN2 = 0
	*/
void MOTOR_Forward(void)
{
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_AIN2_GPIO_PIN,GPIO_PIN_RESET);
}

/**
	*	@brief	电机反转
	*	@note		AIN1 = 0
	*         AIN2 = 1
	*/
void MOTOR_Reversal(void)
{
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_AIN_PORT,MOTOR_AIN2_GPIO_PIN,GPIO_PIN_SET);
}
