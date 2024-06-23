/*
 * myTasks.c
 *
 *  Created on: Sep 20, 2023
 *      Author: NK Kalambay
 */


#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "semphr.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

//create mutex.... for uart...
SemaphoreHandle_t xUartMutex = NULL;

//exten variables...
extern UART_HandleTypeDef hlpuart1;
void vDebug( void * pvParameters )
{
	xUartMutex = xSemaphoreCreateMutex();

	if(xUartMutex == NULL)
	{
		Error_Handler(); //error....
	}
	uint8_t tempBuf[64] = {0};
	size_t bytesAvailable = 0;
	xSemaphoreGive(xUartMutex);
	while(true)
	{
		//wait for data to be available...

		//Take semaphore

		//Send Data.

		//Return Semaphore

		bytesAvailable = xPortGetFreeHeapSize();
		sprintf((char * ) tempBuf, "Free bytes in heap is %u\r\n", bytesAvailable);
		size_t strLength = strlen((char *) tempBuf);
		xSemaphoreTake(xUartMutex, portMAX_DELAY);
		HAL_UART_Transmit(&hlpuart1, (const uint8_t *) tempBuf, strLength, 100);
		xSemaphoreGive(xUartMutex);
		vTaskDelay(100 / portTICK_RATE_MS);

	}
}



