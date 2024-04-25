*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M3 STM32F103 on LeafLabs MapleMini       **
*****************************************************************************

** TARGET **

The demo runs on the MapleMini without the original MapleMini bootloader.
Serial Shell @ 38400 Baud, External I2C EPROM, PWM and ICP.

Pinout:
GPIO - mbed
PA0  - 11  - PWM2/1
PA1  - 10  - PWM2/2
PA2  -  9  - TX2 (Console + Debug)
PA3  -  8  - RX2 (Console + Debug)
PA4  -  7  - 
PA5  -  6  - 
PA6  -  5  - ICP3/1
PA7  -  4  - PWM3/2
PA8  - 27  - VehicleSpeed In PWM1/1
PA9  - 26  - PWM1/2
PA10 - 25  - PWM1/3
PA11 - 24  - USD DM 
PA12 - 23  - USB DP 
PA13 - 22  - SWDIO
PA14 - 21  - SWCLK
PA15 - 20  - PWM2/1
PB0  -  3  - free
PB1  -  -  - LED
PB2  -  2  - boot
PB3  - 19  - MotorSpeed In PWM2/2 remap
PB4  - 18  - CW/CCW PWM3/1
PB5  - 17  - PWM3/2
PB6  - 16  - SCL (EPROM)
PB7  - 15  - SDA (EPROM)
PB8  - but - MotorPWM Out PWM4/3
PB9  -  -  - USB_CON
PB10 -  1  - PWM2/3
PB11 -  0  - PWM2/4
PB12 -  31 - HC05 State
PB13 -  30 - PWM1/1N
PB14 -  29 - PWM1/2N
PB15 -  28 - PWM1/3N
PC13 -  14 - EPROM WP
PC14 -  13 - HC05 Enable
PC15 -  12 - 


