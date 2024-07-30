#include "stm32f4xx.h"
#include "led.h"
#include "usart.h"
#include "arm_math.h"
#include "Timer.h"
#include "delay.h"
#include "ad.h"
#include "arm_math.h"
#include "bsp_key.h"
#include "dac.h"
#include "pwm.h"
#include "rfft.h"

#define Fs 100000
#define FFT_LENGTH (1024)
#define SAMPLE_RATE 40000


float32_t lBufInArray[FFT_LENGTH*2]={0};
float32_t lBufOutArray[FFT_LENGTH/2]={0};
float32_t lBufOut_To_InArray[FFT_LENGTH]={0};
float32_t rlBufOutArray[FFT_LENGTH]={0};
float magnitude[FFT_LENGTH];         // ��������
float phase[FFT_LENGTH];            // ��λ����
float frequency[FFT_LENGTH];       // Ƶ������
int fft_flag = 0;  // ����仯
int fft_reverse = 1; // ������ת���
float max_magnitude;
int max_mag_index;
float avgPhase=0.0;
float dominantFrequency;


void calculateFFT(void);
void Feature_fft(void);
void FFT(void);

int main(){
	LED_initialize();
	usart4.initialize(115200);
	key_Init();
	AD_Init();
	
	while(1){
//			for(int i=0;i<FFT_LENGTH;i++){
//				float v = ad_data[i]*3.3/4096;
//				usart4.printf("v:%f\r\n",v);
//			}
		FFT();
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
		
		for (int i = 1; i < FFT_LENGTH/2; i ++){
			lBufOutArray[i] /= 512;
		}
	
		//calculateFFT();
		float32_t Max = 0;
		uint32_t  index = 0;
		for (uint8_t i = 0; i < 5; i ++){
				arm_max_f32(lBufOutArray, FFT_LENGTH/2, &Max, &index);
				if (index > (FFT_LENGTH/2)){
						index = 1024 - index;
				}
				usart4.printf("Max: %f index: %d\r\n", Max, index);
				lBufOutArray[index] = 0;
				lBufOutArray[1024 - index] = 0;
		}
		usart4.printf("------------------------\r\n");
//		for(int16_t i = 1; i < FFT_LENGTH/2; i++)
//		{
//			usart4.printf("A:%f\r\n", lBufOutArray[i]);
//		}
}

// Resver
void Resver_FFT(void)
{
	arm_cfft_radix4_instance_f32 scfft;
	
	// resver fft
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,!fft_flag,fft_reverse);
	
	// ����
	arm_cfft_radix4_f32(&scfft,lBufOut_To_InArray);
	
	// ת��ʱ��
	arm_cmplx_mag_f32(lBufOut_To_InArray,rlBufOutArray,FFT_LENGTH);
	
	for(int i=1;i<FFT_LENGTH;i++){
		usart4.printf("A%f\r\n",rlBufOutArray[i]);
	}
}

void calculateFFT(void)
{
	for(int i=0;i<FFT_LENGTH;i++)
	{
		lBufOut_To_InArray[i*2]=lBufOutArray[i];
		lBufOut_To_InArray[i*2+1]=0;
	}
	for (int i = 0; i < FFT_LENGTH; i++) {
		float real = lBufOutArray[2 * i];          // ʵ��
		float imag = lBufOutArray[2 * i + 1];      // �鲿

		// �������
		magnitude[i] = sqrt(real * real + imag * imag);

		// ������λ
		phase[i] = atan2(imag, real);

		// ����Ƶ��
		frequency[i] = i * (SAMPLE_RATE / FFT_LENGTH);
   }
	Feature_fft();
}

void Feature_fft(void)
{
	// ������
	max_magnitude = magnitude[0];
	max_mag_index=0;
	for(int i=0;i<FFT_LENGTH;i++){
		if(magnitude[i]>max_magnitude){
			max_magnitude = magnitude[i];
			max_mag_index=i;
		}
	}
	
	// ��λ��Χ��ƽ��ֵ
	for (int i = 0; i < FFT_LENGTH; i++) {
			avgPhase += phase[i];
	}
	avgPhase /= FFT_LENGTH;
	
	// ��ҪƵ��
	float dominantFrequency = frequency[max_mag_index];
	
	usart4.printf("max mag: %f, %f\r\n",max_magnitude,avgPhase);
}