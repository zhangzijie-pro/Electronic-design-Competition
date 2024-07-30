#ifndef __AD_H__
#define __AD_H__

#include "stm32f4xx.h"

#define ADC_BUFFER_SIZE 1024
void AD_Init(void);
extern uint16_t ad_data[ADC_BUFFER_SIZE];
#endif