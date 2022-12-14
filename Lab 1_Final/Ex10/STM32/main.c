/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void clearAllClock();
void setNumberOnClock(int num);
void clearNumberOnClock(int num);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  int second = 0, minute = 0, hour = 0;	// current led index for second, minute, hour
  int second_ = 0, minute_ = 0, hour_ = 0;	// previous led index for second, minute, hour
  int counter = -1; // counter for calculate current second, minute, hour
  // token instance for each led, token == 0 - led OFF; otherwise, led ON
  int enable[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	 if (counter == -1){	// Initial state - make all led off
		 clearAllClock();
		 counter = 1;	 //set counter equal to second 1
		 enable[0] = 3;	// now all second, minute, hour are at 0
		 setNumberOnClock(0); // enable[0] != 0 --> turn on clock 0
	 }
	 if (counter == 3600)	//Represent 1 hour passed, set counter = 0
		 counter = 0;

	 /* change led for SECOND (each 5-second) */
	 if (counter % 5 == 0){
		 second++;	//switch to next SECOND led index (from now on, it becomes current led)
		 second_ = second - 1;
		 if (second == 12)
			 second = 0;
		 enable[second_]--;	//decrease token of previous SECOND led
		 if (enable[second_] == 0)
			 clearNumberOnClock(second_);	//check token, if token==0, turn OFF previous SECOND led
		 enable[second]++;
		 setNumberOnClock(second);	//always turn ON current SECOND led
	 }

	 /* change led for MINUTE (each 5-minute or 300-second) */
	 if (counter % 300 == 0){
		 minute++;	//switch to next MINUTE led index (from now on, it becomes current led)
		 minute_ = minute - 1;
		 if (minute == 12)
			 minute = 0;
		 enable[minute_]--;
		 if (enable[minute_] == 0)
			 clearNumberOnClock(minute_);	//check token, if token==0, turn OFF previous MINUTE led
		 enable[minute]++;
		 setNumberOnClock(minute);  // always turn ON current MINUTE led
	 }

	 /* change led for HOUR (each 1-hour or 3600-second) */
	 if (counter % 3600 == 0){
		 hour++; //switch to next HOUR led index (from now on, it becomes current led)
		 hour_ = hour - 1;
		 if (hour == 12)
			 hour=0;
		 enable[hour_]--; //decrease token of previous HOUR led
		 if (enable[hour_] == 0)
			 clearNumberOnClock(hour_); //check token, if token==0, turn OFF previous HOUR led
		 enable[hour]++;
		 setNumberOnClock(hour);
	 }

	 counter++;	// as second increase
	 HAL_Delay(10);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CLK_0_Pin|CLK_1_Pin|CLK_2_Pin|CLK_3_Pin
                          |CLK_4_Pin|CLK_5_Pin|CLK_6_Pin|CLK_7_Pin
                          |CLK_8_Pin|CLK_9_Pin|CLK_10_Pin|CLK_11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CLK_0_Pin CLK_1_Pin CLK_2_Pin CLK_3_Pin
                           CLK_4_Pin CLK_5_Pin CLK_6_Pin CLK_7_Pin
                           CLK_8_Pin CLK_9_Pin CLK_10_Pin CLK_11_Pin */
  GPIO_InitStruct.Pin = CLK_0_Pin|CLK_1_Pin|CLK_2_Pin|CLK_3_Pin
                          |CLK_4_Pin|CLK_5_Pin|CLK_6_Pin|CLK_7_Pin
                          |CLK_8_Pin|CLK_9_Pin|CLK_10_Pin|CLK_11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void clearAllClock(){
	HAL_GPIO_WritePin(GPIOA, CLK_0_Pin|CLK_1_Pin|CLK_2_Pin|CLK_3_Pin|
							 CLK_4_Pin|CLK_5_Pin|CLK_6_Pin|CLK_7_Pin|
							 CLK_8_Pin|CLK_9_Pin|CLK_10_Pin|CLK_11_Pin, 1); //turn OFF all clock leds
}

void setNumberOnClock(int num){
	switch (num){
		case 0:{	// turn on clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_0_Pin, 0);
			break;
		}
		case 1:{ // turn on clock led 1
					HAL_GPIO_WritePin(GPIOA, CLK_1_Pin, 0);
					break;
		}
		case 2:{ // turn on clock led 2
			HAL_GPIO_WritePin(GPIOA, CLK_2_Pin, 0);
			break;
		}
		case 3:{ // turn on clock led 3
			HAL_GPIO_WritePin(GPIOA, CLK_3_Pin, 0);
			break;
		}
		case 4:{ // turn on clock led 4
			HAL_GPIO_WritePin(GPIOA, CLK_4_Pin, 0);
			break;
		}
		case 5:{ // turn on clock led 5
			HAL_GPIO_WritePin(GPIOA, CLK_5_Pin, 0);
			break;
		}
		case 6:{ // turn on clock led 6
			HAL_GPIO_WritePin(GPIOA, CLK_6_Pin, 0);
			break;
		}
		case 7:{ // turn on clock led 7
			HAL_GPIO_WritePin(GPIOA, CLK_7_Pin, 0);
			break;
		}
		case 8:{ // turn on clock led 8
			HAL_GPIO_WritePin(GPIOA, CLK_8_Pin, 0);
			break;
		}
		case 9:{ // turn on clock led 9
			HAL_GPIO_WritePin(GPIOA, CLK_9_Pin, 0);
			break;
		}
		case 10:{ // turn on clock led 10
			HAL_GPIO_WritePin(GPIOA, CLK_10_Pin, 0);
			break;
		}
		case 11:{ // turn on clock led 11
			HAL_GPIO_WritePin(GPIOA, CLK_11_Pin, 0);
			break;
		}
	}
}

void clearNumberOnClock(int num){
	switch (num){
		case 0:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_0_Pin, 1);
			break;
		}
		case 1:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_1_Pin, 1);
			break;
		}
		case 2:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_2_Pin, 1);
			break;
		}
		case 3:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_3_Pin, 1);
			break;
		}
		case 4:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_4_Pin, 1);
			break;
		}
		case 5:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_5_Pin, 1);
			break;
		}
		case 6:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_6_Pin, 1);
			break;
		}
		case 7:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_7_Pin, 1);
			break;
		}
		case 8:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_8_Pin, 1);
			break;
		}
		case 9:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_9_Pin, 1);
			break;
		}
		case 10:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_10_Pin, 1);
			break;
		}
		case 11:{ //turn off clock led 0
			HAL_GPIO_WritePin(GPIOA, CLK_11_Pin, 1);
			break;
		}
	}
}
/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
