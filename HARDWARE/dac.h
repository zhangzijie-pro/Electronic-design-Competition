#ifndef __DAC_H__
#define __DAC_H__
#define OUT_LENGTH (1024)
#include "stm32f4xx.h"

void DAC_Init_Voice(void);
extern float Dac_output[OUT_LENGTH];
extern uint8_t dac_start_flag;

#endif