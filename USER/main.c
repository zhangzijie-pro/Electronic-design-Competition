#include "stm32f4xx.h"
#include "led.h"
#include "usart.h"
#include "arm_math.h"
#include "Timer.h"
#include "delay.h"
#include "ad.h"
#include "arm_math.h"
#include "bsp_key.h"
#include "pwm.h"
#include "lcd_init.h"
#include "lcd1.h"
#include "pic.h"
#include "rfft.h"
#include <stdbool.h>

#define Fs 100000
#define FFT_LENGTH (1024)
#define SAMPLE_RATE 40000
#define WINDOW_SIZE 128 // 定义窗口大小，必须是FFT_LENGTH的因子
#define NUM_WINDOWS (FFT_LENGTH / WINDOW_SIZE) // 计算窗口数量

// ------------------- FFT value------------------------
float32_t lBufInArray[FFT_LENGTH*2]={0};
float32_t lBufOutArray[FFT_LENGTH]={0};
float32_t lBufOut_To_InArray[FFT_LENGTH]={0};
float32_t rlBufOutArray[FFT_LENGTH]={0};
float window_sum[NUM_WINDOWS]; // 存储每个窗口的和
float magnitude[FFT_LENGTH];         // 幅度数组
float phase[FFT_LENGTH];            // 相位数组
float frequency[FFT_LENGTH];       // 频率数组
int fft_flag = 0;  // 正向变化
int fft_reverse = 1; // 开启反转输出
float max_magnitude;
int max_mag_index;
float avgPhase=0.0;
float dominantFrequency;

// ------------------- Voice Input------------------------
//int Max_Index[5] = {0};
int one_part_index[3]={0};
float input_recognize[9];
// ------------------- State------------------------
float SOME_THRESHOLD;  // 阈值
int pwm_flag=1;		// pwm档位
bool Ustate = false;   // 超声波开关
int cnt=0;							// 超声波计数
bool Vstate = false;   // 声音状态   是否有声音
int shield_type=0;						// 屏蔽内容 
int vot_cnt = 0;       // 关闭电源计数

// ------------------- Func------------------------
void FFT(void);								// FFT 																---------->		{voice source  ,  shield_type}
void low_high_sort(int *input, int size);	// sort   low -> high			|
void voice_source_func(void);							//  voice -> state ->  exist/unexist  ->  NULL
void switch_Ustate(void);
void recognize_voice(void);		// 声音来源检测
void sliding_window_fft_analysis(); // 声音检测有无
void Window_threshold();
void analysis_choice_human_music();  // 声音来源检测
void close_vot(void);

int main(){
	
	delay_init(168);
	LED_initialize();
	key_Init();
	Timer_Init();
	PWM_initialize();
	AD_Init();
	//usart4.initialize(115200);
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	close_vot();
	while(1){
		key_Scan();
		recognize_voice();
		//analysis_choice_human_music();
		
		switch_Ustate();
		voice_source_func();
	
		// PWM 按键档位控制
		if(Key1){							// 1挡
			PWM_set_compare(70);
			LED_GREEN_OFF;
			LED_RED_ON;
			LED_BLUE_OFF;
			pwm_flag=1;
		}
		if(Key2){							// 2挡
			PWM_set_compare(80);
			LED_GREEN_ON;
			LED_RED_OFF;
			LED_BLUE_OFF;
			pwm_flag=2;
		}
		if(Key3){							// 3挡
			PWM_set_compare(90);

			LED_GREEN_OFF;
			LED_RED_OFF;
			LED_BLUE_ON;
			pwm_flag=3;
		}
		if(Key4){							// 4挡
			PWM_set_compare(100);
			LED_GREEN_OFF;
			LED_RED_OFF;
			LED_BLUE_OFF;
			pwm_flag=4;
		}
		
		if(Key5){				// 开启超声波
			cnt++;
			if(cnt%2==0){
				Ustate=false;
			}else{
				Ustate=true;
			}
		}
		
		if(Key6){			// 切换屏蔽内容      1 ->  human      2 -> music
			shield_type=1;
		}
		
		if(Key7){
			shield_type =2;
		}

	}
}

// FFT main
void FFT(void)
{
		arm_cfft_radix4_instance_f32 scfft;
		
		arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,fft_flag,fft_reverse);		// 初始化结构体   0: 正向变化   1：开启反转输出

		for(int i=0;i<FFT_LENGTH;i++)
		{
			lBufInArray[i*2]= (float) ad_data[i]*3.3/4096;
			lBufInArray[i*2+1]=0;
		}
		
		arm_cfft_radix4_f32(&scfft,lBufInArray);	// fft计算  时域信号转为频域信号
		arm_cmplx_mag_f32(lBufInArray, lBufOutArray, FFT_LENGTH);
		lBufOutArray[0]=0;
		lBufOutArray[1]=0;
		lBufOutArray[2]=0;
		
		for (int i = 1; i < FFT_LENGTH; i ++){
			lBufOutArray[i] /= 512;
		}

		float32_t Max = 0;
		uint32_t  index = 0;
		for(uint8_t i=0;i<3;i++){
//		for (uint8_t i = 0; i < 5; i ++){
				arm_max_f32(lBufOutArray, FFT_LENGTH, &Max, &index);
				if (index > (FFT_LENGTH/2)){
						index = 1024- index;
				}
//				usart4.printf("Max: %f index: %d\r\n", Max, index);
				one_part_index[i]=index;
				//Max_Index[i]=index;
				lBufOutArray[index] = 0;
				lBufOutArray[1024 - index] = 0;
		}
