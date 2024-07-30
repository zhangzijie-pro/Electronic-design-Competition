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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);                             //ʹ��DMA2ʱ��
  DMA_DeInit(DMA2_Stream0);                                                        //����DMA2
  while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)                                //�ȴ�DMA������
  {
  }
	
	return 0;
}


/*
example: 

DMA_InitTypeDef  DMA_InitStructure;
	
	led.initialize();
	dma.initialize();
	
	//����DMA�����Գɹ�������˸������ʧ�ܣ������˸
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;                                   //ѡ��ͨ��0
  DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned long int)src_buffer;        //DMA�������ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (unsigned long int)dst_buffer;           //DMA�洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;                              //����Ϊ�洢�����洢��
  DMA_InitStructure.DMA_BufferSize = (unsigned long int)BUFFER_SIZE;               //���ݴ�����
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;                  //��������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                          //�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;          //�����������ݿ��
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;                  //���ô洢�����ݿ��
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                    //����ģʽΪ��ͨģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                              //���ȼ�����Ϊ��
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                           //FIFOģʽ��ֹ
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;                    //FIFO��ֵ
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                      //�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;              //����ͻ�����δ���
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);

  DMA_Cmd(DMA2_Stream0, ENABLE);                                                   //ʹ��DMA2_Stream0

	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
	
	for(i = 0;i < BUFFER_SIZE;i++){
		if(dst_buffer[i] != src_buffer[i]){
			//����ʧ��
			while(1){
					for(k = 0;k < 10000000;k++);
					LED_RED_ON;
					for(k = 0;k < 10000000;k++);
					LED_RED_OFF;
			}
		}
	}
	//���Գɹ�
	while(1){
		for(k = 0;k < 10000000;k++);
		LED_BLUE_ON;
		for(k = 0;k < 10000000;k++);
		LED_BLUE_OFF;		
	}


*/