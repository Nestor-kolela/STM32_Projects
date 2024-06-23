/*
 * spiDriver.h
 *
 *  Created on: Sep 20, 2023
 *      Author: kolel
 */

#ifndef INC_SPIDRIVER_H_
#define INC_SPIDRIVER_H_

#include "main.h"
#include "spiDriver.h"
#include "enc28j60_Driver.h"



/* ================================== Extern Variables ======================================================*/
extern SPI_HandleTypeDef hspi1;


/*================================ Defines ==================================================*/
#ifndef ENC28J60_SLAVE_SELECT
#define ENC28J60_SLAVE_SELECT()		HAL_GPIO_TogglePin(ETHERNET_CS_GPIO_Port, ETHERNET_CS_Pin);
#endif

#ifndef	ENC28J60_SLAVE_UNSELECT
#define ENC28J60_SLAVE_UNSELECT()	HAL_GPIO_TogglePin(ETHERNET_CS_GPIO_Port, ETHERNET_CS_Pin);
#endif

#define HARDWARE_SPI_WRITE(ptrData, dataSize)			HAL_SPI_Transmit(&hspi1, ptrData, dataSize, 1000)
#define HARDWARE_SPI_WRITE_READ(txData, rxData, size)		HAL_SPI_TransmitReceive(&hspi1, txData, rxData, size, 1000)
#define DUMMY_BYTE					0xAA

#define ENC28J60_DELAY(x)			HAL_Delay(1);//wait for 1 milli-second to wait for pin to go fully high


typedef enum _SPI_INSTRUCTION
{
	read_control_reg 	= 0x00,
	reag_buffer_mem		= 0x01,
	write_control_reg	= 0x02,
	write_buffer_mem	= 0x03,
	bit_field_set		= 0x04,
	bit_clear_clear		= 0x05,
	system_reset_cmd	= 0x07
}SPI_INSTRUCTION;

typedef enum _ENC28J60_BITS
{
	bit_one 	= 1u,
	bit_two		= 1 << 1u,
	bit_three	= 1 << 2u,
	bit_four	= 1 << 3u,
	bit_five	= 1 << 4u,
	bit_six		= 1 << 5u,
	bit_seven	= 1 << 6U,
	bit_eight	= 1 << 7U
 }ENC28J60_BITS;

void SPI_Change_Bank(sfr_bank bank);
uint8_t SPI_READ_CONTROL_REG(enc28j60_phy_registers_t reg);
void SPI_WRITE_CONTROL_REG(enc28j60_phy_registers_t reg, uint8_t uiData);

#endif /* INC_SPIDRIVER_H_ */
