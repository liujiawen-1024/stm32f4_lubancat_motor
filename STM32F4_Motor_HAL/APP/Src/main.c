#include "main.h"
#include "if_motor.h"

int32_t count = 0;
float sum = 0.0f;
#define PID_TARGET_PULSES 1040

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    
    APP_MOTOR_Init();
    APP_ENCODER_Init();
    APP_ENCODER_Start();
    APP_ENCODER_Clear();
    APP_UART_Init(115200);

    APP_MOTOR_PID_Init();
    
    printf("\r\n开始 PID 位置控制测试\r\n");
    printf("目标: %d 个脉冲\r\n", PID_TARGET_PULSES);
    
    APP_MOTOR_Go2Revolutions(20.0f);
    
    while(1)
    {
        APP_MOTOR_RunToPosition(PID_TARGET_PULSES);
        
        static uint32_t timer = 0;
        if (++timer >= 100) {
            timer = 0;
            int32_t current = APP_ENCODER_Count();
            int32_t error = PID_TARGET_PULSES - current;
            float output = APP_MOTOR_GetPIDOutput();
            printf("目标: %d, 当前位置: %ld, 误差: %ld, PID输出: %.2f\r\n",
                   PID_TARGET_PULSES, (long)current, (long)error, output);
        }
        
        HAL_Delay(10);
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
