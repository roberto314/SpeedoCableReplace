/*
    Abstraction layer for EEPROM ICs.

    Copyright (C) 2012 Uladzimir Pylinski aka barthess

    Licensed under the 3-Clause BSD license, (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://directory.fsf.org/wiki/License:BSD_3Clause
*/

#ifndef EEPROM_FILE_H_
#define EEPROM_FILE_H_

#include "ch.h"
#include "hal.h"
#include "table.h"

/*
 * ATMEL 24C32:
 * 4kB x 8
 * Page: 32Byte
 * Write Time 10ms
 * Address: 0b1010000
 */

#define EEPROM_PAGE_SIZE        32          // page size in bytes. Consult datasheet.
#define EEPROM_SIZE             4096        // total amount of memory in bytes
#define EEPROM_WRITE_TIME_MS    10          // time to write one page in mS. Consult datasheet!

/*
 * ATMEL 24C04:
 * 512B x 8
 * Page: 32Byte
 * Write Time 10ms
 * Address: 0b1010000
 */
/*
#define EEPROM_PAGE_SIZE        16          // page size in bytes. Consult datasheet.
#define EEPROM_SIZE             512        // total amount of memory in bytes
#define EEPROM_WRITE_TIME_MS    10          // time to write one page in mS. Consult datasheet!
*/

#define EEPROM_I2C_ADDR         0b1010000   // EEPROM address on bus
#define EEPROM_I2CD             I2CD1       /* ChibiOS I2C driver used to communicate with EEPROM */
#define EEPROM_TX_DEPTH         (EEPROM_PAGE_SIZE + 2)/* temporal transmit buffer depth for eeprom driver */

/* region for settings file */
#define EEPROM_SETTINGS_START         128
#define EEPROM_SETTINGS_SIZE          (sizeof(configstruct_t) / sizeof(uint8_t))
#define EEPROM_SETTINGS_END           (EEPROM_SETTINGS_START + EEPROM_SETTINGS_SIZE - 1)
#define CONFIG_VERSION                120
#define CHECKSUM                      0xaa55

#define EEPROM_START         0
#define EEPROM_END           EEPROM_SIZE-1
//#define NR_OF_TABLES 1

typedef struct {
	table_1d_u16_t table0; // Table to convert from real speed to Output Frequency
	uint16_t corr;         // Correction factor between real speed and shown speed (fake speed) (1000 == 100%)
	float fs_corr;         // factor for the calculation between input frequency and real speed
	uint32_t in_freq;
	uint32_t out_freq;
	uint16_t ver;          // Version of the data structure
	uint16_t chksm;        // Checksum to verify the integrity of the data
} configstruct_t;

typedef union {
    configstruct_t configstruct;
    uint8_t configarray[sizeof (configstruct_t)];
} configunion_t;

extern void configInit(void);
extern void configParameterRead(void);
extern void configParameterWrite(void);
extern void configLoadDefault(void);

#endif /* EEPROM_FILE_H_ */
