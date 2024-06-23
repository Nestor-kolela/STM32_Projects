/*
 * blinky.c
 *
 *  Created on: Oct 4, 2023
 *      Author: NK Kalambay
 */

#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "semphr.h"
#include <math.h>


void vBlinky(void * pvParameters)
{
	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin); //LED_BLUE_Pin
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	while(1)
	{
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin); //LED_BLUE_Pin
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
