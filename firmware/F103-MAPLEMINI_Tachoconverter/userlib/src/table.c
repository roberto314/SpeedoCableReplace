/*
 * table.c
 *
 *  Created on: 18.03.2018
 *      Author: Anwender
 */

#include "table.h"
/* Declare variable using above structure and the function datapoints */
/* These coordinates correspond to the points illustrated in the above graph */
//static float sine_x[] = {0.0, 0.24, 0.64, 1.03, 1.43, M_PI_2};
//static float sine_y[] = {0.0, 0.24, 0.6,  0.87, 1.00, 1.00};
/*
static float in[16]={ 0,   10,  20,  30,
                      40,  50,  60,  70,
                      80,  90, 100, 110,
                     120, 130, 140, 150};
static float out[16]={  1.0, 0.85, 0.92, 0.95,  //Fallback Values
                       0.97, 0.99,  1.0,  1.0,
                        1.0,  1.0,  1.0,  1.0,
                        1.0,  1.0,  1.0,  1.0
                     };
*/
/******************************************************************************
* Returns the nearest position of the y-value.
* Saturates to 0 or LENGTH if x outside interval [x0, xmax] and returns value pos | 0xFF00.
*/
uint16_t get_pos_float(table_1d_float_t * table, float x){ //finds the nearest position,
  // if return value is > 0xFF input value rails at max or min
  uint16_t index;
  // First check boundaries
  if (x >= (table->in[table->t_length-1])) {
     /* x-value too large, saturate to max y-value */
      return (uint16_t)((table->t_length-1) | 0xFF00);
  }
  else if (x <= (table->in[0])) {
     /* x-value too small, saturate to min y-value */
      return 0xFF00;
  }
  /* Find the nearest segment that holds x */
  for (index = 0; index<(table->t_length-1); index++){
      if ((table->in[index] <= x) && (table->in[index+1] >= x)){
        float middle = (table->in[index] + table->in[index + 1]) / 2;
        if (table->in[index] < table->in[index + 1]){ //rising slope
          if (x < middle) return index;
          else return index + 1;
        }
        else{                                        // falling slope
          if (x > middle) return index;
          else return index + 1;
        }
      }
  }
  return table->t_length-1;
}
uint16_t get_pos_u16(table_1d_u16_t * table, uint16_t x){ //finds the nearest position,
  // if return value is > 0xFF input value rails at max or min
  uint16_t index;
  // First check boundaries
  if (x >= (table->in[table->t_length-1])) {
     /* x-value too large, saturate to max y-value */
      return (uint16_t)((table->t_length-1));
  }
  else if (x <= (table->in[0])) {
     /* x-value too small, saturate to min y-value */
      return 0;
  }
  /* Find the nearest segment that holds x */
  for (index = 0; index<(table->t_length-1); index++){
      if ((table->in[index] <= x) && (table->in[index+1] >= x)){
        float middle = (table->in[index] + table->in[index + 1]) / 2;
        if (table->in[index] < table->in[index + 1]){ //rising slope
          if (x < middle) return index;
          else return index + 1;
        }
        else{                                        // falling slope
          if (x > middle) return index;
          else return index + 1;
        }
      }
  }
  return table->t_length-1;
}
/******************************************************************************
* Returns the interpolated y-value.
* Saturates to y0 or y1 if x outside interval [x0, x1].
*/
float interpolate_segment_float(float x0, float y0, float x1, float y1, float x){
    float t;

    if (x <= x0) { return y0; }
    if (x >= x1) { return y1; }

    t =  (x-x0);
    t /= (x1-x0);

    return y0 + t*(y1-y0);
}
uint16_t interpolate_segment_u16(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x){
  float t;

    if (x <= x0) { return y0; }
    if (x >= x1) { return y1; }

    t =  ((float)x-(float)x0);
    t /= ((float)x1-(float)x0);

    return (uint16_t)((float)y0 + t*((float)y1-(float)y0));
}
/******************************************************************************
*
*
*/
uint8_t get_idx_float(table_1d_float_t * table, float x){
  uint16_t index;

  /* Find the nearest segment that holds x */
  for (index = 0; index<(table->t_length-1); index++){
      if ((table->in[index] <= x) && (table->in[index+1] >= x)){
        return index;
      }
  }
  /* Something with the data was wrong if we get here */
  /* Saturate to the max value */
  return table->t_length-1;
}
uint8_t get_idx_u16(table_1d_u16_t * table, uint16_t x){
  uint16_t index;

  /* Find the nearest segment that holds x */
  for (index = 0; index<(table->t_length-1); index++){
      if ((table->in[index] <= x) && (table->in[index+1] >= x)){
        return index;
      }
  }
  /* Something with the data was wrong if we get here */
  /* Saturate to the max value */
  return table->t_length-1;
}

/******************************************************************************
*
*
*/
float read_table_float(table_1d_float_t * table, float in){
  uint16_t idx; //
  // First check boundaries
  if (in >= (table->in[table->t_length-1])){ // in-value too large, saturate to max y-value
    return table->out[table->t_length-1];
  }
  else if (in <= (table->in[0])){            // in-value too small, saturate to min y-value
    return table->out[0];
  }
  // Now we can get to business
  idx = get_idx_float(table, in);
  return interpolate_segment_float(table->in[idx],    /* x0 */
         table->out[idx],                     /* y0 */
         table->in[idx+1],                    /* x1 */
         table->out[idx+1],                   /* y1 */
         in);                                  /* x  */
}

uint16_t read_table_u16(table_1d_u16_t * table, uint16_t in){
  uint16_t idx; //
  // First check boundaries
  if (in >= (table->in[table->t_length-1])){ // in-value too large, saturate to max y-value
    return table->out[table->t_length-1];
  }
  else if (in <= (table->in[0])){            // in-value too small, saturate to min y-value
    return table->out[0];
  }
  // Now we can get to business
  idx = get_idx_u16(table, in);
  return interpolate_segment_u16(table->in[idx],    /* x0 */
         table->out[idx],                     /* y0 */
         table->in[idx+1],                    /* x1 */
         table->out[idx+1],                   /* y1 */
         in);                                  /* x  */
}

void set_table_length(table_1d_u16_t * corr_tab, uint16_t tl){
  corr_tab->t_length = tl;
}


void set_table_minmax_float(table_1d_float_t * corr_tab, float min, float max){
  int i;
  float step = (max - min) / (corr_tab->t_length - 1);
  //corr_tab.in[0] = min;
  //corr_tab.in[corr_tab.t_length - 1] = max;
  for (i=0; i < corr_tab->t_length; i++){
    corr_tab->in[i] = min + i * step;
  }
}
void set_table_minmax_u16(table_1d_u16_t * corr_tab, uint16_t min, uint16_t max){
  int i;
  float step = (max - min) / (corr_tab->t_length - 1);
  //corr_tab.in[0] = min;
  //corr_tab.in[corr_tab.t_length - 1] = max;
  for (i=0; i < corr_tab->t_length; i++){
    corr_tab->in[i] = min + i * step;
  }
}

void set_table_cf_float(table_1d_float_t * corr_tab, uint16_t pos, float cf){
  corr_tab->out[pos] = cf;
}
void set_table_cf_u16(table_1d_u16_t * corr_tab, uint16_t pos, uint16_t cf){
  corr_tab->out[pos] = cf;
}
