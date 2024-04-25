/*
 * myStringfunctions.c
 *
 *  Created on: 07.01.2018
 *      Author: rob
 */

#include <stdlib.h> //to get rid of compiler warnings
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "myStringfunctions.h"

// convert float to string one decimal digit at a time
// *buf is the output string. Size must be fsz+1!
// fltval is the input
// fsz (final string size < 10) is the number of characters in the output (if size > actual number
// tabing characters are inserted), if number is too big for the output the
// OVF String is given
//#define ZERO_NEEDED 1 // comment for .3456 instead 0.345
#define DP '.' //Decimal Point
#define TC ' '
#define OVF "OVF" //Overflow string
#define MAX "MSZ" //final size > maximal size
#define MIN "MIN"
#define NEG '-'   //mandatory
//#define POS '+' //optional, comment if not needed
#define MAXSZ 10  // 10digits maximum
//void myftoap(char *buf, double fltval, uint8_t fsz, BaseSequentialStream *chp){
void myftoap(char *buf, double fltval, uint8_t fsz){
  uint8_t pos = 0;  // position in string
  uint8_t len;      // length of decimal part of result
  uint8_t checkagain = 1;      //
  uint32_t intval, temp;       // decimal digit(s) to convert
  const uint32_t sz[MAXSZ] = {1,     //0  10^0
                          10,        //1  10^1
                          100,       //2  10^2
                          1000,      //3  10^3
                          10000,     //4  10^4
                          100000,    //5  10^5
                          1000000,   //6  10^6
                          10000000,  //7  10^7
                          100000000, //8  10^8
                          1000000000};//9 10^9
  const double rounders[MAXSZ] = {
      0.5,                // 0
      0.05,               // 1
      0.005,              // 2
      0.0005,             // 3
      0.00005,            // 4
      0.000005,           // 5
      0.0000005,          // 6
      0.00000005,         // 7
      0.000000005,        // 8
      0.0000000005        // 9
  };
  if (fsz >= MAXSZ){      //specified too many digits
    memcpy(buf,MAX,sizeof(MAX));
    return;
  }
  if (fltval < 0 ){  // handle negative numbers
    fltval *= -1;
    buf[pos++]=NEG;
    fsz--;
  }
  else{
#ifdef POS
    buf[pos++]=POS;
    fsz--;
#endif
  }
  // OK. We have space for fsz digits. Lets assume we have fsz=4 and a value of
  // 99.99. If we truncate right away we come up with 99.9 which is not really correct.
  // We need to round the value but where?
  // I came up with the following solution:

CHECKAGAIN:
  intval = (uint32_t)fltval;  // truncate the floating point number (intval=99)
  //chprintf(chp, "intval: %d\r\n", intval);
  //chprintf(chp, "fsz now(1): %d\r\n", fsz);
  if (intval >= sz[MAXSZ-1]){ //more than 9 digits is not supported
    memcpy(buf,MAX,sizeof(MAX));
    return;
  }
  for (len=0; len < MAXSZ; len++){  //find out how many digits we need
    if (intval < sz[len]){
      break;
    }
  }
  if (len > fsz){  //we can't handle this number
    memcpy(buf,OVF,sizeof(OVF));
    return;
  }
  //chprintf(chp, "needed digits: %d\r\n", len);
  // Now we now we have len int-digits and can calculate the fract-digits
  if (((fsz - len) > 0) && checkagain){  // we need space for DP and round only once
    fltval += rounders[fsz - len - 1];
  }
  else{  // here are no fractions

  }
  intval = (uint32_t)fltval;
  if (checkagain){
    checkagain = 0;
    goto CHECKAGAIN;
  }
#ifdef ZERO_NEEDED
  if (len==0){ // value is 0.xxx so we need a "0."
    buf[pos++] = '0';
    fsz--;
  }
  else{  //number fits in output
#endif
    if (len == (fsz - 1)){ // special case we have space for a comma but for no digit
      buf[pos++] = TC;
      fsz--;
      //chprintf(chp, "set brk\r\n");
    }
    //chprintf(chp, "fsz now(2): %d\r\n", fsz);
    for (;len>0;len--){  //handle whole numbers
      temp = intval / sz[len-1];
      buf[pos++] = temp + '0';
      fsz--;
      intval -= (temp * sz[len-1]);
      if (!fsz){
        buf[pos++] = 0; //terminate string
        return;
      }
    }
#ifdef ZERO_NEEDED
  }
#endif
  buf[pos++] = DP;
  fsz--;
  //chprintf(chp, "fsz now(3): %d\r\n", fsz);
  intval = (uint32_t)fltval;
  while (fsz){
  //0.3456 = 12.3456- 12
    fltval = fltval - (double)intval;  // hack off the whole part of the number
    fltval *= 10;  // move next digit over
    intval = (uint32_t)fltval;  // get next digit
    buf[pos++] = intval + '0';
    fsz--;
  }
  buf[pos++] = 0; //terminate string
  return;
}
/*
char * myftoa(double fltval, uint8_t fsz){
  uint8_t pos = 0;  // position in string
  uint8_t len;      // length of decimal part of result
  uint8_t checkagain = 1;      //
  uint32_t intval, temp;       // decimal digit(s) to convert
  char buf[11];
  const uint32_t sz[MAXSZ] = {1,     //0  10^0
                          10,        //1  10^1
                          100,       //2  10^2
                          1000,      //3  10^3
                          10000,     //4  10^4
                          100000,    //5  10^5
                          1000000,   //6  10^6
                          10000000,  //7  10^7
                          100000000, //8  10^8
                          1000000000};//9 10^9
  const double rounders[MAXSZ] = {
      0.5,                // 0
      0.05,               // 1
      0.005,              // 2
      0.0005,             // 3
      0.00005,            // 4
      0.000005,           // 5
      0.0000005,          // 6
      0.00000005,         // 7
      0.000000005,        // 8
      0.0000000005        // 9
  };
  if (fsz >= MAXSZ){      //specified too many digits
    memcpy(buf,MAX,sizeof(MAX));
    return buf;
  }
  if (fltval < 0 ){  // handle negative numbers
    fltval *= -1;
    buf[pos++]=NEG;
    fsz--;
  }
  else{
#ifdef POS
    buf[pos++]=POS;
    fsz--;
#endif
  }
  // OK. We have space for fsz digits. Lets assume we have fsz=4 and a value of
  // 99.99. If we truncate right away we come up with 99.9 which is not really correct.
  // We need to round the value but where?
  // I came up with the following solution:

CHECKAGAIN:
  intval = (uint32_t)fltval;  // truncate the floating point number (intval=99)
  //chprintf(chp, "intval: %d\r\n", intval);
  //chprintf(chp, "fsz now(1): %d\r\n", fsz);
  if (intval >= sz[MAXSZ-1]){ //more than 9 digits is not supported
    memcpy(buf,MAX,sizeof(MAX));
    return buf;
  }
  for (len=0; len < MAXSZ; len++){  //find out how many digits we need
    if (intval < sz[len]){
      break;
    }
  }
  if (len > fsz){  //we can't handle this number
    memcpy(buf,OVF,sizeof(OVF));
    return buf;
  }
  //chprintf(chp, "needed digits: %d\r\n", len);
  // Now we now we have len int-digits and can calculate the fract-digits
  if (((fsz - len) > 0) && checkagain){  // we need space for DP and round only once
    fltval += rounders[fsz - len - 1];
  }
  else{  // here are no fractions

  }
  intval = (uint32_t)fltval;
  if (checkagain){
    checkagain = 0;
    goto CHECKAGAIN;
  }
#ifdef ZERO_NEEDED
  if (len==0){ // value is 0.xxx so we need a "0."
    buf[pos++] = '0';
    fsz--;
  }
  else{  //number fits in output
#endif
    if (len == (fsz - 1)){ // special case we have space for a comma but for no digit
      buf[pos++] = TC;
      fsz--;
      //chprintf(chp, "set brk\r\n");
    }
    //chprintf(chp, "fsz now(2): %d\r\n", fsz);
    for (;len>0;len--){  //handle whole numbers
      temp = intval / sz[len-1];
      buf[pos++] = temp + '0';
      fsz--;
      intval -= (temp * sz[len-1]);
      if (!fsz){
        buf[pos++] = 0; //terminate string
        return buf;
      }
    }
#ifdef ZERO_NEEDED
  }
#endif
  buf[pos++] = DP;
  fsz--;
  //chprintf(chp, "fsz now(3): %d\r\n", fsz);
  intval = (uint32_t)fltval;
  while (fsz){
  //0.3456 = 12.3456- 12
    fltval = fltval - (double)intval;  // hack off the whole part of the number
    fltval *= 10;  // move next digit over
    intval = (uint32_t)fltval;  // get next digit
    buf[pos++] = intval + '0';
    fsz--;
  }
  buf[pos++] = 0; //terminate string
  return buf;
}
*/
int uitoa(int value, char * buf, int max) {
  int n = 0;
  int i = 0;
  int tmp = 0;

  if (!buf)
    return -3;
  if (2 > max)
    return -4;
  i = 1;
  tmp = value;
  if (0 > tmp) {
    tmp *= -1;
    i++;
  }
  for (;;) {
    tmp /= 10;
    if (0 >= tmp)
      break;
    i++;
  }
  if (i >= max) {
    buf[0] = '?';
    buf[1] = 0x0;
    return 2;
  }
  n = i;
  tmp = value;
  if (0 > tmp) {
    tmp *= -1;
  }
  buf[i--] = 0x0;
  for (;;) {
    buf[i--] = (tmp % 10) + '0';
    tmp /= 10;
    if (0 >= tmp)
      break;
  }
  if (-1 != i) {
    buf[i--] = '-';
  }
  return n;
}
/* atof: convert string s to double */
double patof(char s[]) {
  double val, power;
  int sign;
  uint8_t i;

  for (i = 0; isspace((int)s[i]); i++)
    /* skip white space */
    ;
  sign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-')
    i++;
  for (val = 0.0; isdigit((int)s[i]); i++)
    val = 10.0 * val + (s[i] - '0');
  if (s[i] == '.')
    i++;
  for (power = 1.0; isdigit((int)s[i]); i++) {
    val = 10.0 * val + (s[i] - '0');
    power *= 10;
  }
  return sign * val / power;
}
// like normal memset but can init with character c and terminates it with NULL
void my_memset (char* s, uint8_t c, int len){
  len--;
  while(len--)
    *s++ = c;
  *s = 0;
}
/*
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * At most n characters are moved.
 * Return s1.
 */
char * strcatn(char* s1, char* s2, int n)
//register char *s1, *s2;
//register n;
{
    char *os1;

    os1 = s1;
    while (*s1++)
        ;
    --s1;
    while (*s1++ == *s2++)
        if (--n < 0) {
            *--s1 = '\0';
            break;
        }
    return(os1);
}
