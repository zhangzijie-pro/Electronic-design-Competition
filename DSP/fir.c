#include "fir.h"
#include "arm_math.h"
#include "usart.h"
#include "ad.h"


// fir
#define TEST_LENGTH_SAMPLES 320 // 采样点数
#define BLOCK_SIZE	32		// 调用一个fir处理的采样点数
#define NUM_TAPS		29	// 滤波器系数个数

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE; // 调用fir次数

static float32_t testInput_f32[TEST_LENGTH_SAMPLES];  // 采样点
static float32_t testOutput[TEST_LENGTH_SAMPLES];	// 滤波输出
static float32_t firState[BLOCK_SIZE+NUM_TAPS-1];	 // 状态


// 高通
const float32_t firCoeffs32LP[NUM_TAPS] = {
     0.0018157335, 0.001582013792,-6.107207639e-18,-0.003683975432,-0.008045346476,
  -0.008498443291,-1.277260999e-17,  0.01733288541,  0.03401865438,   0.0332348831,
  -4.021742543e-17, -0.06737889349,  -0.1516391635,  -0.2220942229,   0.7486887574,
    -0.2220942229,  -0.1516391635, -0.06737889349,-4.021742543e-17,   0.0332348831,
    0.03401865438,  0.01733288541,-1.277260999e-17,-0.008498443291,-0.008045346476,
  -0.003683975432,-6.107207639e-18, 0.001582013792,   0.0018157335
};





// fir
void fir_init()
{
	for(int i=0;i<TEST_LENGTH_SAMPLES;i++)
	{
		testInput_f32[i]=ad_data[i]*3.3/4096;
	}
}

void fir(){
	fir_init();
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t *inputF32, *outputF32;
	
	// init
	inputF32 = &testInput_f32[0];
	outputF32 = &testOutput[0];
	
	arm_fir_init_f32(&S,NUM_TAPS,(float32_t *)&firCoeffs32LP[0],&firState[0],blockSize);
	
	for (i = 0; i < numBlocks; i++) {
		arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}
	
	for(i=0;i<TEST_LENGTH_SAMPLES;i++){
		usart4.printf("fir:%f,%f\r\n",testOutput[i],testInput_f32[i]);
	}
}