#include "led.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

//--------------------- Function Prototype ----------------------//
int LED_initialize(void);

int LED_initialize(void)
{
	GPIO_InitTypeDef   GPIO_uInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
	GPIO_uInitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;    
	GPIO_uInitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_uInitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_uInitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           
	GPIO_uInitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOI,&GPIO_uInitStructure);

	GPIO_SetBits(GPIOI,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

	return 0;
}