//		int part_index=0;
//		low_high_sort(Max_Index,5);		// 排序
//		for(int i=4;i>1;i--){
//			one_part_index[part_index++]=Max_Index[i];
//		}
		  low_high_sort(one_part_index,3);
//		usart4.printf("------------------------\r\n");
}

// 交换两个整数的值
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 从小到大排序函数
void low_high_sort(int *input, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (input[j] > input[j + 1]) {
                swap(&input[j], &input[j + 1]);
            }
        }
    }
}

// 得到input_recognize的值到voice_Detection(float *input) ；
void recognize_voice(){
	int input_index = 0;
	for(int i=0;i<3;i++){
		FFT();
		input_recognize[input_index++]=one_part_index[0];
		input_recognize[input_index++]=one_part_index[1];
		input_recognize[input_index++]=one_part_index[2];
	}
	
	voice_Detection(input_recognize);  // return voice_source  -> 0 -> music    1 -> human
}

// 得到阈值
void Window_threshold(){
	  float mean = calculate_mean(lBufOutArray, FFT_LENGTH);
    float std_dev = calculate_std_dev(lBufOutArray, FFT_LENGTH, mean);

    // 设置阈值为均值加上标准差
    SOME_THRESHOLD = mean +  std_dev;
}


//  判断有无音频
void sliding_window_fft_analysis() {
	Window_threshold();
	if(SOME_THRESHOLD>10){
		Vstate = true;
	}else{
		Vstate=false;
	}
}


void analysis_choice_human_music() {
	FFT();
	Window_threshold();
	if(SOME_THRESHOLD>8&&SOME_THRESHOLD<30){
		voice_source = 1;
	}else if(SOME_THRESHOLD>30&&SOME_THRESHOLD<80){
		voice_source=0;
	}
}


// Ultrasonic
void switch_Ustate(void){
	// 超声波开关
	LCD_ShowString(2,1,"Ultrasonic state:",WHITE,BLACK,12,0);
	if(Ustate==true){
			LCD_ShowString(110,1," ON",WHITE,BLACK,12,0);
	}else if(Ustate==false){
			LCD_ShowString(110,1,"OFF",WHITE,BLACK,12,0);
	}	
	LCD_ShowString(2,15,"Ultrasonic gear:",WHITE,BLACK,12,0);	
	LCD_ShowIntNum(100,15,pwm_flag,3,WHITE,BLACK,12);
	LCD_ShowString(2,30,"Output power:",WHITE,BLACK,12,0);	
	LCD_ShowIntNum(100,30,pwm_flag,3,WHITE,BLACK,12);
	LCD_ShowString(140,30,"W",WHITE,BLACK,12,0);
}

// Voice
void voice_source_func(void)
{
	LCD_ShowString(2,60,"Voice state:",WHITE,BLACK,12,0);
	sliding_window_fft_analysis();
	if(Vstate){  // 识别
			recognize_voice();
			LCD_ShowString(80,60,"  exist",WHITE,BLACK,12,0);
			//  检测人声，音乐
			LCD_ShowString(2,75,"Voice source:",WHITE,BLACK,12,0);
			if(voice_source==0){
				LCD_ShowString(80,75,"music",WHITE,BLACK,12,0);
			}else if(voice_source==1){
				LCD_ShowString(80,75,"human",WHITE,BLACK,12,0);
			}
			//  屏蔽内容选项
			if(shield_type==1){
					//  当前屏蔽内容
					LCD_ShowString(2,90,"shield now:",WHITE,BLACK,12,0);	
					LCD_ShowString(70,90,"human    ",WHITE,BLACK,12,0);
			}else if(shield_type==2){
					//  当前屏蔽内容
					LCD_ShowString(2,90,"shield now:",WHITE,BLACK,12,0);		
					LCD_ShowString(70,90,"music    ",WHITE,BLACK,12,0);	
			}
			else{
					LCD_ShowString(2,90,"Please select shield",WHITE,BLACK,12,0);
			}
		
	}
	else
	{
			LCD_ShowString(80,60,"Unexist",WHITE,BLACK,12,0);
			LCD_ShowString(2,75,"                    ",WHITE,BLACK,12,0);
			LCD_ShowString(2,90,"                    ",WHITE,BLACK,12,0);
	}
}

// PF10  1 -> 0  |  0 -> 1
void close_vot(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
}