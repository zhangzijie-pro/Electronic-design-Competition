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
