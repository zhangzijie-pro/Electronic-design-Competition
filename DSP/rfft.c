#include "rfft.h"
#include "arm_math.h"

//以下是C代码
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int voice_source;

void conv1(float *input, float *conv1_weight, float *conv1_bias, float *conv1_output) {
    for (int i = 0; i < 4; i++) {
        float conv1 = 0;
        for (int j = 0; j < 9; j++) {
            conv1 += input[j] * conv1_weight[i*9+j];
        }
        conv1 += conv1_bias[i];
        conv1_output[i] = conv1;
    }
}

void act(float *conv1_output, float *act_output) {
    for (int i = 0; i < 4; i++) {
        if (conv1_output[i] > 0) {
            act_output[i] = conv1_output[i];
        } else {
            act_output[i] = 0;
        }
    }
}

void conv2(float *act_output, float *conv2_weight, float *conv2_bias, float *conv2_output) {
    for (int i = 0; i < 2; i++) {
        float conv2 = 0;
        for (int j = 0; j < 4; j++) {
            conv2 += act_output[j] * conv2_weight[i*4+j];
        }
        conv2 += conv2_bias[i];
        conv2_output[i] = conv2;
    }
}

void voice_Detection(float *input) {
    // input就是FFT输出的top3序号的值，累计三组
//    float input[9] = { \
//        3.0, 4.0, 5.0, \
//        3.0, 4.0, 5.0, \
//        3.0, 4.0, 5.0 \
//    };
    //以下为模型参数
    float conv1_weight[36] = {-0.2724142074584961,-0.25513792037963867,0.4341772496700287,-0.2760794758796692,-0.20988984405994415,0.38190317153930664,-0.29966798424720764,-0.21790088713169098,0.41909661889076233,-0.33638909459114075,0.0887327492237091,-0.33819296956062317,-0.04218309372663498,-0.09570443630218506,0.4896083176136017,-0.08728881925344467,0.022566966712474823,0.41850170493125916,0.05632603541016579,-0.1549403965473175,0.43814587593078613,-0.3271814286708832,0.049282483756542206,-0.27144238352775574,-0.050113871693611145,-0.13919149339199066,0.5154788494110107,-0.09784085303544998,-0.11591261625289917,0.4904712438583374,-0.028288966044783592,-0.12863565981388092,0.46313080191612244,-0.13798804581165314,0.06191118806600571,-0.3404494822025299};
    float conv1_bias[4] = {-1.2870845794677734,-2.1978414058685303,-2.1863582134246826,-2.1322779655456543};
    float conv2_weight[8] = {0.2090248465538025,0.20907816290855408,0.16277062892913818,0.1743628978729248,-0.14075712859630585,-0.12825648486614227,-0.19994868338108063,-0.17333313822746277};
    float conv2_bias[2] = {-0.8803797960281372,0.8870134353637695};

    float conv1_output[4] = {0};
    float act_output[4] = {0};
    float conv2_output[2] = {0.0};
    bool is_music = false;

    conv1(input, conv1_weight, conv1_bias, conv1_output);
    act(conv1_output, act_output);
    conv2(act_output, conv2_weight, conv2_bias, conv2_output);
    //验证此处conv2_output输出为[-0.8803797960281372, 0.8870134353637695]
    if (conv2_output[0] > conv2_output[1]) {
        is_music = true;
    }

    if (is_music) {
        voice_source = 0;
    } else {
        voice_source = 1;
    }

}


float calculate_mean(float *array, int length) {
    float sum = 0;
    for (int i = 0; i < length; i++) {
        sum += array[i]*4096/3.3;
    }
    return sum / length;
}

float calculate_std_dev(float *array, int length, float mean) {
    float sum = 0;
    for (int i = 0; i < length; i++) {
        sum += pow(array[i]*4096/3.3 - mean, 2);
    }
    return sqrt(sum / length);
}
