/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include "fatfs_sd.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum _GPIO_STATE
{
	GPIO_IDLE 	= 0,
	GPIO_HIGH	= 1,
	GPIO_LOW	= 2
}GPIO_STATE;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */


char time[24] = {0};
char date[24] = {0};

RTC_DateTypeDef gDate = {0};
RTC_TimeTypeDef gTime = {0};

extern uint8_t u8Var;
extern uint8_t u9Var;

volatile GPIO_STATE channel0 = GPIO_IDLE;
volatile GPIO_STATE channel1 = GPIO_IDLE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
bool logDataToSD(const uint8_t * ptr, const uint8_t * time, const uint8_t * date, const uint8_t * fileName);
FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
char buffer[1024];

UINT br, bw;  // File read/write count

bool bLogData = false;
/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;


/* to send the data to the uart */
void send_uart (char *string)
{
	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart2, (uint8_t*) string, len, 2000); // transmit in blocking mode
}

/* to find the size of data in the buffer */
int bufsize (char *buf)
{
	int i=0;
	while(*buf++ !='\0')i++;
	return i;
}

/* to clear the buffer */
void bufclear (void)
{
	for (int i = 0; i < 1024; i++)
	{
		buffer[i] = '\0';
	}
}


void set_time(void)
{
	  RTC_TimeTypeDef sTime = {0};
	  RTC_DateTypeDef sDate = {0};
	  /** Initialize RTC and set the Time and Date
	  */
	  sTime.Hours = 0x17;
	  sTime.Minutes = 0x05;
	  sTime.Seconds = 0x00;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	  sDate.Month = RTC_MONTH_DECEMBER;
	  sDate.Date = 0x18;
	  sDate.Year = 0x23;

	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void set_alarm(void)
{
	RTC_AlarmTypeDef sAlarm = {0};
	  /** Enable the Alarm A
	  */
	  sAlarm.AlarmTime.Hours = 0x0;
	  sAlarm.AlarmTime.Minutes = 0x0;
	  sAlarm.AlarmTime.Seconds = 0x0;
	  sAlarm.AlarmTime.SubSeconds = 0x0;
	  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	  sAlarm.AlarmDateWeekDay = 0x1;
	  sAlarm.Alarm = RTC_ALARM_A;
	  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN RTC_Init 2 */

	  /* USER CODE END RTC_Init 2 */
}

void get_time(void)
{

	/* Get the RTC current Time */
	 HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	 HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

	/* Display time Format: hh:mm:ss */
	 sprintf((char*) time, "%02d:%02d:%02d", gTime.Hours, gTime.Minutes, gTime.Seconds);

	/* Display date Format: dd-mm-yy */
	 sprintf((char*) date, "%02d-%02d-%2d", gDate.Date, gDate.Month, 2000 + gDate.Year);
}

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
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_FATFS_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  set_time();

  //set_alarm ();

  fresult = f_mount(&fs,"",0);
  if(fresult != FR_OK )
  {
	  send_uart ("error in mounting SD CARD....\n");
  }
  else
  {
	  send_uart("SD CARD mounted successfully....\n");
  }



  /************************************* Card capacity details ***********************************/
  /* Check free space */

  	 f_getfree("", &fre_clust, &pfs);

	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	sprintf (buffer, "SD CARD Total Size: \t%lu\n", total);
	send_uart(buffer);
	bufclear();

	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	sprintf (buffer, "SD CARD Free Space: \t%lu\n", free_space);
	send_uart(buffer);
	bufclear();
#if 0
    /************************************* The following operation is using PUTS and GETS ***********************************/
  	/* Open file to write/ create a file if it doesn't exist */
      fresult = f_open(&fil, "file1.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	/* Writing text */
	f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);

  	/* Close file */
  	fresult = f_close(&fil);

  	send_uart ("File1.txt created and the data is written \n");

  	/* Open file to read */
  	fresult = f_open(&fil, "file1.txt", FA_READ);

  	/* Read string from the file */
  	f_gets(buffer, fil.sect, &fil);

  	send_uart(buffer);

  	/* Close file */
  	f_close(&fil);

  	bufclear();

    /************************************* The following operation is using f_write and f_read ***********************************/
  	/* Open file to write/ create a file if it doesn't exist */
      fresult = f_open(&fil, "file2.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	/* Writing text */
	strcpy (buffer, "This is File 2 and it says Hello from controller\n" );

	fresult = f_write(&fil,buffer, bufsize(buffer), &bw);

	send_uart ("File2.txt created and data is written\n");

  	/* Close file */
  	f_close(&fil);

  	//clearing buffer to show result obtained is from the file
  	bufclear();

  	/* Open file to read */
  	fresult = f_open(&fil, "file2.txt", FA_READ);

  	/*Read data from the file
  	 * please see the function details for the arguments*/
  	f_read(&fil, buffer, fil.sect, &br);
  	send_uart(buffer);

  	/* Close file */
  	f_close(&fil);

  	bufclear();
#endif


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //get_time();
	  switch(channel0)
	  {
		  case GPIO_LOW:
		  {
			  const uint8_t * response = (uint8_t *) "Channel 0 Low at \r\n";
			  //HAL_UART_Transmit_IT(&huart2, (uint8_t *) response, strlen((char *) response));
			  HAL_UART_Transmit(&huart2, (uint8_t *) response, strlen((char *) response), 50);

			  //bool logDataToSD(const uint8_t * ptr, const uint8_t * time, const uint8_t * date, const uint8_t * fileName)
			  get_time();
			  //logDataToSD((char *) "Channel 0 Low at ", (uint8_t *) time, (uint8_t *) date, (uint8_t *) "channel0.txt");
			  if(logDataToSD((const uint8_t *) "Channel 0 Low at", (const uint8_t *) time, (const uint8_t *) date, (const uint8_t *) "channel0.txt") == true)
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING SUCCESS From Channel 0\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }else
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING FAILED From Channel 0\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }
			  channel0 = GPIO_IDLE;
		  }break;

		  case GPIO_HIGH:
		  {
			  const uint8_t * response = (uint8_t *) "Channel 0 High.\r\n";

			  //HAL_UART_Transmit_IT(&huart2, (uint8_t *) response, strlen((char *) response));
			  HAL_UART_Transmit(&huart2, (uint8_t *) response, strlen((char *) response), 50);
			  get_time();
			  if(logDataToSD((const uint8_t *) "Channel 0 High at", (uint8_t *) time, (uint8_t *) date, (uint8_t *) "channel0.txt") == true)
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING SUCCESS From Channel 0\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }else
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING FAILED From Channel 0\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }
			  channel0 = GPIO_IDLE;
		  }break;

		  default:
		  {

		  }break;
	  }

	  switch(channel1)
	  {
		  case GPIO_LOW:
		  {
			  const uint8_t * response = (uint8_t *) "Channel 1 Low.\r\n";
			  //HAL_UART_Transmit_IT(&huart2, (uint8_t *) response, strlen((char *) response));
			  HAL_UART_Transmit(&huart2, (uint8_t *) response, strlen((char *) response), 50);

			  get_time();
			  if(logDataToSD((const uint8_t *) "Channel 1 High at", (uint8_t *) time, (uint8_t *) date, (uint8_t *) "channel1.txt") == true)
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING SUCCESS From Channel 1\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }else
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING FAILED From Channel 1\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }
			  channel1 = GPIO_IDLE;
		  }break;

		  case GPIO_HIGH:
		  {

			  const uint8_t * response = (uint8_t *) "Channel 1 High.\r\n";
			  //HAL_UART_Transmit_IT(&huart2, (uint8_t *) response, strlen((char *) response));
			  HAL_UART_Transmit(&huart2, (uint8_t *) response, strlen((char *) response), 50);

			  get_time();
			  if(logDataToSD((const uint8_t *) "Channel 1 High at", (uint8_t *) time, (uint8_t *) date, (uint8_t *) "channel1.txt") == true)
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING SUCCESS From Channel 1\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }else
			  {
				  const uint8_t * dataLoggingResponse = (const uint8_t *) "LOGGING FAILED From Channel 1\r\n";
				  HAL_UART_Transmit(&huart2, (uint8_t *) dataLoggingResponse, strlen((char *) dataLoggingResponse), 50);
			  }
			  channel1 = GPIO_IDLE;
		  }break;

		  default:
		  {

		  }break;
	  }
