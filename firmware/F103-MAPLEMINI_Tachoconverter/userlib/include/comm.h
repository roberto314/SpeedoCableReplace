/*
 * comm.h
 *
 *  Created on: 04.02.2018
 *      Author: Anwender
 */

#ifndef USERLIB_INCLUDE_COMM_H_
#define USERLIB_INCLUDE_COMM_H_

#include "myStringfunctions.h"
#define cli_println(a); chprintf((BaseSequentialStream *)&SD2, a"\r\n");
#define OK(); do{cli_println(" ... OK"); chThdSleepMilliseconds(20);}while(0)

void cmd_status(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_modify(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_ch_pwm(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_ch_speed(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_ch_fspeed(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_tune_speed(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_ch_speed_cf(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_scorr(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_stl(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_smm(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_scf(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_dumpee(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_writee(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_dumpvar(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_fillee(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_confrd(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_confwrt(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_ch_tim_freq(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_per(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_dis(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_dir(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_ch_inp_f(BaseSequentialStream *chp, int argc, char *argv[]);


#endif /* USERLIB_INCLUDE_COMM_H_ */
