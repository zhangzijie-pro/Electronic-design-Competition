#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* 动态创建	*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
StackType_t 						StartTask_stack[START_STK_SIZE];
StaticTask_t 						start_task_tcb;
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
StackType_t 						Task1_stack[TASK1_STK_SIZE];
StaticTask_t 						task1_tcb;
void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
StackType_t 						Task2_stack[TASK2_STK_SIZE];
StaticTask_t 						task2_tcb;
void task2(void *pvParameters);             /* 任务函数 */

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO      4                   /* 任务优先级 */
#define TASK3_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task3Task_Handler;  /* 任务句柄 */
StackType_t 						Task3_stack[TASK3_STK_SIZE];
StaticTask_t 						task3_tcb;
void task3(void *pvParameters);             /* 任务函数 */


/*	静态创建	*/


/* 空闲任务内存分配 -> 静态使用	*/
StaticTask_t idle_task_tcb;
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];
void vApplicationGetIdleTaskMemory(
		StaticTask_t ** ppxIdleTaskTCBBuffer,
		StackType_t	** ppxIdleTaskStackBuffer,
		uint32_t * pulIdleTaskStackSize
){
	* ppxIdleTaskTCBBuffer = &idle_task_tcb;
	* ppxIdleTaskStackBuffer = idle_task_stack;
	* pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* 软件定时器内存分配 */
StaticTask_t timer_task_tcb;
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];
void vApplicationGetTimerTaskMemory(
		StaticTask_t ** ppxTimerTaskTCBBuffer,
		StackType_t	** ppxTimerTaskStackBuffer,
		uint32_t * pulTimerTaskStackSize
){
	* ppxTimerTaskTCBBuffer = &timer_task_tcb;
	* ppxTimerTaskStackBuffer = timer_task_stack;
	* pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/******************************************************************************************************/
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{	
	// 动态
	xTaskCreate(
		(TaskFunction_t) start_task,
		(char *) "_start_task",
		(configSTACK_DEPTH_TYPE) START_STK_SIZE,
		(void *) NULL,
		(UBaseType_t) START_TASK_PRIO,
		(TaskHandle_t *) &StartTask_Handler
	);
	// 静态
	StartTask_Handler = xTaskCreateStatic(  (TaskFunction_t) start_task,
																					 (char *) "_start_task",
																					 (uint32_t) START_STK_SIZE,
																					 (void *) NULL,
																					 (UBaseType_t) START_TASK_PRIO,
																					 (StackType_t *) StartTask_stack,
																					 (StaticTask_t *) &start_task_tcb 
										 );
  vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */ 
void start_task(void *pvParameters){
	while(1){
		taskENTER_CRITICAL();		// 进入临界区
		
		/* 动态创建	*/
		xTaskCreate(
			(TaskFunction_t) task1,
			(char *) "_task1",
			(configSTACK_DEPTH_TYPE) TASK1_STK_SIZE,
			(void *) NULL,
			(UBaseType_t) TASK1_PRIO,
			(TaskHandle_t *) &Task1Task_Handler
		);
		xTaskCreate(
			(TaskFunction_t) task2,
			(char *) "_task2",
			(configSTACK_DEPTH_TYPE) TASK2_STK_SIZE,
			(void *) NULL,
			(UBaseType_t) TASK2_PRIO,
			(TaskHandle_t *) &Task2Task_Handler
		);
		xTaskCreate(
			(TaskFunction_t) task3,
			(char *) "_task3",
			(configSTACK_DEPTH_TYPE) TASK3_STK_SIZE,
			(void *) NULL,
			(UBaseType_t) TASK3_PRIO,
			(TaskHandle_t *) &Task3Task_Handler
		);
			
			
		/* 静态创建	*/
		Task1Task_Handler = xTaskCreateStatic( (TaskFunction_t) task1,
									 (char *) "_task1",
									 (uint32_t) TASK1_STK_SIZE,
									 (void *) NULL,
									 (UBaseType_t) TASK1_PRIO,
									 (StackType_t *) Task1_stack,
									 (StaticTask_t *) &task1_tcb 
		);
		Task2Task_Handler = xTaskCreateStatic( (TaskFunction_t) task2,
									 (char *) "_task2",
									 (uint32_t) TASK2_STK_SIZE,
									 (void *) NULL,
									 (UBaseType_t) TASK2_PRIO,
									 (StackType_t *) Task2_stack,
									 (StaticTask_t *) &task2_tcb 
		);
		Task3Task_Handler = xTaskCreateStatic( (TaskFunction_t) task3,
									 (char *) "_task3",
									 (uint32_t) TASK3_STK_SIZE,
									 (void *) NULL,
									 (UBaseType_t) TASK3_PRIO,
									 (StackType_t *) Task3_stack,
									 (StaticTask_t *) &task3_tcb 
		);
			
		//vTaskDelete(NULL); 		动态可： 删除当前任务
		vTaskDelete(StartTask_Handler);		// 静态删除
		taskEXIT_CRITICAL();	// 退出临界区
	}
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task1(void *pvParameters){
	while(1){
		printf("task1 running \r\n!");
		LED0_TOGGLE();
		vTaskDelay(500);  // 500ms
	}
}

/**
 * @brief       task2
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task2(void *pvParameters){
	while(1){
		printf("task2 running \r\n!");
		LED0_TOGGLE();
		vTaskDelay(500);  // 500ms
	}
}

/**
 * @brief       task3
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task3(void *pvParameters){
	uint8_t key=0;
	while(1){
		printf("task3 running \r\n!");
		key=key_scan(0);
		if(key==KEY0_PRES){
			if(Task1Task_Handler!=NULL){		// 判断摁下前是否摁过
				printf("del task1 \r\n!");
				vTaskDelete(Task1Task_Handler);
				Task1Task_Handler=NULL;
			}
		}else if(key==KEY1_PRES){
			if(Task2Task_Handler!=NULL){
				printf("del task2 \r\n!");
				vTaskDelete(Task2Task_Handler);
				Task2Task_Handler=NULL;
			}
		}
		/*
		if(key==KEY0_PRES){
			vTaskSuspend(Task1Task_Handler);		// 挂起
		}else if(key==KEY1_PRES){
			vTaskResume(Task1Task_Handler);			// 恢复
		}
		*/
		
		vTaskDelay(10);
	}
}
