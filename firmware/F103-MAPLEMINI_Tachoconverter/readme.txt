*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M3 STM32F103 on LeafLabs MapleMini       **
*****************************************************************************

** TARGET **

The demo runs on the MapleMini without the original MapleMini bootloader.
Serial Shell @ 38400 Baud, External I2C EPROM, PWM and ICP.

Pinout:
GPIO - mbed - Voltage
PA0  - 11  - 3V3 - free PWM2/1
PA1  - 10  - 3V3 - /HC05ON
PA2  -  9  - 3V3 - TX2 (Console + Debug)
PA3  -  8  - 3V3 - RX2 (Console + Debug)
PA4  -  7  - 3V3 - free
PA5  -  6  - 3V3 - free SPI1 SCK
PA6  -  5  - 3V3 - free SPI1 MISO
PA7  -  4  - 3V3 - free SPI1 MOSI
PA8  - 27  -  5V - VehicleSpeed In PWM1/1
PA9  - 26  -  5V - free PWM1/2
PA10 - 25  -  5V - free PWM1/3
PA11 - 24  -  5V - USD DM 
PA12 - 23  -  5V - USB DP 
PA13 - 22  -  5V - SWDIO
PA14 - 21  -  5V - SWCLK
PA15 - 20  -  5V - free PWM2/1
PB0  -  3  - 3V3 - free
PB1  -  -  -  5V - LED
PB2  -  2  -  5V - free boot
PB3  - 19  -  5V - MotorSpeed In PWM2/2 remap
PB4  - 18  -  5V - CW/CCW PWM3/1
PB5  - 17  - 3V3 - free PWM3/2
PB6  - 16  -  5V - SCL (EPROM)
PB7  - 15  -  5V - SDA (EPROM)
PB8  - but -  5V - MotorPWM Out PWM4/3
PB9  -  -  -  5V - USB_CON
PB10 -  1  -  5V - free PWM2/3
PB11 -  0  -  5V - free PWM2/4
PB12 -  31 -  5V - HC05 State
PB13 -  30 -  5V - free PWM1/1N
PB14 -  29 -  5V - free PWM1/2N
PB15 -  28 -  5V - free PWM1/3N
PC13 -  14 - 3V3 - EPROM WP
PC14 -  13 - 3V3 - HC05 Enable
PC15 -  12 - 3V3 - free


