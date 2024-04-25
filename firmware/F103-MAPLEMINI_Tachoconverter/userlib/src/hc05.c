/*
 * hc05.c
 *
 *  Created on: Apr 25, 2024
 *      Author: rob
 */

#include "ch.h"
#include "hal.h"

void hc05_init(void){
  palSetPad(GPIOC, 14); // HC05 Enable off
//  palClearPad(GPIOC, 14); // HC05 Enable on
  palSetPadMode(GPIOC, 14, PAL_MODE_OUTPUT_PUSHPULL); // HC05 Enable Low active
  palSetPad(GPIOA, 1); // HC05 Off
//  palClearPad(GPIOA, 1); // HC05 On
  palSetPadMode(GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL); // HC05 On Low active
  palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_PULLUP); // HC05 State
}
