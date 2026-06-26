#include "main.h"

int32_t count = 0;

#define PULSES_PER_REV  52    // 轮子转一圈的脉冲数
#define TARGET_REV      200.0f    // 目标
#define TARGET_PULSES   (int32_t)(TARGET_REV * PULSES_PER_REV)

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    
    APP_UART_Init(115200);
    
    APP_ENCODER_Init();
    APP_ENCODER_Start();
    APP_ENCODER_Clear();  // 清零
    
    APP_MOTOR_Init();
    APP_MOTOR_PID_Init();
    
    printf("\r\n===== PID 位置控制测试 =====\r\n");
    printf("目标: %.1f 圈 (%d 个脉冲)\r\n", TARGET_REV, TARGET_PULSES);
    
    // 启动位置控制
    APP_MOTOR_Go2Revolutions(TARGET_REV);
    
    uint32_t print_timer = 0;
    
    while(1)
    {
        // 持续运行 PID
        APP_MOTOR_RunToPosition(TARGET_PULSES);
      
				count = APP_ENCODER_Count();
			
        // 每0.5秒打印一次
        if (++print_timer >= 50) {  // 50 * 10ms = 0.5秒
            print_timer = 0;
            int32_t current = APP_ENCODER_Count();
            int32_t error = TARGET_PULSES - current;
            float output = APP_MOTOR_GetPIDOutput();
            float rev = (float)current / PULSES_PER_REV;
            
            printf("目标: %5.1f圈 | 当前: %5.1f圈 | 误差: %5.1f圈 | PID: %6.2f | 当前脉冲数：%d\r\n",
                   TARGET_REV, rev, (float)error / PULSES_PER_REV, output,count);
        }
        
        HAL_Delay(10);  // 10ms 控制周期
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
