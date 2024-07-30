#ifndef _DAC8550_h_
#define _DAC8550_h_

#include "stm32f4xx.h"

#define DIN1 GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define SCLK1 GPIO_SetBits(GPIOC,GPIO_Pin_10)
#define SYNC1  GPIO_SetBits(GPIOC,GPIO_Pin_9)


#define DIN0 GPIO_ResetBits(GPIOC,GPIO_Pin_11)
#define SCLK0 GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define SYNC0  GPIO_ResetBits(GPIOC,GPIO_Pin_9)


void DAC8550_init(void);
void write_8550(unsigned int data);
void voltage_output(int16_t data);

#endif
