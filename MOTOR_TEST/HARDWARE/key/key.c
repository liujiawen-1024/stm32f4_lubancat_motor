#include "key.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef gpio = {0};
	
	KEY0_RCC_ON();
	KEY_UP_RCC_ON();
	
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_LOW;
	gpio.Pin = KEY0_PIN;
	HAL_GPIO_Init(KEY0_PORT,&gpio);
	
	gpio.Pin = KEY_UP_PIN;
	HAL_GPIO_Init(KEY_UP_PORT,&gpio);
}

