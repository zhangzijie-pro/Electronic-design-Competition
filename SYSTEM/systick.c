// 定时

//--------------------------- Include ---------------------------//
#include "systick.h"
#include "..\fwlib\inc\stm32f4xx_syscfg.h"

//--------------------- Function Prototype ----------------------//
static int initialize(void);

//--------------------------- Variable --------------------------//
SYSTICK_T systick = {
	.initialize = initialize,
	.second_flag = 0
};
//--------------------------- Function --------------------------//

static int initialize(void)
{
	//定时10ms
	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}
	return 0;
}

void SysTick_Handler(void)
{
	static int counter = 0;
	
	if((counter ++ % 1000) == 0){
		systick.second_flag = 1;
	}
}


/*
func_code:

int main(void)
{
	static int work_status = 0;
	led.initialize();
	systick.initialize();
	while(1){
		if(systick.second_flag == 1){
			systick.second_flag = 0;
			work_status += 1;
			if(work_status > 2)work_status = 0;
		}
		switch (work_status){}
	}
}


*/