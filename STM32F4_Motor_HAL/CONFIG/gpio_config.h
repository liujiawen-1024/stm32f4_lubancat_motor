#ifndef __GPIO_CONFIG_H_
#define __GPIO_CONFIG_H_

#define USART1_PORT					GPIOA
#define USART1_RCC_ON				__HAL_RCC_GPIOA_CLK_ENABLE();	
#define USART1_TX_PIN				GPIO_Pin_9
#define USART1_RX_PIN				GPIO_Pin_10

#endif /*__GPIO_CONFIG_H_*/
