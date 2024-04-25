/*
This file is part of Trunetcopter.

Trunetcopter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Trunetcopter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Trunetcopter.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef I2C_LOCAL_H_
#define I2C_LOCAL_H_

#define I2C_BUS I2CD1

#include "ch.h"
#include "hal.h"
typedef bool bool_t;

void I2CInitLocal(void);
msg_t i2c_transmit(i2caddr_t addr, const uint8_t *txbuf,
                  size_t txbytes, uint8_t *rxbuf, size_t rxbytes);
msg_t i2c_receive(i2caddr_t addr, uint8_t *rxbuf, size_t rxbytes);

int8_t i2c_readBit(i2caddr_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
int8_t i2c_readBits(i2caddr_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
int8_t i2c_readByte(i2caddr_t devAddr, uint8_t regAddr, uint8_t *data);
int8_t i2c_readWord(i2caddr_t devAddr, uint8_t regAddr, uint16_t *data);
int8_t i2c_readBytes(i2caddr_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
int8_t i2c_readWords(i2caddr_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

bool_t i2c_writeBit(i2caddr_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
bool_t i2c_writeBits(i2caddr_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
bool_t i2c_writeByte(i2caddr_t devAddr, uint8_t regAddr, uint8_t data);
bool_t i2c_writeWord(i2caddr_t devAddr, uint8_t regAddr, uint16_t data);
bool_t i2c_writeBytes(i2caddr_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
bool_t i2c_writeWords(i2caddr_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

#endif /* I2C_LOCAL_H_ */
