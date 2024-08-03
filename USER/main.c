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
#include "dac.h"
#include <stdbool.h>

#define Fs 100000
#define FFT_LENGTH (1024)
#define SAMPLE_RATE 40000
#define WINDOW_SIZE 128 // ���崰�ڴ�С��������FFT_LENGTH������
#define NUM_WINDOWS (FFT_LENGTH / WINDOW_SIZE) // ���㴰������


// ------------------- FFT value------------------------
float32_t lBufInArray[FFT_LENGTH*2]={0};
float32_t lBufOutArray[FFT_LENGTH]={0};
float32_t lBufOut_To_InArray[FFT_LENGTH]={0};
float32_t rlBufOutArray[FFT_LENGTH]={0};
float window_sum[NUM_WINDOWS]; // �洢ÿ�����ڵĺ�
float magnitude[FFT_LENGTH];         // ��������
float phase[FFT_LENGTH];            // ��λ����
float frequency[FFT_LENGTH];       // Ƶ������
int fft_flag = 0;  // ����仯
int fft_reverse = 1; // ������ת���
float max_magnitude;
int max_mag_index;
float avgPhase=0.0;
float dominantFrequency;

// ------------------- Voice Input------------------------
int one_part_index[3]={0};
float input_recognize[9];
// ------------------- State------------------------
float SOME_THRESHOLD;  // ��ֵ
int pwm_flag=0;		// pwm��λ
bool Ustate = false;   // ����������
int cnt=0;							// ����������
bool Vstate = false;   // ����״̬   �Ƿ�������
int shield_type=0;						// �������� 
int vot_cnt = 0;       // �رյ�Դ����
int voice_cnt=0;
bool Voice_Exist=false;
bool auto_state = false;
// ------------------- Func------------------------
void FFT(void);								// FFT 																---------->		{voice source  ,  shield_type}
void low_high_sort(int *input, int size);	// sort   low -> high			|
void voice_source_func(void);							//  voice -> state ->  exist/unexist  ->  NULL
void switch_Ustate(void);			// ��������Դ
void recognize_voice(void);		// ������Դ���
void sliding_window_fft_analysis(); // �����������
void Window_threshold();							// �õ���ֵ
void analysis_choice_human_music();  // ������Դ���
void gpio_delay_ms(uint32_t ms);
void open_U();
void auto_voice_source_func(void);		// �Զ�
void voice_source_func(void);					// �ֶ�


int main(){
	
	delay_init(168);
	LED_initialize();
	key_Init();
	Timer_Init();
	
	PWM_initialize();
	
	AD_Init();
	dac1.initialize();
	
	LCD_Init();//LCD��ʼ��
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	while(1){
		key_Scan();
		
		recognize_voice();
		
		switch_Ustate();
		voice_source_func();
	
		// PWM ������λ����
		if(Key1){							// 1��*
			PWM_set_compare(550);
			LED_GREEN_OFF;
			LED_RED_ON;
			LED_BLUE_OFF;
			pwm_flag=1;
		}
		if(Key2){							// 2��
			PWM_set_compare(600);
			LED_GREEN_ON;
			LED_RED_OFF;
			LED_BLUE_OFF;
			pwm_flag=2;
		}
		if(Key3){							// 3��
			PWM_set_compare(700); 

			LED_GREEN_OFF;
			LED_RED_OFF;
			LED_BLUE_ON;
			pwm_flag=3;
		}
		if(Key4){						// 4��
			PWM_set_compare(790);
			LED_GREEN_OFF;
			LED_RED_OFF;
			LED_BLUE_OFF;
			pwm_flag=4;
		}
		
		if(Key5){				// ����������
			cnt++;
			if(cnt%2==0){
				Ustate=false;
				open_U();
			}else{
				Ustate=true;
				PWM_set_compare(1125);
				open_U();
			}
		}
		
		if(Key6){			// �л���������      1 ->  human      2 -> music
			shield_type=1;
		}
		
		if(Key7){
			shield_type =2;
		}
		
//		if(Key8){		// �Զ�ʶ��
//			auto_state = true;
//			auto_voice_source_func();
//		}
//	
//		if(Key9){
//			auto_state = false;
//			open_U();
//		}
	
	}
}


// FFT main
void FFT(void)
{
		arm_cfft_radix4_instance_f32 scfft;
		
		arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,fft_flag,fft_reverse);		// ��ʼ���ṹ��   0: ����仯   1��������ת���

		for(int i=0;i<FFT_LENGTH;i++)
		{
			lBufInArray[i*2]= (float) ad_data[i]*3.3/4096;
			lBufInArray[i*2+1]=0;
		}
		
		arm_cfft_radix4_f32(&scfft,lBufInArray);	// fft����  ʱ���ź�תΪƵ���ź�
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
				arm_max_f32(lBufOutArray, FFT_LENGTH, &Max, &index);
				if (index > (FFT_LENGTH/2)){
						index = 1024- index;
				}
				one_part_index[i]=index;
				lBufOutArray[index] = 0;
				lBufOutArray[1024 - index] = 0;
		}
		  low_high_sort(one_part_index,3);
}

