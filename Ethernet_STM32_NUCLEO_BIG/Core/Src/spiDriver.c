/*
 * spiDriver.c
 *
 *  Created on: Sep 20, 2023
 *      Author: NK KALAMBAY
 */
#include "main.h"
#include "spiDriver.h"


/*****************************************************
 * Read Control Register Command
 *****************************************************/
uint8_t SPI_READ_CONTROL_REG(enc28j60_phy_registers_t reg)
{
	sfr_bank bank = (sfr_bank) (reg & 0xE0);
	uint8_t address = (uint8_t) (reg & 0x1F);
	uint8_t returnValue[2] = {0x00};
	if(bank != sfr_common && bank != lastUsedBank)
	{
		SPI_Change_Bank(bank);
	}

	ENC28J60_SLAVE_SELECT();
	uint8_t bytesToSend[2];
	*(bytesToSend)= READ_CONTROL_REG | address;
	*(bytesToSend + 1) = DUMMY_BYTE;

	HARDWARE_SPI_WRITE_READ(&bytesToSend[0], &returnValue[0], 2);
	ENC28J60_SLAVE_UNSELECT();
	return returnValue[1];
}

/*****************************************************
 * Read Control Register Command MAC and MII Registers
 *****************************************************/
uint8_t SPI_READ_MAC_MII_REG(enc28j60_phy_registers_t reg)
{
	sfr_bank bank = (sfr_bank) (reg & 0xE0);
	uint8_t address = (uint8_t) (reg & 0x1F);
	uint8_t returnValue[3] = {0x00};
	if(bank != lastUsedBank && bank != sfr_common)
	{
		SPI_Change_Bank(bank);
	}
	ENC28J60_SLAVE_SELECT();
	uint8_t bytesToSend[3] = {0x00};
	*(bytesToSend)= READ_CONTROL_REG | address;
	*(bytesToSend + 1) = DUMMY_BYTE;
	*(bytesToSend + 2) = DUMMY_BYTE;

	HARDWARE_SPI_WRITE_READ(&bytesToSend[1], &returnValue[0], 3);

	ENC28J60_SLAVE_UNSELECT();
	return returnValue[2];

}

/*****************************************************
 * Read Buffer Memory
 *****************************************************/
uint8_t * SPI_READ_BUFFER_MEMORY(const uint8_t startAddress, uint16_t numberOfBytes, uint8_t * ptr)
{
	ENC28J60_SLAVE_SELECT();

	uint8_t cmd = READ_BUFFER_MEM | CONST_FOR_BUFFER_READ;

	//ptrData, dataSize)
	HARDWARE_SPI_WRITE(&cmd, 1); //Send the 1 byte as described

	uint8_t uDummyData = DUMMY_BYTE;
	uint8_t	tempData;
	for(uint16_t cnt = 0; cnt < numberOfBytes; cnt++)
	{

		HARDWARE_SPI_WRITE_READ(&uDummyData, &tempData, 1); //
		*(ptr + cnt) = tempData;
	}
	ENC28J60_SLAVE_UNSELECT();
	return ptr;
}

/*****************************************************
 * Write Control Register Command
 *****************************************************/
void SPI_WRITE_CONTROL_REG(enc28j60_phy_registers_t reg, uint8_t uiData)
{
	sfr_bank bank = (sfr_bank) (reg & 0xE0);
	uint8_t address = (uint8_t) (reg & 0x1F); // get address.
	if(bank != lastUsedBank && bank != sfr_common)
	{
		//Change Bank
		SPI_Change_Bank(bank);
	}
	//Then write CONTROL REGISTER VALUE...
	ENC28J60_SLAVE_SELECT();
	uint8_t bytesToSend[2];
	*(bytesToSend) = WRITE_CONTROL_REG | address;
	*(bytesToSend + 1) = uiData;
	HARDWARE_SPI_WRITE(&bytesToSend[0], 2);
	ENC28J60_SLAVE_UNSELECT();
}

/*****************************************************
 * Write buffer memory command
 *****************************************************/
