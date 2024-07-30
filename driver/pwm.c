#include "pwm.h"
#include "..\fwlib\inc\stm32f4xx_gpio.h"
#include "..\fwlib\inc\stm32f4xx_rcc.h"
#include "..\fwlib\inc\stm32f4xx_tim.h"

int PWM_initialize(void);
int PWM_set_compare(int temp);



int PWM_initialize(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);                       //开启TIM8定时器的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);                     //开启GPIOI的时钟
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);                     //PI5复用为TIM8
	//GPIO初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                              //模式设为复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                            //复用推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	//TIM8初始化
	TIM_TimeBaseStructure.TIM_Prescaler = 83;                                 //设置预分频值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;               //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 499;                                   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                   //设置时钟分割 
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	
	//TIM8 CH1 LED_RED 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                         //设置模式为PWM
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;             //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;           //互补输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                                        //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                  //当定时器的值小于脉冲值时，输出低电平
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);                         //使能预装载寄存器
 
	TIM_ARRPreloadConfig(TIM8,ENABLE);                                        //使能TIM8在ARR上的预装载寄存器
	
	TIM_Cmd(TIM8, ENABLE);                                                    //TIM8使能
	
	TIM_CtrlPWMOutputs(TIM8, ENABLE);                                         //使能主输出
	
	return 0;
}


int PWM_set_compare(int temp)
{
	TIM_SetCompare1(TIM8,temp);	                                              //修改占空比
	return 0;
}