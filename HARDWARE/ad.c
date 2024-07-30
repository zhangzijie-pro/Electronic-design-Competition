#include "stm32f4xx.h"
#include "ad.h"

uint16_t        ad_data[ADC_BUFFER_SIZE] = {0};         // ADC数据采集

static void TIM3_Config(void){

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE );

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_Prescaler = 1-1;
    TIM_TimeBaseStructure.TIM_Period = 2100-1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure );

    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
		TIM_ARRPreloadConfig(TIM3, ENABLE);
}

static void AD_GPIO_Config(void){

    // 打开GPIO 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


static void AD_DMA_Config(void){

    // 打开DMA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);   

    DMA_DeInit(DMA2_Stream0); // 复位DMA2 Stream0
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
	{
	}

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ad_data;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

    DMA_Init(DMA2_Stream0, &DMA_InitStructure);   
    DMA_ClearITPendingBit( DMA2_Stream0 ,DMA_IT_TCIF0 );
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);   
    DMA_Cmd(DMA2_Stream0, ENABLE);                         
}

static void AD_Mode_Config(void){

    // 打开ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC1, &ADC_InitStructure);

    //ADC1 规则通道为通道2，一个序列，采样时间为3个周期
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_3Cycles); 

    // 启用DMA请求  
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    // 使能ADC DMA 请求
    ADC_DMACmd(ADC1, ENABLE); 
}

void AD_Init(void){

    // ADC GPIO 初始化
    AD_GPIO_Config();
    // TIM3 初始化
    TIM3_Config();
    // ADC DMA 初始化
    AD_DMA_Config();
    // ADC 模式初始化
    AD_Mode_Config();
 
    TIM_Cmd(TIM3, ENABLE);	   
    // 开启ADC ，并开始转换
    ADC_Cmd(ADC1, ENABLE);  
}