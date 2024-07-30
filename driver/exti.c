#include "..\include\exti.h"
#include "..\fwlib\inc\stm32f4xx_gpio.h"
#include "..\fwlib\inc\stm32f4xx_rcc.h"
#include "..\fwlib\inc\stm32f4xx_exti.h"
#include "..\fwlib\inc\misc.h"
#include "..\fwlib\inc\stm32f4xx_syscfg.h"
#include "led.h"

// 中断

//---------------------------- Define ---------------------------//
static int initialize(void);
//-------------------------- Variable ---------------------------//
EXTI_T exti = {
	.initialize = initialize
};


static int initialize(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);                    //使能GPIOH时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                  //使能SYSCFG时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOH, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH, EXTI_PinSource15);          //PH15与EXTI15连接

	EXTI_InitStructure.EXTI_Line = EXTI_Line15;                             //设置连接中断线15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                     //设置中断模式为中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                 //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//设置中断优先级分组
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	return 0;
}

void EXTI15_10_IRQHandler(void)
{
	int i;
	static int work_status = 0;

	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
