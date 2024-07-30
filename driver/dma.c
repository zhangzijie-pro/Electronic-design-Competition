/*
 * FILE                : dma.c
 * DESCRIPTION         : This file is dma driver.
 * Author              : ysloveivy
 * Copyright           :
 *
 * History
 * --------------------
 * Rev                 : 0.00
 * Date                : 11/29/2015
 *
 * create.
 * --------------------
 */
//--------------------------- Include ---------------------------//
#include "..\include\dma.h"
#include "..\fwlib\inc\stm32f4xx_dma.h"
#include "..\fwlib\inc\stm32f4xx_rcc.h"
#include "..\include\main.h"

//--------------------- Function Prototype ----------------------//
static int initialize(void);

//--------------------------- Variable --------------------------//
DMA_T dma = {
	.initialize = initialize
};

//--------------------------- Function --------------------------//

/*
 * Name                : initialize
 * Description         : ---
 * Author              : ysloveivy.
 *
 * History
 * --------------------
 * Rev                 : 0.00
 * Date                : 11/29/2015
 * 
 * create.
 * --------------------
 */

static int initialize(void)
{  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                             //使能DMA2时钟
  DMA_DeInit(DMA2_Stream0);                                                        //配置DMA2
  while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)                                //等待DMA可配置
  {
  }
	
	return 0;
}


/*
example: 

DMA_InitTypeDef  DMA_InitStructure;
	
	led.initialize();
	dma.initialize();
	
	//测试DMA，测试成功蓝灯闪烁，测试失败，红灯闪烁
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;                                   //选择通道0
  DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned long int)src_buffer;        //DMA外设基地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (unsigned long int)dst_buffer;           //DMA存储器地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;                              //方向为存储器到存储器
  DMA_InitStructure.DMA_BufferSize = (unsigned long int)BUFFER_SIZE;               //数据传输量
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;                  //外设增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                          //存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;          //设置外设数据宽度
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;                  //设置存储器数据宽度
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                    //运行模式为普通模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                              //优先级级别为高
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                           //FIFO模式禁止
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;                    //FIFO阈值
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                      //存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;              //外设突发单次传输
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);

  DMA_Cmd(DMA2_Stream0, ENABLE);                                                   //使能DMA2_Stream0

	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
	
	for(i = 0;i < BUFFER_SIZE;i++){
		if(dst_buffer[i] != src_buffer[i]){
			//测试失败
			while(1){
					for(k = 0;k < 10000000;k++);
					LED_RED_ON;
					for(k = 0;k < 10000000;k++);
					LED_RED_OFF;
			}
		}
	}
	//测试成功
	while(1){
		for(k = 0;k < 10000000;k++);
		LED_BLUE_ON;
		for(k = 0;k < 10000000;k++);
		LED_BLUE_OFF;		
	}


*/