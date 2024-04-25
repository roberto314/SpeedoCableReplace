/*
    Abstraction layer for EEPROM ICs.

    Copyright (C) 2012 Uladzimir Pylinski aka barthess

    Licensed under the 3-Clause BSD license, (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://directory.fsf.org/wiki/License:BSD_3Clause
*/

#include "ch.h"
#include "hal.h"
#include "comm.h"
#include "table.h"
#include "eeprom.h"
#include "string.h"
#include "main.h"
#include "chprintf.h"

/*
 ******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************
 */

I2CEepromFileStream eeFile;
EepromFileStream *eeFS;

static uint8_t eeprom_buf[EEPROM_TX_DEPTH];
extern configunion_t cudata;

static const I2CEepromFileConfig eeCfg = {
 EEPROM_START,
 EEPROM_END,
 EEPROM_SIZE,
 EEPROM_PAGE_SIZE,
 TIME_MS2I(EEPROM_WRITE_TIME_MS),
 &EEPROM_I2CD,
 EEPROM_I2C_ADDR,
 eeprom_buf,
};

/*
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 */

void configParameterRead(void) {
	configunion_t cureadback;
	uint8_t i;

	fileStreamSetPosition(eeFS, EEPROM_SETTINGS_START);
	fileStreamRead(eeFS, &(cureadback.configarray[0]), EEPROM_SETTINGS_SIZE);
	chprintf((BaseSequentialStream *)&SD2, "EPROM Dump Start");
	for (i=0; i < EEPROM_SETTINGS_SIZE; i++){
	  if (i%16 == 0){
	  	chprintf((BaseSequentialStream *)&SD2, "pos = %u\r\n", EEPROM_SETTINGS_START+i);
	  }
	  chprintf((BaseSequentialStream *)&SD2, "%02x ", cureadback.configarray[i]);
	}
	cli_println("");
	chprintf((BaseSequentialStream *)&SD2, "Check Version. RAM: %d, EEPROM: %d\r\n", cudata.configstruct.ver, cureadback.configstruct.ver);
	chprintf((BaseSequentialStream *)&SD2, "Checksum: %x\r\n", cureadback.configstruct.chksm);
	if ((cureadback.configstruct.chksm == CHECKSUM) && (cureadback.configstruct.ver == CONFIG_VERSION)){
		chprintf((BaseSequentialStream *)&SD2, "Checksum and Version match.\r\n");
		memcpy(&(cudata.configarray[0]), &(cureadback.configarray[0]), EEPROM_SETTINGS_SIZE);
	}
	else{
		chprintf((BaseSequentialStream *)&SD2, "Checksum or Version mismatch. Save config data first.\r\n");
		return;
	}
}

void configParameterWrite(void) {

  fileStreamSetPosition(eeFS, EEPROM_SETTINGS_START);
	if (EEPROM_SETTINGS_END > (EEPROM_SIZE -1)){
		chprintf((BaseSequentialStream *)&SD2, "Size of config is too big for EEPROM! Size is: %d\r\n", EEPROM_SETTINGS_SIZE);
		return;
	}
	chprintf((BaseSequentialStream *)&SD2, "config fits. Size is: %d, End of config: %d\r\n", EEPROM_SETTINGS_SIZE, EEPROM_SETTINGS_END);
    fileStreamWrite(eeFS, &(cudata.configarray[0]), EEPROM_SETTINGS_SIZE);
}

void configInit(void) {
    chprintf((BaseSequentialStream *)&SD2, "Open EEPROM...");
    eeFS = I2CEepromFileOpen(&eeFile, &eeCfg, EepromFindDevice(EEPROM_DEV_24XX));
    chprintf((BaseSequentialStream *)&SD2, "Done\r\n");
    configParameterRead();
    cudata.configstruct.in_freq = 500000;
    cudata.configstruct.out_freq = 500000;
    cudata.configstruct.fs_corr = 4.42;
    cudata.configstruct.corr = 1050;   //1050 = 105%
//    set_tim_freq(TIM3, cudata.configstruct.in_freq);
//    set_tim_freq(TIM4, cudata.configstruct.out_freq);
    //fileStreamClose(eeFS);
    //chprintf((BaseSequentialStream *)&SD2, "Closed\r\n");

}
