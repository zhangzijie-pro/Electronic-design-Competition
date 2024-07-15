/*
 * FILE                : adc.c
 * DESCRIPTION         : --
 * Author              : ysloveivy.
 * Copyright           :
 *
 * History
 * --------------------
 * Rev                 : 0.00
 * Date                : 11/21/2015
 *
 * create.
 * --------------------
 */
//------------------------ Include files ------------------------//
#include "..\FWlib\inc\stm32f4xx_adc.h"
#include "..\FWlib\inc\stm32f4xx_gpio.h"
#include "..\FWlib\inc\stm32f4xx_rcc.h"
#include "adc.h"

//--------------------- Function Prototype ----------------------//
static int initialize(void);
static int read_adc(int);

//-------------------------- Variable ---------------------------//
SYS_ADC_T adc = {
	.initialize = initialize,
	.read = read_adc
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
 * Date                : 11/21/2015
 * 
 * create.
 * --------------------
 */
static int initialize(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;
	ADC_InitTypeDef  ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);                            //???ADC1??? 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);                            //???ADC3??? 

	//ADC1_PC4<----->???14, ADC1_PC5<----->?? ?15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                                    //???????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//ADC3_PF5<----->???15, ADC3_PF6<----->???4, ADC3_PF7<----->???5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                                    //???????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                        //ADC???????
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;    //?????????????????????????????5??????
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;         //???DMA 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div6;                     //???¡Â??????6??? 
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//?????ADC
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;                          //ADC????????12¦Ë
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                   //???????
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                             //????????????
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;     //??????????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                          //??????
	ADC_InitStructure.ADC_NbrOfConversion = 1;                                      //1????????????????
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//???ADC
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC3, ENABLE);

	return 0;
}
/*
 * Name                  : sort
 * Description           : ---
 * Author                : XiaomaGee.
 *
 * History
 * ----------------------
 * Rev                   : 0.00
 * Date                  : 11/21/2015
 *
 * create.
 * ----------------------
 */
void sort(unsigned short int a[], int n)
{
	int i, j, t;
	
	//????§³????????
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (a[j] > a[j + 1]) {
				t = a[j];
				a[j] = a[j + 1];
				a[j + 1] = t;
			}
		}
	}
}
/*
 * Name                  : read_adc
 * Description           : ---
 * Author                : ysloveivy.
 *
 * History
 * ----------------------
 * Rev                   : 0.00
 * Date                  : 11/21/2015
 *
 * create.
 * ----------------------
 */
static int read_adc(int channel)
{
	int i,k;
	unsigned long int temp[20] = {0};
	unsigned long int value;
	unsigned short int data[100];
	unsigned char channel_remap[5] = {ADC_Channel_14,ADC_Channel_15,ADC_Channel_15,ADC_Channel_4,ADC_Channel_5};

	//??????100?????????§Þ?20??????????????
	//?????20???????????????????????????????
	for(k = 0;k < 20;k++){
		for(i = 0;i < 100;i++){
			if(channel == 0 || channel == 1){
				ADC_RegularChannelConfig(ADC1, channel_remap[channel], 1, ADC_SampleTime_480Cycles); //ADC1???????????14??15????????§µ?????????480??????

				ADC_SoftwareStartConv(ADC1);                                                         //????????ADC1?????????????????

				while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));                                       //??????????

				data[i] = ADC_GetConversionValue(ADC1);	                                             //?????????

			}else {
				ADC_RegularChannelConfig(ADC3, channel_remap[channel], 1, ADC_SampleTime_480Cycles); //ADC3???????????15??4??5????????§µ?????????480??????

				ADC_SoftwareStartConv(ADC3);

				while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));

				data[i] = ADC_GetConversionValue(ADC3);	

			}
		}

		sort(data,100);

		for(i = 40;i < 60;i++){
			temp[k] += data[i];
		}

		temp[k] = temp[k] / 20;
	}

	value = 0;
	for(k = 0;k < 20;k++){
		value += temp[k]; 
	}
	value /= 20;

	adc.value[channel] = value * ADC_REF / 4096; 

	return value;
}
