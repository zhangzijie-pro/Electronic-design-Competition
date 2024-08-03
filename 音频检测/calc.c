
//以下是C代码
#include <stdio.h>
#include <stdlib.h>

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

int main() {
    // input就是FFT输出的top3序号的值，累计三组
    float input[9] = { \
        3.0, 4.0, 5.0, \
        3.0, 4.0, 5.0, \
        3.0, 4.0, 5.0 \
    };
    //以下为模型参数
    float conv1_weight[36] = {-0.15909916162490845,-0.008416073396801949,0.2429903894662857,-0.16311486065387726,-0.10580400377511978,-0.0935896635055542,-0.0662730410695076,0.05698031559586525,0.3303470015525818,0.046729184687137604,0.0856015533208847,-0.11863432079553604,-0.23057231307029724,-0.059863414615392685,-0.006049585063010454,-0.018405506387352943,-0.20528613030910492,0.015395033173263073,-0.22991962730884552,-0.12861588597297668,-0.10336729139089584,-0.17207661271095276,0.022556019946932793,0.48650944232940674,-0.16591346263885498,0.10794887691736221,-0.030371997505426407,-0.052660077810287476,-0.18202324211597443,-0.05889785289764404,0.007683710195124149,0.04116180166602135,-0.16588670015335083,0.15377984941005707,0.057808805257081985,-0.11910209059715271};
    float conv1_bias[4] = {-2.1894240379333496,-0.10507945716381073,-1.9382234811782837,0.1403675675392151};
    float conv2_weight[8] = {0.5853970646858215,-0.06548965722322464,0.5283345580101013,-0.06061948090791702,-0.49538564682006836,-0.014937086962163448,-0.8133894205093384,0.19766469299793243};
    float conv2_bias[2] = {-0.4006880521774292,0.3992657959461212};

    float conv1_output[4] = {0};
    float act_output[4] = {0};
    float conv2_output[2] = {0.0};
    bool is_music = false;

    conv1(input, conv1_weight, conv1_bias, conv1_output);
    act(conv1_output, act_output);
    conv2(act_output, conv2_weight, conv2_bias, conv2_output);

    if (conv2_output[0] > conv2_output[1]) {
        is_music = true;
    }

    if (is_music) {
        printf("music\n");
    } else {
        printf("voice\n");
    }

    return 0;

}