//	  if set
#if 0
	  if(u8Var)
	  {
		  HAL_Delay(20);
		  if()
		  get_time();
		  size_t length = strlen(time);
		  HAL_UART_Transmit(&huart2, (uint8_t *) time, length, 100);
		  //HAL_Delay(20);
		  length = strlen(date);
		  HAL_UART_Transmit(&huart2, (uint8_t *)date, length, 100);

		  u8Var = 0;

	  }

	  if(u9Var)
	  {

		  get_time();
		  size_t length = strlen(time);
		  HAL_UART_Transmit(&huart2, (uint8_t *) time, length, 100);
		  //HAL_Delay(20);
		  length = strlen(date);
		  HAL_UART_Transmit(&huart2, (uint8_t *) date, length, 100);
		  HAL_Delay(1000);
		  u9Var = 0;

	  }
#endif

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
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

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x14;
  sTime.Minutes = 0x45;
  sTime.Seconds = 0x50;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_DECEMBER;
  sDate.Date = 0x11;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_CS_Pin */
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Channel1_Pin Channel0_Pin */
  GPIO_InitStruct.Pin = Channel1_Pin|Channel0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_9)
	{
		if(HAL_GPIO_ReadPin(Channel0_GPIO_Port, Channel0_Pin) == 0)
		{
			channel0 = GPIO_LOW;
		}else
		{
			channel0 = GPIO_HIGH;
		}
	}else if(GPIO_Pin == GPIO_PIN_8)
	{
		if(HAL_GPIO_ReadPin(Channel1_GPIO_Port, Channel1_Pin) == 0)
		{
			channel1 = GPIO_LOW;
		}else
		{
			channel1 = GPIO_HIGH;
		}
	}
}

bool logDataToSD(const uint8_t * ptr, const uint8_t * time, const uint8_t * date, const uint8_t * fileName)
{
	uint8_t	buffer[64] = {0};
	fresult = open_append(&fil, (char *) fileName); //f_open(&fil, (char *) fileName, FA_CREATE_ALWAYS | FA_WRITE | FA_OPEN_APPEND);

	if(fresult != FR_OK)
	{
		return false;
	}

	strcpy((char *) buffer, (char *) ptr);
	strcat((char *) buffer, (char *) " ");
	strcat((char *) buffer, (char *) time);
	strcat((char *) buffer, (char *) " ");
	strcat((char *) buffer, (char *) date);
	strcat((char *) buffer, (char *) "\r\n");

	/* Writing text */
	//strcpy (buffer, "This is File 2 and it says Hello from controller\n" );

	/* Writing text */
	//f_puts("This data is from the FILE1.txt. And it was written using ...f_puts... ", &fil);

	fresult = f_write(&fil, buffer, strlen((char *) buffer), &bw);

	f_close(&fil);

	if(fresult == FR_OK)
	{
		return true;
	}else
	{
		return false;
	}
}

FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
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
