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
    float conv1_weight[36] = {0.15877380967140198,-0.048163823783397675,-0.10244785249233246,0.10698658227920532,-0.16398586332798004,-0.21286192536354065,-0.1990298181772232,0.03670421242713928,-0.024115007370710373,-0.21434637904167175,-0.17286677658557892,-0.10150738060474396,-0.23075591027736664,0.017695503309369087,-0.25572893023490906,-0.21016597747802734,0.12328990548849106,-0.0831630751490593,0.046895984560251236,-0.18896391987800598,0.2750101685523987,0.03528532013297081,-0.14967772364616394,0.24229267239570618,0.005977099761366844,-0.15671813488006592,0.2751006782054901,-0.11654232442378998,-0.08926359564065933,-0.017788905650377274,-0.10520598292350769,0.012897730804979801,-0.17505504190921783,0.0554022490978241,-0.05089716613292694,-0.011917398311197758};
    float conv1_bias[4] = {-0.14965559542179108,-0.020411493256688118,-2.1647465229034424,0.1456596553325653};
    float conv2_weight[8] = { -0.12396419793367386,-0.05989959463477135,1.071378469467163,0.16645938158035278,0.2731224596500397,0.12274488806724548,-1.071602463722229,-0.19036632776260376};
    float conv2_bias[2] = {-2.9994308948516846,2.9996719360351562};

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
