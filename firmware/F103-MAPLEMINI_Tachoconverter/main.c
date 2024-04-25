/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include "string.h"
#include "stdint.h"
#include "stdlib.h" //to get rid of compiler warnings
#include "ch.h"
#include "hal.h"
#include "chprintf.h" //to get rid of compiler warnings
#include "shell.h"
#include "comm.h"
#include "table.h"
#include "i2c_local.h"
#include "eeprom.h"
//################################################################################################
//                                  Static Variables
#define ICPCLKFREQ 18000000
uint16_t real_freq, f_enable = 1;
float speed_in;
uint16_t cnt;
//icucnt_t last_width;
uint32_t last_period;
configunion_t cudata = {
        .configstruct.ver = CONFIG_VERSION,
        .configstruct.chksm = CHECKSUM,
        .configstruct.table0.t_length = 23,
        .configstruct.table0.in[0] = 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220,
        .configstruct.table0.out[0] = 48000, 24100, 12000, 8180, 6040, 4810, 3990, 3430, 2990, 2650, 2380, 2140, 1960, 1810, 1677, 1565, 1470, 1385, 1308, 1240, 1175, 1122, 1071


};
/*
configunion_t cudata = {
        .configstruct.table0.t_length = 23,
        .configstruct.table0.in[0] = 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220,
        .configstruct.table0.out[0] = 48000, 24100, 12000, 8180, 6040, 4810, 3990, 3430, 2990, 2650, 2380, 2140, 1960, 1810, 1677, 1565, 1470, 1385, 1308, 1240, 1175, 1122, 1071,
        .configstruct.corr = 1050
};
*/
//################################################################################################
//                                  Function declarations
void PWM_Off(void);
void ch_fSpeed(uint16_t speed);
//################################################################################################

