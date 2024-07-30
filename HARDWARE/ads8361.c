//------------------------ Include files ------------------------//
#include "..\FWlib\inc\stm32f4xx_adc.h"
#include "..\FWlib\inc\stm32f4xx_gpio.h"
#include "..\FWlib\inc\stm32f4xx_rcc.h"
#include "ads8361.h"
#include "delay.h"


/**********************************************************************************************************
*	函 数 名: ADS8361_IO_Init
*	功能说明: ADS8361 IO口初始化
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void ADS8361_IO_Init(void)	   //ADS8361 IO口初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	 //使能端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	CS=1;  //失能芯片 初始化CLOCK为低
  CLOCK=0;
	RD=0;          
  CONVST=0; 
}

/**********************************************************************************************************
*	函 数 名: ADS8361_Mode_selection
*	功能说明: ADS8361模式选择
*	形    参: mode
*	返 回 值: 无
**********************************************************************************************************/
void ADS8361_Mode_selection(uint8_t mode)//ADS8361模式选择
{
	switch(mode)
  {
    case Mode_I:	M0 = 0; M1 = 0;
      break;
		case Mode_II:	M0 = 0; M1 = 1;
      break;
		case Mode_III:M0 = 1; M1 = 0;
      break;
		case Mode_IV:	M0 = 1; M1 = 1;
      break;
    default:
      break;
  }
}

/**********************************************************************************************************
*	函 数 名: ADS8361_Init
*	功能说明: ADS8361初始化引脚并选择模式
*	形    参: mode
*	返 回 值: 无
**********************************************************************************************************/
void ADS8361_Init(uint8_t mode)	   //ADS8361初始化
{
	ADS8361_IO_Init();
	delay_ms(10);
	ADS8361_Mode_selection(mode);
	delay_ms(10);
}

/**********************************************************************************************************
*	函 数 名: ADS8361_ReadData
*	功能说明: 读取ADS8361通道AB0或AB1数据(模式I)。不同模式时序请参考ADS8361芯片手册！！！
*	形    参: A0_Select=0选择通道A0,B0; A0_Select=1时选择通道A1,B1
*	返 回 值: 无
**********************************************************************************************************/
void ADS8361_ReadData(uint8_t A0_Select,uint16_t *data)
{
  u8 i = 0;	
  
	CS=0;  	//使能芯片
	CLOCK=1;
	A0=CH_AB0; //选择下一个转换周期的通道进行读取
	RD=1;  
	CONVST=1;  // 开始转换
	CLOCK=0;

	CLOCK=1;
	CLOCK=0;
	CLOCK=1;
	CLOCK=0;// 前两个脉冲在模式I下并不起作用没有标志的用途// 所以忽略不读
								
	for(i=0;i<16;i++)  // 读取16位AD转换后的值，高位在前，所以每读一位是左移
	{
		data[2] <<= 1;
		data[3] <<= 1;
		CLOCK=1;
		if(DATA_A){data[2]++;}//读取A1通道数据,下一个周期才是A0B0
		if(DATA_B){data[3]++;}//读取B1通道数据
		CLOCK=0;
	}
	CLOCK=1;
	CLOCK=0;
	CLOCK=1;
	CLOCK=0;//补齐一个读写时序20位
	
	while(BUSY);  // 等待转换周期完成

	RD=0;         // 准备下一次转换和读写
	CONVST=0;     // 准备下一次转换和读写
	
}


/**********************************************************************************************************
*	函 数 名: ADS8361_Read_ALLData
*	功能说明: 读取ADS8361所有通道数据(模式I)
*	形    参: data:数据指针
*	返 回 值: 无
**********************************************************************************************************/

void ADS8361_Read_ALLData(uint16_t *data)
{
	u8 i = 0;	
	
	CS=0;  	//使能芯片
	CLOCK=1;
	A0=CH_AB0; //选择下一个转换周期的通道进行读取
	RD=1;  
	CONVST=1;  // 开始转换
	CLOCK=0;

	CLOCK=1;
	CLOCK=0;
	CLOCK=1;
	CLOCK=0;// 前两个脉冲在模式I下并不起作用没有标志的用途// 所以忽略不读
								
	for(i=0;i<16;i++)  // 读取16位AD转换后的值，高位在前，所以每读一位是左移
	{
		data[2] <<= 1;
		data[3] <<= 1;
		CLOCK=1;
		if(DATA_A){data[2]++;}//读取A1通道数据,下一个周期才是A0B0
		if(DATA_B){data[3]++;}//读取B1通道数据
		CLOCK=0;
	}
	CLOCK=1;
	CLOCK=0;
	CLOCK=1;
	CLOCK=0;//补齐一个读时序20位
	
	while(BUSY);  // 等待转换周期完成

	RD=0;         // 准备下一次转换和读写
	CONVST=0;     // 准备下一次转换和读写
	
	CLOCK=1;
	A0=CH_AB1; // 选择通道进行读取

	RD=1;  
	CONVST=1;  // 开始转换
	CLOCK=0;

	CLOCK=1;
	CLOCK=0;
	CLOCK=1;
	CLOCK=0;// 前两个脉冲在模式I下并不起作用没有标志的用途// 所以忽略不读
                                 
	for(i=0;i<16;i++)  // 读取16位AD转换后的值，高位在前，所以每读一位是左移
	{
		data[0] <<= 1;
		data[1] <<= 1;
		CLOCK=1;
		if(DATA_A){data[0]++;}//读取A0通道数据
		if(DATA_B){data[1]++;}//读取B0通道数据
		CLOCK=0;
	}
	CLOCK=1;
	CLOCK=0;
	CLOCK=1;
	CLOCK=0;//补齐一个读时序20位
	
	while(BUSY);  // 等待转换周期完成

	RD=0;         // 准备下一次转换和读写
	CONVST=0;     // 准备下一次转换和读写

}

