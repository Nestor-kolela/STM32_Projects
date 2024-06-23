/*
 * enc28j60_Driver.c
 *
 *  Created on: Sep 20, 2023
 *      Author: NK KALAMBAY
 */

#include "spiDriver.h"
#include "enc28j60_Driver.h"

sfr_bank lastUsedBank = sfr_bank0; //default bank...

void Ethernet_Init(void);


