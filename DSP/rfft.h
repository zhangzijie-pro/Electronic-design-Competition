#ifndef __RFFT_H__
#define __RFFT_H__

#include "arm_math.h"
// fft

void conv1(float *input, float *conv1_weight, float *conv1_bias, float *conv1_output);
void act(float *conv1_output, float *act_output);
void conv2(float *act_output, float *conv2_weight, float *conv2_bias, float *conv2_output);
void voice_Detection(float *input);
float calculate_mean(float *array, int length);
float calculate_std_dev(float *array, int length, float mean);

extern int voice_source;    // …˘“Ù¿¥‘¥  “Ù¿÷/»À
#endif