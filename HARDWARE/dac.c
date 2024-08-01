#include "dac.h"

#include "..\FWlib\inc\stm32f4xx_rcc.h"
#include "..\FWlib\inc\stm32f4xx_gpio.h"
#include "..\FWlib\inc\stm32f4xx_dac.h"

//--------------------------- Variable --------------------------// 
static int initialize_dac1(void);
static void set_voltage(double);
DAC_T dac1 = {	
	.initialize = initialize_dac1,
	.set_voltage = set_voltage
	};

static int initialize_dac1(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);             // GPIO?  ?  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
	DAC_InitType.DAC_Trigger = DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1,&DAC_InitType);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
	
	return 0;
}

void set_voltage(double voltage)
{	
	unsigned short int temp;
	
	temp = voltage * 4095 / 3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);
}
