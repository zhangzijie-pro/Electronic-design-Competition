# include "bsp_key.h"
# include "Timer.h"
# include "LED.h"
# include "usart.h"

# define KEY_IN1    PGin(0)
# define KEY_IN2    PGin(1)
# define KEY_IN3    PHin(6)
# define KEY_IN4    PHin(7)

# define KEY_OUT1   PHout(8)
# define KEY_OUT2   PHout(9)
# define KEY_OUT3   PHout(10)
# define KEY_OUT4   PHout(11)

static _KEYS_ key[4][4] = {0, 0};

/**
 * @brief 按键初始化
 * 
 */
void key_Init(void){

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOH, &GPIO_InitStructure);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOH, &GPIO_InitStructure);

    KEY_OUT1 = KEY_OUT2 = KEY_OUT3 = KEY_OUT4 = 1;
}

/**
 * @brief 按键扫描
 * 
 */
void key_Scan(void){

    static uint32_t key_timers = 5;
    static uint8_t out = 0;

    if (Timer_get_sub(key_timers) == 0){
        uint8_t sta[4];

        switch(out){
            case 0: KEY_OUT1 = 0; KEY_OUT4 = 1; break;
            case 1: KEY_OUT2 = 0; KEY_OUT1 = 1; break;
            case 2: KEY_OUT3 = 0; KEY_OUT2 = 1; break;
            case 3: KEY_OUT4 = 0; KEY_OUT3 = 1; break;
        }

        sta[0] = KEY_IN1;
        sta[1] = KEY_IN2;
        sta[2] = KEY_IN3;
        sta[3] = KEY_IN4;

        for (uint8_t i = 0; i < 4; i ++){
            switch(key[out][i].judge){
                case 0 :{
                    if (sta[i] == 0){
                        key[out][i].judge = 1;
                    }
                }
                break;
                case 1 :{
                    if (sta[i] == 0){
                        key[out][i].judge = 2;
                    }
                    else {
                        key[out][i].judge = 0;
                    }
                }
                break;
                case 2 :{
                    if (sta[i] == 1){
                        key[out][i].judge = 0;
                        key[out][i].signal_flag = 1;
                    }
                }
                break;
            }   
        }

        out ++;
        out %= 4;
        key_timers = Timer_get() + 5;
    }
    if (Key16){
        NVIC_SystemReset();
    }
}

/**
 * @brief 读按键状态
 * 
 * @param row       按键行
 * @param column    按键列
 * @return uint8_t  1 表示按键按下  0 表示按键未按下
 */
uint8_t key_Read(uint8_t row, uint8_t column){
    if (key[row][column].signal_flag){
        key[row][column].signal_flag = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief 按键测试函数
 * 
 */
void key_Test(void){

    for (uint8_t i = 0; i < 4; i ++){
        for (uint8_t j = 0; j < 4; j ++){
            if (key[i][j].signal_flag){
								usart4.printf("press:%d",i*4+j+1);
                key[i][j].signal_flag = 0;
            }
        }
    }
}



/*
main -> func_code:

int main(){
	key_Init();
	Timer_Init();
	TIM10_NVIC_Configuration();
	led.initialize();
	LED_RED_ON;
	
	
	while(1){
		key_Scan();
		key_Test();
	}
}
*/