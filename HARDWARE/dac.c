#include "dac.h"
#include "stm32f4xx.h"

float Dac_output[OUT_LENGTH]={0};
float Dac_output2[OUT_LENGTH]={0};
uint8_t dac_start_flag=0;


void DAC_GPIO_Init()
{
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void DAC_Time_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 根据需要调整
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // 1 MHz的计数频率
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	TIM_Cmd(TIM6, ENABLE);
}

void DAC_DMA_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Stream5); // DMA1, Stream5, Channel7对应DAC1

	DMA_InitStructure.DMA_Channel = DMA_Channel_7;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Dac_output[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = OUT_LENGTH;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);

	DMA_Cmd(DMA1_Stream5, ENABLE);

	DAC_DMACmd(DAC_Channel_1, ENABLE);
}

void DAC_Out_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	DAC_InitTypeDef DAC_InitStructure;

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO; // 使用定时器6触发
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	
//	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
//	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	if(dac_start_flag) {
    DAC_Cmd(DAC_Channel_1, ENABLE);
//  DAC_Cmd(DAC_Channel_2, ENABLE); // 如果使用通道2
   }
}

void DAC_Init_Voice(void){
	DAC_GPIO_Init();
	DAC_Time_Init();
	DAC_DMA_Init();
	DAC_Out_Init();

}

void DAC_Control(void) {
    if (dac_start_flag) {
        if (!(DAC->CR & DAC_CR_EN1)) { // 如果DAC没有启动
            DAC_Cmd(DAC_Channel_1, ENABLE);
            // DAC_Cmd(DAC_Channel_2, ENABLE); // 如果使用通道2
        }
    } else {
        if (DAC->CR & DAC_CR_EN1) { // 如果DAC启动了
            DAC_Cmd(DAC_Channel_1, DISABLE);
            // DAC_Cmd(DAC_Channel_2, DISABLE); // 如果使用通道2
        }
    }
}