#include "encoder.h"

/**
	*	@brief		编码器初始化
	*/
TIM_HandleTypeDef htim3;

void ENCODER_Init(void)
{
  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 8;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 8;
	
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* tim_encoderHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  if(tim_encoderHandle->Instance==TIM3)
	{
    ENCODER_RCC_ON();

    ENCODER_TIM3_RCC_ON();
		
    GPIO_InitStruct.Pin = ENCODER_A|ENCODER_B;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(ENCODER_PORT, &GPIO_InitStruct);
  }
}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* tim_encoderHandle)
{

  if(tim_encoderHandle->Instance==TIM3)
  {
    ENCODER_TIM3_RCC_OFF();

    HAL_GPIO_DeInit(ENCODER_PORT, ENCODER_A|ENCODER_B);

  }
}

void ENCODER_Start(void)
{
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);	
}

int32_t ENCODER_Count(void)
{
	return (int32_t)__HAL_TIM_GET_COUNTER(&htim3);
}

void ENCODER_Clear(void)
{
    // 直接清零 TIM3 的计数器
    __HAL_TIM_SET_COUNTER(&htim3, 0);
}

float ENCODER_GetRevolutions(void)
{
	int32_t count = ENCODER_Count();
  return (float)count / ENCODER_PULSES_PER_REV;  // 电机转了多少圈
}

