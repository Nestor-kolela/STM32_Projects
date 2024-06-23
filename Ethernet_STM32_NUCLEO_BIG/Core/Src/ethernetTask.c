/*
 * ethernetTask.c
 *
 *  Created on: Oct 5, 2023
 *      Author: NK Kalambay
 */
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "semphr.h"
#include <math.h>
#include "enc28j60_Driver.h"
#include "dataTypes.h"
#include "spiDriver.h"


extern xQueueHandle ethQueue;
void vEthernetTask(void * pvParameters)
{
	//eth_reg reagRegister = {.address = 0x00, .data = 0x00};
	//MX_LWIP_Init();
	while(1)
	{
		vTaskDelay(500 / portTICK_PERIOD_MS);
		//reagRegister.data = SPI_READ_CONTROL_REG(reagRegister.address = J60_ERDPTL);
		//reagRegister.data++;
		//MX_LWIP_Process();

	}
}
