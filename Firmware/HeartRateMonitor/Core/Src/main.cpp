/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.cpp
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "beat.h"
#include "logger.h"
#include "screen.h"
#include "color.h"
#include "PGA.h"
#include "print.h"


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

Beat beat = Beat();

bool isConversionFinished = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config();
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main()
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
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	auto logger = Logger(&huart1);
	logger.log(Logger::INFO, "Logger Initialized");
	
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	
	Screen screen = ScreenFactory().scl(SCREEN_SCL_GPIO_Port, SCREEN_SCL_Pin).sda(SCREEN_SDA_GPIO_Port, SCREEN_SDA_Pin).cs(SCREEN_CS_GPIO_Port, SCREEN_CS_Pin)
			.dc(SCREEN_DC_GPIO_Port, SCREEN_DC_Pin).res(SCREEN_RES_GPIO_Port, SCREEN_RES_Pin)
			.bklt(SCREEN_BLK_GPIO_Port, SCREEN_BLK_Pin).build();
	HAL_Delay(100);
	
	screen.switchBacklight(true);
	HAL_Delay(100);
	screen.fillArea( 0,  0,  32,  32, WHITE);
	screen.fillArea(32,  0,  64,  32, WHITE);
	screen.fillArea(64,  0,  96,  32, WHITE);
	screen.fillArea(96,  0, 128,  32, WHITE);
	screen.fillArea( 0, 32,  32,  64, WHITE);
	screen.fillArea(32, 32,  64,  64, WHITE);
	screen.fillArea(64, 32,  96,  64, WHITE);
	screen.fillArea(96, 32, 128,  64, WHITE);
	screen.fillArea( 0, 64,  32,  96, WHITE);
	screen.fillArea(32, 64,  64,  96, WHITE);
	screen.fillArea(64, 64,  96,  96, WHITE);
	screen.fillArea(96, 64, 128,  96, WHITE);
	screen.fillArea( 0, 96,  32, 128, WHITE);
	screen.fillArea(32, 96,  64, 128, WHITE);
	screen.fillArea(64, 96,  96, 128, WHITE);
	screen.fillArea(96, 96, 128, 128, WHITE);
	
	char str[3] = {'0', '0', '0'};
	screen.drawFont(0, 0, str, 3, WHITE, YELLOW);

	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
	
	HAL_ADCEx_Calibration_Start(&hadc1);
	
	HAL_Delay(100);
	
	PGA pga(1, 20, 1024, &hi2c2, PGA::AddrMode::NC, logger);
	pga.setMagnification(20);
	
	HAL_ADC_Start_IT(&hadc1);
	HAL_TIM_Base_Start(&htim3);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		__NOP();
		__NOP();
		if (isConversionFinished)
		{
			isConversionFinished = false;
			uint16_t val = HAL_ADC_GetValue(&hadc1);
		
			if(beat.update(val))
			{
				char buffer[11] = {0};
				Print print(buffer, 11);
				print.uint8(beat.getRate()).uint16(val).carriagereturn().newline().end();
				HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t *>(buffer), 11, HAL_MAX_DELAY);
				screen.drawFont(0, 0, buffer, 3, WHITE, YELLOW);
			}
		}
	
    /* USER CODE END WHILE */


	// send this to screen
	
    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config()
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
