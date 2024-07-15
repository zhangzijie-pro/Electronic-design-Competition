/*
 * FILE               : adc.h
 * DESCRIPTION        : This file is iCore3 adc file header.
 * Author             : XiaomaGee@Gmail.com
 * Copyright          :
 *
 * History
 * --------------------
 * Rev                : 0.00
 * Date               : 11/21/2015
 *
 * create.
 * --------------------
 */
#ifndef __ADC_H__
#define __ADC_H__
#define ADC_REF 2.483
//--------------------------- Define ---------------------------//

//-------------------------- Typedef----------------------------//
typedef  struct {
	int (* initialize)(void);
	int (* read)(int);
	float value[5];
}SYS_ADC_T;

extern SYS_ADC_T adc;
#endif //__ADC_H__