// ��������������ֵ
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ��С����������
void low_high_sort(int *input, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (input[j] > input[j + 1]) {
                swap(&input[j], &input[j + 1]);
            }
        }
    }
}

// �õ�input_recognize��ֵ��voice_Detection(float *input) ��
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

// �õ���ֵ
void Window_threshold(){
	  float mean = calculate_mean(lBufOutArray, FFT_LENGTH);
    float std_dev = calculate_std_dev(lBufOutArray, FFT_LENGTH, mean);

    // ������ֵΪ��ֵ���ϱ�׼��
    SOME_THRESHOLD = mean +  std_dev;
}


//  �ж�������Ƶ
void sliding_window_fft_analysis() {
	Window_threshold();
	if(SOME_THRESHOLD>10){
		Vstate = true;
	}else{
		Vstate=false;
	}
}


// Ultrasonic
void switch_Ustate(void){
	// ����������
	LCD_ShowString(2,1,"Ultrasonic state:",WHITE,BLACK,12,0);
	if(Ustate==true){
			LCD_ShowString(110,1," ON",WHITE,BLACK,12,0);
	}else if(Ustate==false){
			LCD_ShowString(110,1,"OFF",WHITE,BLACK,12,0);
	}	
	LCD_ShowString(2,15,"Ultrasonic gear:",WHITE,BLACK,12,0);	
	LCD_ShowIntNum(100,15,pwm_flag,3,WHITE,BLACK,12);
	LCD_ShowString(2,30,"Output power:",WHITE,BLACK,12,0);	
	if(pwm_flag==0){
		LCD_ShowString(80,30,"Rated Power",WHITE,BLACK,12,0);
	}else if(pwm_flag!=0){
		LCD_ShowIntNum(80,30,pwm_flag,3,WHITE,BLACK,12);
		LCD_ShowString(100,30,"W               ",WHITE,BLACK,12,0);
	}
}


// �Զ�ʶ��
void auto_voice_source_func(void)
{
	if(Voice_Exist&&auto_state){
		open_U();
		LCD_ShowString(2,105,"model: AUTO ",WHITE,BLACK,12,0);
	}
}

// �ֶ�����
void voice_source_func(void)
{
	LCD_ShowString(2,60,"Voice state:",WHITE,BLACK,12,0);
	sliding_window_fft_analysis();
	if(Vstate){  // ʶ��
			Voice_Exist = true;
			recognize_voice();
			LCD_ShowString(80,60,"exist  ",WHITE,BLACK,12,0);
			//  �������������
			LCD_ShowString(2,75,"Voice source:",WHITE,BLACK,12,0);
		
			if(voice_source==0){
				LCD_ShowString(80,75,"music",WHITE,BLACK,12,0);
			}else if(voice_source==1){
				LCD_ShowString(80,75,"human",WHITE,BLACK,12,0);
			}
			
			//  ��������ѡ��
			if(shield_type==1){
					//  ��ǰ��������
					LCD_ShowString(2,90,"shield now:",WHITE,BLACK,12,0);	
					LCD_ShowString(70,90,"human    ",WHITE,BLACK,12,0);
			}else if(shield_type==2){
					//  ��ǰ��������
					LCD_ShowString(2,90,"shield now:",WHITE,BLACK,12,0);		
					LCD_ShowString(70,90,"music    ",WHITE,BLACK,12,0);	
			}
			else{
					LCD_ShowString(2,90,"Please select shield",WHITE,BLACK,12,0);
			}
			
			if(auto_state){
				LCD_ShowString(2,105,"model: AUTO ",WHITE,BLACK,12,0);
			}else{
				LCD_ShowString(2,105,"model:Manual",WHITE,BLACK,12,0);
			}
	}
	else if(!Vstate)
		{
			if(auto_state){
				open_U();
			}
			Voice_Exist = false;
			LCD_ShowString(80,60,"Unexist",WHITE,BLACK,12,0);
			LCD_ShowString(2,75,"                    ",WHITE,BLACK,12,0);
			LCD_ShowString(2,90,"                    ",WHITE,BLACK,12,0);
	}
}

void gpio_delay_ms(uint32_t ms)
{
    SysTick_Config(SystemCoreClock / 1000); // ����SysTickÿ1ms�ж�һ��
    for(uint32_t i = 0; i < ms; i++)
    {
        while(! (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // �ȴ�SysTick������0
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // �ر�SysTick
}

void open_U(){
		dac1.set_voltage(2.0);
		gpio_delay_ms(200);
		dac1.set_voltage(0);
		gpio_delay_ms(200);
}