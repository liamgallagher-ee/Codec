/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "cordic.h"
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "gpio.h"
#include "stm32g4xx_hal_sai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AUDIO_BLOCK_SIZE 64 //64 stereo samples per block
#define BUFFER_SIZE (AUDIO_BLOCK_SIZE * 2 * 2) // 2 channels * 2 (two halves) * block size

uint16_t audio_rx_buffer(BUFFER_SIZE) // DMA target recieve array
uint16_t audio_tx_buffer(BUFFER_SIZE) // DMA target send array
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_SAI1_Init();
  MX_CORDIC_Init();
  /* USER CODE BEGIN 2 */
  //audio DMA stream setup
  //ensure buffer is 0 to prevent pops and clicks
  memset(audio_tx_buffer,0,BUFFER_SIZE);

  //start reciever DMA(SAI1_A), will idle waiting for master
  HAL_SAI_Recieve_DMA(&hsai_BlockA1, (uint8_t*)audio_rx_buffer, uint16_t BUFFER_SIZE);

  //start transmitter(SAI1_B), will launch both simultaneously
  HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t*)audio_tx_buffer, uint16_t BUFFER_SIZE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    
    /* USER CODE BEGIN 3 */
    
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 73;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV8;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  
}

/* USER CODE BEGIN 4 */
//when DMA fills first half of the RX buffer
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  //make sure callback is from reciever
  if(hsai->Instance == SAI1_Block_A)
  {
    uint16_t block_bytes = (AUDIO_BLOCK_SIZE*2)*sizeof(uint32_t);

    //audio pass through
    memcpy(&audio_tx_buffer,&audio_rx_buffer,block_bytes);

    //DSP Placeholder
    //process_dsp(&audio_tx_buffer[0],AUDIO_BLOCK_SIZE*2);
  }
}
//when DMA fills second half of RX buffer
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if(hsai->Instance == SAI1_Block_A)
  {
    uint16_t block_bytes = (AUDIO_BLOCK_SIZE*2)*sizeof(uint32_t);
    uint16_t offset = AUDIO_BLOCK_SIZE*2;

    //audio passthrough
    memcpy(&audio_tx_buffer[offset],&audio_rx_buffer[offset],block_bytes);

    //DSP Placeholder
    //process_dsp(&audio_tx_buffer[offset],AUDIO_BLOCK_SIZE*2);
  
  }
}

/* DSP processing
void process_dsp_example(uint32_t *slot_buffer, uint16_t total_slots)
{
    for (uint16_t i = 0; i < total_slots; i++)
    {
        //extract the active 16 bits (Assuming standard left-alignment)
        int16_t raw_sample = (int16_t)(slot_buffer[i] >> 16);
        
        //perform your math (e.g., lower the volume by half)
        raw_sample = raw_sample / 2; 
        
        //re-pack back into the 32-bit slot frame
        slot_buffer[i] = ((uint32_t)raw_sample) << 16;
    }
}
*/

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
#ifdef USE_FULL_ASSERT
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