void SPI_WRITE_BUFFER_MEM(uint16_t numOfBytes, uint8_t * data)
{
	//Then write CONTROL REGISTER VALUE...
	ENC28J60_SLAVE_SELECT();

	uint8_t cmd = WRITE_BUFFER_MEM | CONST_FOR_BUFFER_READ;

	HARDWARE_SPI_WRITE(&cmd, 1); //Send the 1 byte as described

	for(uint16_t cnt = 0; cnt < numOfBytes; cnt++)
	{
		HARDWARE_SPI_WRITE(data++, 1); //
	}

	ENC28J60_SLAVE_UNSELECT();
}

/*****************************************************
 * System Reset Command
 *****************************************************/
void SPI_SYSTEM_RESET(void)
{
	//Then write CONTROL REGISTER VALUE...
	ENC28J60_SLAVE_SELECT();

	uint8_t cmd = SYSTEM_RESET | DATA_CONSTANT;

	HARDWARE_SPI_WRITE(&cmd, 1); //Send the 1 byte as described

	ENC28J60_SLAVE_UNSELECT();
}

/*****************************************************
 * BIT FIELD SET COMMAND
 *****************************************************/
void SPI_BIT_FIELD_SET(enc28j60_phy_registers_t reg, ENC28J60_BITS bitsToSet)
{
	sfr_bank bank = (sfr_bank) (reg & 0xE0);
	uint8_t address = (uint8_t) (reg & 0x1F); // get address.
	if(bank != lastUsedBank && bank != sfr_common)
	{
		//Change Bank
		SPI_Change_Bank(bank);
	}

	uint8_t dataToSend[2] = {0};
	dataToSend[0] = BIT_FIELD_SET | address; //wait
	dataToSend[1] = (uint8_t) bitsToSet;

	//Then write CONTROL REGISTER VALUE...
	ENC28J60_SLAVE_SELECT();

	HARDWARE_SPI_WRITE(&dataToSend[0], 2);

	ENC28J60_SLAVE_UNSELECT();
}

/*****************************************************
 * BIT FIELD CLEAR COMMAND
 *****************************************************/
void SPI_BIT_FIELD_CLEAR(enc28j60_phy_registers_t reg, ENC28J60_BITS bitsToSet)
{
	sfr_bank bank = (sfr_bank) (reg & 0xE0);
	uint8_t address = (uint8_t) (reg & 0x1F); // get address.
	if(bank != lastUsedBank && bank != sfr_common)
	{
		//Change Bank
		SPI_Change_Bank(bank);
	}

	uint8_t dataToSend[2] = {0};
	dataToSend[0] = BIT_FIELD_CLEAR | address; //wait
	dataToSend[1] = (uint8_t) bitsToSet;

	//Then write CONTROL REGISTER VALUE...
	ENC28J60_SLAVE_SELECT();

	HARDWARE_SPI_WRITE(&dataToSend[0], 2);

	ENC28J60_SLAVE_UNSELECT();
}
void SPI_Change_Bank(sfr_bank bank)
{
	if(bank != lastUsedBank)
	{
		lastUsedBank = bank;

		uint8_t address = (uint8_t) (J60_ECON1 & 0x1F);

		//Read First...
		uint8_t RegContent[2] = {0};
		ENC28J60_SLAVE_SELECT();
		uint8_t bytesToSend[2];
		*(bytesToSend)= READ_CONTROL_REG | address;
		*(bytesToSend + 1) = DUMMY_BYTE;

		HARDWARE_SPI_WRITE_READ(&bytesToSend[0], &RegContent[0], 2);
		ENC28J60_SLAVE_UNSELECT();

		ENC28J60_DELAY(1);

		uint8_t currentValue = RegContent[1];
		currentValue &= 0xFC; //clear the lower 2 bits...

		switch(bank)
		{
		case sfr_bank1:
				{
					currentValue |= 0x01;
				}break;

		case sfr_bank2:
				{
					currentValue |= 0x02;
				}break;

		case sfr_bank3:
				{
					currentValue |= 0x03;
				}break;

		default:
				{
					//Do Nothing...
				}break;
		}

		//Write
		ENC28J60_SLAVE_SELECT();
		uint8_t ubytesToSend[2];
		*(ubytesToSend) = WRITE_CONTROL_REG | address;
		*(ubytesToSend + 1) = currentValue;
		HARDWARE_SPI_WRITE(&ubytesToSend[0], 2);
		ENC28J60_SLAVE_UNSELECT();
		ENC28J60_DELAY(1);

	}

}

