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
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO      4                   /* 任务优先级 */
#define TASK3_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task3Task_Handler;  /* 任务句柄 */
void task3(void *pvParameters);             /* 任务函数 */
/******************************************************************************************************/
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{	
	xTaskCreate(
		(TaskFunction_t) start_task,
		(char *) "_start_task",
		(configSTACK_DEPTH_TYPE) START_STK_SIZE,
		(void *) NULL,
		(UBaseType_t) START_TASK_PRIO,
		(TaskHandle_t *) &StartTask_Handler
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
			
		vTaskDelete(NULL); 		// 删除当前任务
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
		
		vTaskDelay(10);
	}
}