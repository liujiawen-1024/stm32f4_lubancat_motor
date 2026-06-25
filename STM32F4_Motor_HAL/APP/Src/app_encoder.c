#include "app_encoder.h"
#include "bsp_encoder.h"

void APP_ENCODER_Init(void)
{
	BSP_ENCODER_Init();
}

void APP_ENCODER_Start(void)
{
	BSP_ENCODER_Start();
}

int32_t APP_ENCODER_Count(void)
{
	int32_t count = 0;
	count = BSP_ENCODER_Count();
	return count;
}


float APP_ENCODER_GetRevolutions(void)
{
    int32_t count = BSP_ENCODER_Count();
    return (float)count / ENCODER_PULSES_PER_REV;  // 电机转了多少圈
}

void APP_ENCODER_Clear(void)
{
	BSP_ENCODER_Clear();
}
