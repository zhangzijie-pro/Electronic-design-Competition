// DAC 输出电压
//------------------------ Include files ------------------------//
#include "DAC8550.h"
#include "delay.h"
#include "..\FWlib\inc\stm32f4xx_rcc.h"
#include "..\FWlib\inc\stm32f4xx_gpio.h"
#include "..\FWlib\inc\stm32f4xx_dac.h"


void DAC8550_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	 //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,(GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11));						 //PA.8 输出高
}

//向8550写数据
void write_8550(unsigned int data) 
{ 
  u8 i;  
	SYNC1;   
	delay_us(1);  
	SCLK0;   
	SYNC0; 
	for(i=0;i<24;i++)   
	{ 
    if(0x800000&data)DIN1;     
		else             DIN0;     
		SCLK1;
		delay_us(1);  
		SCLK0;
		data=data<<1;   
	} 
  delay_us(1);   
	SYNC1; 
} 
// date的范围从–32768 到 32767对应输出0到5V
void voltage_output(int16_t data)
{
	unsigned int ctr=0x000000; //正常输出 
	ctr = ctr|(data&0xffff);  
	write_8550(ctr); 
}
