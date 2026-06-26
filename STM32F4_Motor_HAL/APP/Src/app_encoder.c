#include "app_encoder.h"
#include "bsp_encoder.h"
#include <stdlib.h>   // 提供 abs() 函数

void APP_ENCODER_Init(void)
{
	BSP_ENCODER_Init();
	BSP_ENCODER_Clear();
}

void APP_ENCODER_Start(void)
{
	BSP_ENCODER_Start();
}

int32_t APP_ENCODER_Count(void)
{
    // 读取原始 16 位值
    uint32_t raw = BSP_ENCODER_Count();  // 假设返回 0~65535
    int32_t count = (int32_t)(int16_t)raw;  // 转换为有符号 -32768~32767
    
    // 方向取反
    count = -count;

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
