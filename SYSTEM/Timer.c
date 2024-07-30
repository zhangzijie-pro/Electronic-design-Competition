# include "Timer.h"

static uint32_t global_times = 0;

void Timer_Init(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Prescaler = 42-1;
    TIM_InitStructure.TIM_Period = 100-1;
    TIM_InitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM10, &TIM_InitStructure);

    TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM10, ENABLE);
		TIM10_NVIC_Configuration();
}

void TIM10_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能TIM10中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 中断抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 中断子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            // 使能中断
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 获取全局时间
 * 
 * @return uint32_t 
 */
uint32_t Timer_get(void){

    return global_times;
}

/**
 * @brief 判断时间周期
 * 
 * @param c 设置的时间周期
 * @return uint32_t c与时间周期剩余的时间
 */
uint32_t Timer_get_sub(uint32_t c){

    if(c > global_times)
		c -= global_times;
	else
		c = 0;
	return c;
}

/**
 * @brief 中断函数
 * 
 */
void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update) == SET) {   
        TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
		global_times++;
	}
}
