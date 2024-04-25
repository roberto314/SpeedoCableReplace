/*
 * myStringfunctions.h
 *
 *  Created on: 07.01.2018
 *      Author: Anwender
 */

#ifndef MYSTRINGFUNCTIONS_H_
#define MYSTRINGFUNCTIONS_H_



void FloatToStringNew(char *str, float fltval, uint8_t size, uint8_t comma, BaseSequentialStream *chp);
//void myftoap(char *buf, double fltval, uint8_t fsz, BaseSequentialStream *chp); //DEBUG
void myftoap(char *buf, double fltval, uint8_t fsz);
double patof(char s[]);
void my_memset (char* s, uint8_t c, int len);
char * strcatn(char* s1, char* s2, int n);
char * myftoa(double fltval, uint8_t fsz);
int uitoa(int value, char * buf, int max);

#endif /* MYSTRINGFUNCTIONS_H_ */
