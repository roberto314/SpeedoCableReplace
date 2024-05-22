/*
 * comm.c
 *
 *  Created on: 04.02.2018
 *      Author: Anwender
 */



#include "ch.h"
#include "hal.h"
#include "comm.h"

#include "chprintf.h"
#include "stdlib.h"
#include "string.h" /* for memset */
#include "shell.h"
#include "table.h"
#include "eeprom.h"
#include "i2c_local.h"
#include "main.h"

uint16_t speed;
uint16_t one = 60000;
int16_t two = -22;
uint32_t three = 0xffffffff;
int32_t four = -645599;
extern configunion_t cudata;
extern I2CEepromFileStream eeFile;
extern EepromFileStream *eeFS;
extern uint16_t real_freq, f_enable;
extern uint8_t direction;
extern icucnt_t last_period;

void update_values(uint16_t * v){
  ch_Speed(*v);
}
/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

//#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE(256)
 void shellModifyu16(BaseSequentialStream *chp, uint16_t * modify_p) {
  chprintf(chp, "Welcome to modify u16\r\n");
  chprintf(chp, "press '+' to increase one, '-' to decrease one \r\n");
  chprintf(chp, "'D' to increase 10, 'd' to decrease 10 \r\n");
  chprintf(chp, "'C' to increase 100, 'c' to decrease 100 \r\n");
  chprintf(chp, "'M' to increase 1000, 'm' to decrease 1000 \r\n");
  chprintf(chp, "'*' to multiply with 10, '/' to divide by 10 \r\n");
  chprintf(chp, "press 'x' to exit \r\n");

  chprintf(chp, "Status of var is: %u\r\n", *modify_p);
  while (true) {
    char c;

    if (streamRead(chp, (uint8_t *)&c, 1) == 0)
      chThdSleepMilliseconds(250);

    if (c == '+') {
      (*modify_p)++;
    }
    if (c == '-') {
      (*modify_p)--;
    }
    if (c == 'D') {
      *modify_p += 10;
    }
    if (c == 'd') {
      *modify_p -= 10;
    }
    if (c == 'C') {
      *modify_p += 100;
    }
    if (c == 'c') {
      *modify_p -= 100;
    }
    if (c == 'M') {
       *modify_p += 1000;
     }
     if (c == 'm') {
       *modify_p -= 1000;
     }
    if (c == '*') {
      *modify_p *= 10;
    }
    if (c == '/') {
      *modify_p /= 10;
    }
    if (c == 'x') {
      chprintf(chp, "EXIT\r\n");
      return;
    }
    if (c < 0x20)
      continue;
    chprintf(chp, "%u\r\n", *modify_p);
    update_values(modify_p);
  }
}
void shellModifys16(BaseSequentialStream *chp, int16_t * modify_p) {
  chprintf(chp, "Welcome to modify s16\r\n");
  chprintf(chp, "press '+' to increase one, '-' to decrease one \r\n");
  chprintf(chp, "'u' to increase 10, 'd' to decrease 10 \r\n");
  chprintf(chp, "'U' to increase 100, 'D' to decrease 100 \r\n");
  chprintf(chp, "'*' to multiply with 10, '/' to divide by 10 \r\n");
  chprintf(chp, "press 'x' to exit \r\n");

  chprintf(chp, "Status of var is: %d\r\n", *modify_p);
  while (true) {
    char c;

    if (streamRead(chp, (uint8_t *)&c, 1) == 0)
      chThdSleepMilliseconds(250);

    if (c == '+') {
      (*modify_p)++;
    }
    if (c == '-') {
      (*modify_p)--;
    }
    if (c == 'u') {
      *modify_p += 10;
    }
    if (c == 'd') {
      *modify_p -= 10;
    }
    if (c == 'U') {
      *modify_p += 100;
    }
    if (c == 'D') {
      *modify_p -= 100;
    }
    if (c == '*') {
      *modify_p *= 10;
    }
    if (c == '/') {
      *modify_p /= 10;
    }
    if (c == 'x') {
      chprintf(chp, "EXIT\r\n");
      return;
    }
    if (c < 0x20)
      continue;
    chprintf(chp, "%d\r\n", *modify_p);
  }
}
void shellModifyu32(BaseSequentialStream *chp, uint32_t * modify_p) {
  chprintf(chp, "Welcome to modify u32\r\n");
  chprintf(chp, "press '+' to increase one, '-' to decrease one \r\n");
  chprintf(chp, "'u' to increase 10, 'd' to decrease 10 \r\n");
  chprintf(chp, "'U' to increase 100, 'D' to decrease 100 \r\n");
  chprintf(chp, "'*' to multiply with 10, '/' to divide by 10 \r\n");
  chprintf(chp, "press 'x' to exit \r\n");

  chprintf(chp, "Status of var is: %u\r\n", *modify_p);
  while (true) {
    char c;

    if (streamRead(chp, (uint8_t *)&c, 1) == 0)
      chThdSleepMilliseconds(250);

    if (c == '+') {
      (*modify_p)++;
    }
    if (c == '-') {
      (*modify_p)--;
    }
    if (c == 'u') {
      *modify_p += 10;
    }
    if (c == 'd') {
      *modify_p -= 10;
    }
    if (c == 'U') {
      *modify_p += 100;
    }
    if (c == 'D') {
      *modify_p -= 100;
    }
    if (c == '*') {
      *modify_p *= 10;
    }
    if (c == '/') {
      *modify_p /= 10;
    }
    if (c == 'x') {
      chprintf(chp, "EXIT\r\n");
      return;
    }
    if (c < 0x20)
      continue;
    chprintf(chp, "%u\r\n", *modify_p);
  }
}
void shellModifys32(BaseSequentialStream *chp, int32_t * modify_p) {
  chprintf(chp, "Welcome to modify s32\r\n");
  chprintf(chp, "press '+' to increase one, '-' to decrease one \r\n");
  chprintf(chp, "'u' to increase 10, 'd' to decrease 10 \r\n");
  chprintf(chp, "'U' to increase 100, 'D' to decrease 100 \r\n");
  chprintf(chp, "'*' to multiply with 10, '/' to divide by 10 \r\n");
  chprintf(chp, "press 'x' to exit \r\n");

  chprintf(chp, "Status of var is: %d\r\n", *modify_p);
  while (true) {
    char c;

    if (streamRead(chp, (uint8_t *)&c, 1) == 0)
      chThdSleepMilliseconds(250);

    if (c == '+') {
      (*modify_p)++;
    }
    if (c == '-') {
      (*modify_p)--;
    }
    if (c == 'u') {
      *modify_p += 10;
    }
    if (c == 'd') {
      *modify_p -= 10;
    }
    if (c == 'U') {
      *modify_p += 100;
    }
    if (c == 'D') {
      *modify_p -= 100;
    }
    if (c == '*') {
      *modify_p *= 10;
    }
    if (c == '/') {
      *modify_p /= 10;
    }
    if (c == 'x') {
      chprintf(chp, "EXIT\r\n");
      return;
    }
    if (c < 0x20)
      continue;
    chprintf(chp, "%d\r\n", *modify_p);
  }
}
void cmd_status(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)* argv;
  chprintf(chp, "Status of Variables is: %u %d %u %d\r\n", one, two, three, four);
}
void cmd_modify(BaseSequentialStream *chp, int argc, char *argv[]) {
  const char * const usage = "Usage: ch NUMBER\r\n";

  if (argc != 1) {
    chprintf(chp, usage);
    return;
  }
  //uint16_t nr = (uint16_t)atoi(argv[0]);

  switch (atoi(argv[0])){
  case 0:
    shellModifyu16(chp, &one);
    break;
  case 1:
    shellModifys16(chp, &two);
    break;
  case 2:
    shellModifyu32(chp, &three);
   break;
  case 3:
    shellModifys32(chp, &four);
    break;
  default:
    chprintf(chp, "something went wrong\r\n");
    break;
  }
}
void cmd_stl(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  uint8_t chan;
  if (argc != 1) {
    chprintf(chp, "Set Table Lentgh: stl LENGTH(1..32) \r\n");
    for (chan = 0; chan < 1; chan++){
      chprintf(chp, "Table Length is now: %2d\r\n", cudata.configstruct.table0.t_length);
    }
    return;
  }
  uint8_t tl = atoi(argv[0]);
  if (tl > TABLE_LENGTH){
	  chprintf(chp, "Table Lentgh too big! Max. LENGTH = 32\r\n");
	  return;
  }
  set_table_length(&cudata.configstruct.table0, tl);
  chprintf(chp, "Length is changed to: %2d\r\n", cudata.configstruct.table0.t_length);

}
void cmd_smm(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  //table_1d_u16_t * corr_tab = get_table_data();
  uint8_t chan, i, length;
  if (argc != 2) {
    chprintf(chp, "Set Table Min and Max Val: smm MIN MAX \r\n");

    for (chan = 0; chan < 1; chan++){
      length = cudata.configstruct.table0.t_length;
      chprintf(chp, "Length is: %d\r\n", length);
      for (i = 0; i < length; i++){
        chprintf(chp, "%d ", (uint32_t)(cudata.configstruct.table0.in[i]*1));
      }
      chprintf(chp, "\r\n");
    }
    return;
  }
  uint16_t min = atoi(argv[0]);
  uint16_t max = atoi(argv[1]);
  set_table_minmax_u16(&cudata.configstruct.table0, min, max);
  length = cudata.configstruct.table0.t_length;
  chprintf(chp, "Data is: \r\n");
  for (i = 0; i < length; i++){
    chprintf(chp, "%3d ", (uint32_t)(cudata.configstruct.table0.in[i]*1));
  }
  chprintf(chp, "\r\n");
}
void cmd_scf(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  uint8_t i, length;
  uint16_t cf;
  //table_1d_u16_t * corr_tab = get_table_data();
  uint16_t pos = get_pos_u16(&cudata.configstruct.table0, speed);
  if ((argc != 2) && (argc != 1)) {
    chprintf(chp, "Set Corr. Factor at pos: scf POS CORR (0 <= cf < 65535)\r\n");
    chprintf(chp, "OR Set Corr. Factor at current pos: scf CORR (0 <= cf < 65535)\r\n");
    chprintf(chp, "Change pos with 'cspd' or 'tspd' \r\n");
    length = cudata.configstruct.table0.t_length;
    chprintf(chp, "Corr. Factors are now: ");
    for (i = 0; i < length; i++){
      chprintf(chp, "%3d ", (uint32_t)((cudata.configstruct.table0.out[i])*1));
    }
    chprintf(chp, "\r\n");
    chprintf(chp, "Value at pos. %d is now: %d\r\n", pos, cudata.configstruct.table0.out[pos]);
    return;
  }
  else if (argc == 1){
   length = cudata.configstruct.table0.t_length;
    cf = atoi(argv[0]);
    set_table_cf_u16(&cudata.configstruct.table0, pos, cf);
    chprintf(chp, "Corr. Factor is: ");
    for (i = 0; i < length; i++){
      chprintf(chp, "%3d ", (uint32_t)((cudata.configstruct.table0.out[i])*1));
    }
    chprintf(chp, "\r\n");
    return;
  }

  else if (argc == 2){
    length = cudata.configstruct.table0.t_length;
    pos = atoi(argv[0]);
    cf = atoi(argv[1]);
    set_table_cf_u16(&cudata.configstruct.table0, pos, cf);
    chprintf(chp, "Corr. Factor is: ");
    for (i = 0; i < length; i++){
      chprintf(chp, "%3d ", (uint32_t)((cudata.configstruct.table0.out[i])*1));
    }
    chprintf(chp, "\r\n");
    return;
  }
}
void cmd_tune_speed(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  uint16_t cf;
  char c;
  uint16_t pos = get_pos_u16(&cudata.configstruct.table0, speed);

  if (argc == 1){
    speed = atoi(argv[0]);
    chprintf(chp, "Changed speed to: %d\r\n", speed);
  }
  else {
	  chprintf(chp, "Speed is now: %d\r\n", speed);
  }
  if (speed == 0){
    chprintf(chp, "Set PWM Output in table at current pos: tspd \r\n");
    chprintf(chp, "Speed seems to be zero. Please enter desired speed with 'cspd XXX' or 'tspd XXX' \r\n");
    chprintf(chp, "Please enter desired SPEED Output (should be a Value in the Table! - Check with 'smm 0')\r\n");
    return;
  }
  pos = get_pos_u16(&cudata.configstruct.table0, speed);
new_loop:
  chprintf(chp, "Pos. is now: %2d\r\n", pos);
  cf = cudata.configstruct.table0.out[pos];
  chprintf(chp, "Speed at pos. is now: %d\r\n", cudata.configstruct.table0.in[pos]);
  chprintf(chp, "Value at pos. is now: %d\r\n", cf);
  update_values(&cf);
  shellModifyu16(chp, &cf);
  set_table_cf_u16(&cudata.configstruct.table0, pos, cf);
  chprintf(chp, "Corr. Factor is: %d\r\n", cf);
  if (pos < cudata.configstruct.table0.t_length - 1){
	  chprintf(chp, "Value at next pos. is: %d\r\n", cudata.configstruct.table0.out[pos+1]);
	  chprintf(chp, "Would you like to tune the next value? [y,n] \r\n");
	  while (true) {
		  if (streamRead(chp, (uint8_t *)&c, 1) == 0)
		  	  chThdSleepMilliseconds(250);
		  if ((c == 'n') || (c == 'N')) {
		      chprintf(chp, "EXIT\r\n");
		      return;
		  }
		  if ((c == 'y') || (c == 'Y')) {
              pos ++;
              speed = cudata.configstruct.table0.in[pos];
              goto new_loop;
		  }
	  }
  }
}
void cmd_ch_pwm(BaseSequentialStream *chp, int argc, char *argv[]){
	(void)argv;
	  if (argc != 1) {
	    chprintf(chp, "Changes Freq. of PWM Output to Speedometer \r\n");
	    return;
	  }
	  ch_PWM_Freq(atoi(argv[0]));
}
void cmd_ch_speed(BaseSequentialStream *chp, int argc, char *argv[]){
	(void)argv;
	  if (argc != 1) {
	    chprintf(chp, "Changes Speed on Speedometer \r\n");
	    chprintf(chp, "Speed is now: %d", speed);
	    return;
	  }
	  speed = atoi(argv[0]);
	  ch_Speed(speed);
}
void cmd_ch_fspeed(BaseSequentialStream *chp, int argc, char *argv[]){
	(void)argv;
	  if (argc != 1) {
	    chprintf(chp, "Changes Fake Speed on Speedometer (Speed with 'speed correction factor') \r\n");
	    return;
	  }
	  ch_fSpeed(atoi(argv[0]));
}
void cmd_ch_speed_cf(BaseSequentialStream *chp, int argc, char *argv[]){
	(void)argv;
	//uint16_t scf;
	if (argc == 0){
	  chprintf(chp, "Set Corr. Factor for Speed (0 <= cf < 65535)\r\n");
	  chprintf(chp, "1000 equals 100 Percent\r\n");
	  chprintf(chp, "Factor is now: %2d\r\n", cudata.configstruct.corr);
	  return;
	}
	cudata.configstruct.corr = atoi(argv[0]);

}
void cmd_dumpee(BaseSequentialStream *chp, int argc, char *argv[]){
  (void)argc;
  (void)* argv;
  uint16_t i, adr, sz;
  uint8_t buf[128];
  adr = 0;
  sz = 128;
  chprintf(chp, "Dumps 128 Byte of EEPROM at Address. Usage: dumpee [addr]\r\n");
  adr = atoi(argv[0]);
  chprintf(chp, "Starting at Address: %d\r\n", adr);
  fileStreamSetPosition(eeFS, adr);
  memset(buf, 0, 128);
  chprintf(chp, "size = %u, position = %u, barrier_low = %u, barrier_hi = %u",
          sz,
          eepfs_getposition(eeFS),
          eeFile.cfg->barrier_low,
          eeFile.cfg->barrier_hi);
  cli_println("");
  fileStreamRead(eeFS, buf, 128);
  for (i=0; i<sz; i++){
    if (i%16 == 0){
    	//cli_println("");
    	chprintf(chp, " pos = %u\r\n", adr+i);
    }
    chprintf(chp, "%02x ", buf[i]);
    //
  }
  cli_println("");
  //fileStreamClose(eeFS);
  chprintf(chp, "size = %u, position = %u, barrier_low = %u, barrier_hi = %u",
          sz,
          eepfs_getposition(eeFS),
          eeFile.cfg->barrier_low,
          eeFile.cfg->barrier_hi);
    cli_println("");
  //chThdSleepMilliseconds(100);
}
void cmd_writee(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)* argv;
  int16_t pos;
  chprintf(chp, "Writes to EEPROM\r\n");
  chprintf(chp, "Usage: writee ADDRESS[0..4095] DATA[0..0xff]\r\n");
  if (argc != 2)
    return;
  pos = eepfs_lseek(&eeFile, atoi(argv[0]));
  if (pos != eepfs_getposition(&eeFile)){
    chprintf(chp, "EEPROM fseek Error\r\n");
    return;
  }
  chprintf(chp, "Pos: %d\r\n", pos);
  EepromWriteByte(eeFS, (uint8_t)strtol(argv[1], NULL, 16));
  chprintf(chp, "Written to EEPROM\r\n");
}
void cmd_dumpvar(BaseSequentialStream *chp, int argc, char *argv[]){
  (void)argc;
  (void)* argv;
  uint16_t szc;                              //data size complete
  uint16_t i;

  chprintf(chp, "Dumps Struct Variables\r\n");

  szc = sizeof(configstruct_t) / sizeof(uint8_t);

  chprintf((BaseSequentialStream *)&SD2, "size complete: %d\r\n", szc);
  chprintf((BaseSequentialStream *)&SD2, "%d pages\r\n", szc/EEPROM_PAGE_SIZE);
  chprintf((BaseSequentialStream *)&SD2, "%d rest\r\n", szc%EEPROM_PAGE_SIZE);

  for (i = 0; i<szc; i++){
	  if (i%16 == 0){
	    //cli_println("");
	    chprintf(chp, " pos = %u\r\n", i);
	  }
	  chprintf((BaseSequentialStream *)&SD2, "%02x ", cudata.configarray[i]);
  }
  cli_println("");
  chThdSleepMilliseconds(100);
}
void cmd_fillee(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)* argv;
  int16_t pos;
  uint16_t cnt, i;
  uint8_t data;
  chprintf(chp, "Fills EEPROM from ADDR with COUNT * DATA with val\r\n");
  chprintf(chp, "Usage: fillee ADDRESS[0..4095] COUNT DATA[0..0xff]\r\n");
  if (argc != 3)
    return;
  pos = eepfs_lseek(&eeFile, atoi(argv[0]));
  cnt = (uint16_t)strtol(argv[1], NULL, 0);
  data = (uint8_t)strtol(argv[2], NULL, 0);
  chprintf(chp, "Start writing %d at Pos: %d for cnt: %d\r\n", data, pos, cnt);
  if (pos != eepfs_getposition(&eeFile)){
    chprintf(chp, "EEPROM fseek Error\r\n");
    return;
  }
  for (i=0; i<cnt; i++){
    eepfs_lseek(&eeFile, pos++);
    EepromWriteByte(eeFS, data);
  }

  chprintf(chp, "Written to EEPROM\r\n");
}
void cmd_confrd(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)* argv;
  chprintf(chp, "Reads EEPROM and initializes config data\r\n");
  chprintf(chp, "Usage: confrd\r\n");
  configParameterRead();
}
void cmd_confwrt(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)* argv;
  chprintf(chp, "Writes config data to EEPROM\r\n");
  chprintf(chp, "Usage: confwrt\r\n");
  configParameterWrite();
}
//void cmd_ch_tim_freq(BaseSequentialStream *chp, int argc, char *argv[]){
//  (void)argc;
//  uint16_t dtim;
//  uint32_t freq;
//  TIM_TypeDef * TIMP;
//  chprintf(chp, "Changes or reads Timer frequency (in kHz) of input and output timer\r\n");
//  chprintf(chp, "Usage: ctimf TIMER[0,1] FREQ(in kHz)\r\n");
//  chprintf(chp, "Input Timer Frequency is now: %dkHz, clock: %dkHz, Prescaler: %d\r\n", get_tim_freq(TIM3)/1000, STM32_TIMCLK1/1000, TIM3->PSC);
//  chprintf(chp, "Output Timer Frequency is now: %dkHz, clock: %dkHz, Prescaler: %d\r\n", get_tim_freq(TIM4)/1000, STM32_TIMCLK1/1000, TIM4->PSC);
//  if (argc == 2){
//	  freq = ((uint32_t)atoi(argv[1])) * 1000;
//	  if (atoi(argv[0]) == 0){
//		  TIMP = TIM3;
//		  cudata.configstruct.in_freq = freq;
//		  dtim = 3;
//	  }
//	  else{
//		  TIMP = TIM4;
//		  cudata.configstruct.out_freq = freq;
//		  dtim = 4;
//	  }
//	  set_tim_freq(TIMP, freq);
//	  chprintf(chp, "Changed frequency of TIM%d to: %dkHz, Freq input: %dkHz\r\n", dtim, get_tim_freq(TIMP)/1000, freq/1000);
//  }
//}
void cmd_per(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argc;
  (void )* argv;
  //uint16_t st;
  chprintf(chp, "Gets Period\r\n");

  chprintf(chp, "Period is now: %d, Speed: %d\r\n", last_period, real_freq);
}
void cmd_dis(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argc;
  (void )* argv;
  uint16_t dis;
  chprintf(chp, "Disables or Enables update of Output frequency\r\n");
  chprintf(chp, "Usage: dis[0,1] where 0 == Disable and 1 == Enable\r\n");
  dis = atoi(argv[0]);
  if (dis < 2){
	  chprintf(chp, "Updated Enable Flag. Data was: %d\r\n", dis);
	  f_enable = dis;
  }
}
void cmd_dir(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argc;
  (void )* argv;
  uint16_t dir;
  if (argc == 0){
    chprintf(chp, "Changes direction of Motor\r\n");
    chprintf(chp, "Usage: dir[0,1] where 0 == CW and 1 == CCW\r\n");
    chprintf(chp, "Dir is now: %d\r\n", direction);
    return;
  }
  dir = atoi(argv[0]);
  if (dir == 0){
    direction = 0;
    chprintf(chp, "Direction changed to: %d\r\n", dir);
    palClearPad(GPIOB, 4);
  }
  else if (dir == 1){
    direction = 1;
    chprintf(chp, "Direction changed to: %d\r\n", dir);
    palSetPad(GPIOB, 4);
  }
}
void cmd_ch_inp_f(BaseSequentialStream *chp, int argc, char *argv[]){
	(void)argv;
	//uint16_t scf;
	float inputf;
	char pps_str[26];
	memset(&pps_str, 0, sizeof(pps_str));

	if (argc == 0){
	  chprintf(chp, "Set Factor for Input frequency to Speed (float accuracy)\r\n");
	  //chprintf(chp, "1000 equals 100 Percent\r\n");
	  inputf = cudata.configstruct.fs_corr;
	  myftoap(pps_str, inputf, 9);
	  chprintf(chp, "Factor is now: %s\r\n", pps_str);
	  return;
	}
	cudata.configstruct.fs_corr = patof(argv[0]);

}