static PWMConfig pwmcfg = {
  ICPCLKFREQ,                              // 18MHz initial clock frequency.
  1800,                                    // Initial PWM period 100uS (10kHz).
  NULL,
  {
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0,
#if STM32_PWM_USE_ADVANCED
  0
#endif
};

static void icuperiodcb(ICUDriver *icup) {
    //float temp = 2*ICPCLKFREQ/4.42;
    //static uint16_t cnt;
    //last_period += (uint32_t)icuGetPeriodX(icup);
  last_period = (float)icuGetPeriodX(icup);
    cnt++;
    /*if (cnt >= 2){
        cnt = 0;
        real_freq = (uint16_t)(temp/last_period);
        last_period = 0;
    }*/
}

static void icuoverflowcb(ICUDriver *icup) {
    (void) icup;
    //real_freq = 0;
    speed_in = 0;
    cnt = 0;
}
static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_LOW,
  100000,                              // 500kHz initial ICU clock frequency.
  NULL,
  icuperiodcb,
  icuoverflowcb,
  ICU_CHANNEL_2,
  0,
  0xFFFFFFFFU
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/
#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
char history_buffer[8*64];
static const ShellCommand commands[] = {
  {"vstat", cmd_status},
  {"mod", cmd_modify},
  {"stl", cmd_stl},
  {"smm", cmd_smm},
  {"scf", cmd_scf},
  {"cpwm", cmd_ch_pwm},
  {"cspd", cmd_ch_speed},
  {"cfspd", cmd_ch_fspeed},
  {"cspdcf", cmd_ch_speed_cf},
  {"tspd", cmd_tune_speed},
  {"dumpvar", cmd_dumpvar},
  {"dumpee", cmd_dumpee},
  {"writee", cmd_writee},
  {"fillee", cmd_fillee},
  {"confrd", cmd_confrd},
  {"confwrt", cmd_confwrt},
  //{"ctimf", cmd_ch_tim_freq},
  {"per", cmd_per},
  {"dis", cmd_dis},
  {"dir", cmd_dir},
  {"cinpf", cmd_ch_inp_f},
  {NULL, NULL}
 };

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
  commands,
  history_buffer,
  sizeof(history_buffer)
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/
//uint32_t get_tim_freq(TIM_TypeDef * TIMP){
//      uint32_t freq, clock;
//      uint16_t psc = TIMP->PSC;
//      clock = STM32_TIMCLK1;
//      freq = clock / (psc+1);
//      return freq;
//}
//void set_tim_freq(TIM_TypeDef * TIMP, uint32_t freq){
//      uint32_t clock;
//      uint16_t psc;
//      clock = STM32_TIMCLK1;
//      psc = (clock / (freq )) - 1;
//      TIMP->PSC = psc;
//}
void update_pwm(void){
    float temp;
    temp = speed_in / cnt; //averaging
    speed_in = 0;
    cnt = 0;
    //last_period = (uint16_t) temp;
    temp *= cudata.configstruct.fs_corr;  //conversion from frequency to real speed
    temp = 2*ICPCLKFREQ / temp;
    real_freq = (uint16_t)temp;
    //ch_fSpeed((uint16_t)temp);
}
/*
 * Blinker thread.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  static uint16_t cnt = 0;
  chRegSetThreadName("blinker");
  while (true) {
      cnt++;
      if (cnt >= 5){
          palTogglePad(GPIOB, GPIOB_LED);
          cnt = 0;
      }

      //palSetPad(GPIOC, GPIOC_LED);
      //chThdSleepMilliseconds(500);
      //palClearPad(GPIOC, GPIOC_LED);
      if (f_enable > 0){
          update_pwm();
      }

      chThdSleepMilliseconds(100);
  }
}
void PWM_Off(void){
    pwmDisableChannel(&PWMD4, 3);
}
void ch_PWM_Freq(uint32_t freq){
    pwmChangePeriod(&PWMD4, freq);
    pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
}
void ch_Speed(uint16_t speed){
    //table lookup here!
    //ch_PWM_Freq(read_table_u16(&(cudata.configstruct.table0), speed));
    pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, speed));
    pwmEnableChannelNotification(&PWMD4, 2);
}
void ch_fSpeed(uint16_t speed){ // fspeed is Fake speed. It uses a correction factor (corr)
                                // to show (usually) more speed as in reality.
    uint32_t fspeed;
    fspeed = (uint32_t)(speed)*cudata.configstruct.corr;
    ch_Speed((uint16_t)(fspeed / 1000));
}
/*
 * Application entry point.
 */
int main(void) {
  thread_t *shelltp = NULL;
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE; //PB3, PB4, PA15 working, PA13, PA14 NOT
  //AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE;     //PB3, PB4, PA15, PA13, PA14 working
  AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP_0; // Partial Remap PWM2/2 to PB3

  pwmStart(&PWMD4, &pwmcfg);
  pwmEnablePeriodicNotification(&PWMD4);
  // TIM1 Outputs
  palSetPadMode(GPIOB, 8, PAL_MODE_STM32_ALTERNATE_PUSHPULL); //TIM4-CH3, Motor PWM out

  /*
   * Starts the PWM channel 1 using 30% duty cycle.
   */
  pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 3000));
  pwmEnableChannelNotification(&PWMD4, 2);
  //ch_PWM_Freq(600000);

  icuStart(&ICUD2, &icucfg);
  icuStartCapture(&ICUD2);
  icuEnableNotifications(&ICUD2);
  //palSetPadMode(GPIOB, 11, PAL_MODE_INPUT_PULLUP); //TIM2-CH4
  palSetPadMode(GPIOB, 3, PAL_MODE_INPUT_PULLUP); //TIM2-CH2
  /*
   * Activates the serial driver 2 using the driver default configuration.
   * PA2(TX) and PA3(RX) are routed to USART2.
   */
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_STM32_ALTERNATE_PUSHPULL); //UART TX
  palSetPadMode(GPIOB, 4, PAL_MODE_OUTPUT_PUSHPULL); //CW/CCW
  //palSetPad(GPIOB, 4);

  chprintf((BaseSequentialStream *)&SD2, "ChibiOS Bluepill F103 Shell v0.1\r\n");
  /*
   * Shell manager initialization.
   */
  shellInit();
  I2CInitLocal(); //Starts I2C Driver
  configInit();    //checks config version and checksum and, if OK, copies it to RAM

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched.
   */
  while (true) {
    if (!shelltp)
      shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                    "shell", NORMALPRIO + 1,
                                    shellThread, (void *)&shell_cfg1);
    else if (chThdTerminatedX(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
    chThdSleepMilliseconds(500);
  }
}
