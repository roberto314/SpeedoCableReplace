/*
 * table.h
 *
 *  Created on: 18.03.2018
 *      Author: Anwender
 */

#ifndef USERLIB_INCLUDE_TABLE_H_
#define USERLIB_INCLUDE_TABLE_H_
#include "ch.h"
#include "hal.h"

#define TABLE_LENGTH 32
typedef struct{
  uint16_t t_length; // Number of data points
  uint16_t in[TABLE_LENGTH];        // Array of x-coordinates
  uint16_t out[TABLE_LENGTH];       // Array of y-coordinates
} table_1d_u16_t ;
typedef struct{
  uint16_t t_length; // Number of data points
  float in[TABLE_LENGTH];        // Array of x-coordinates
  float out[TABLE_LENGTH];       // Array of y-coordinates
} table_1d_float_t ;

float read_table_float(table_1d_float_t * corr_tab, float in);
uint16_t read_table_u16(table_1d_u16_t * corr_tab, uint16_t in);
void set_table_length(table_1d_u16_t * corr_tab, uint16_t tl);
void set_table_minmax_float(table_1d_float_t * corr_tab, float min, float max);
void set_table_minmax_u16(table_1d_u16_t * corr_tab, uint16_t min, uint16_t max);
void set_table_cf_float(table_1d_float_t * corr_tab, uint16_t pos, float cf);
void set_table_cf_u16(table_1d_u16_t * corr_tab, uint16_t pos, uint16_t cf);
uint16_t get_pos_float(table_1d_float_t * table, float x);
uint16_t get_pos_u16(table_1d_u16_t * table, uint16_t x);

#endif /* USERLIB_INCLUDE_TABLE_H_ */
