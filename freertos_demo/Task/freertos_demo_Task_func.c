/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 移植实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 F407电机开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./MALLOC/malloc.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );


/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    task2_handler;
void task2( void * pvParameters );
/******************************************************************************************************/

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    xTaskCreate((TaskFunction_t         )   start_task,
                (char *                 )   "start_task",
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   START_TASK_PRIO,
                (TaskHandle_t *         )   &start_task_handler );
    vTaskStartScheduler();
}


void start_task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区 */
    xTaskCreate((TaskFunction_t         )   task1,
                (char *                 )   "task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
                
    xTaskCreate((TaskFunction_t         )   task2,
                (char *                 )   "task2",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &task2_handler );
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，实现LED0每500ms翻转一次 */
void task1( void * pvParameters )
{
    while(1)
    {
        LED0_TOGGLE();
        vTaskDelay(500);
    }
}


/* 任务二 */
char task_buff[500];
void task2( void * pvParameters )
{
		UBaseType_t priority_num = 0;
		UBaseType_t task_num = 0;
		UBaseType_t task_num2 = 0;
		TaskStatus_t * status_array;	
		TaskStatus_t * status_array2;
		TaskHandle_t task_handler = 0;
		UBaseType_t task_stack_min=0;
		eTaskState state = 0;
		
		vTaskPrioritySet(task2_handler,30);			// 修改优先级
		priority_num = uxTaskPriorityGet(task1_handler);		// 获取优先级 NULL 为自己
		printf("task1 %1d\r\n",priority_num);
    
		task_num = uxTaskGetNumberOfTasks();		// 获取任务数量
	
		// 获取所有任务状态信息
		status_array = mymalloc(SRAMIN,(sizeof(TaskStatus_t)* task_num));
		task_num2 = uxTaskGetSystemState(status_array,task_num,NULL);
		printf("name\t\t priority \t\t id\r\n");
		for(int i=0; i< task_num2;i++){
			printf("%s\t\t%1d\t%1d\r\n",
						status_array[i].pcTaskName,
						status_array[i].uxCurrentPriority,
						status_array[i].xTaskNumber
			);
		}
		
		// 获取单个任务状态信息
		status_array2 = mymalloc(SRAMIN,(sizeof(TaskStatus_t)));
		vTaskGetInfo(task1_handler,status_array2,pdTRUE,eInvalid);
		printf("name:%s\r\n",status_array2->pcTaskName);
		printf("priority:%1d\r\n",status_array2->uxCurrentPriority);
		printf("id:%1d\r\n",status_array2->xTaskNumber);
		printf("status:%d\r\n",status_array2->eCurrentState);
		
		task_handler = xTaskGetHandle("task1");
		printf("handler:%#x\r\n",(int)task_handler);
		printf("task1_handler:%#x\r\n",(int)task1_handler);
		
		// 获取某运行任务的状态
		state = eTaskGetState(task2_handler);
		printf("task2_state:%d\r\n",state);
		
		// 获取所有任务信息
		vTaskList(task_buff);
		printf("%s\r\n",task_buff);
		
		while(1)
    {
				// 历史剩余最小堆栈位
				task_stack_min = uxTaskGetStackHighWaterMark(task2_handler);
				printf("stack:%1d",(int)task_stack_min);
        vTaskDelay(1000);
    }
}

