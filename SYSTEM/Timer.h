# ifndef _TIMER_H
# define _TIMER_H

# include "stm32f4xx.h"

void        Timer_Init(void);
uint32_t    Timer_get(void);
uint32_t    Timer_get_sub(uint32_t c);
void        TIM10_NVIC_Configuration(void);
void TIM2_Init(uint16_t arr, uint16_t psc);

# endif
