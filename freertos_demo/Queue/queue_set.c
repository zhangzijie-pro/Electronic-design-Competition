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
#include "semphr.h"
#include "queue.h"
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
QueueHandle_t queueset_handle;
QueueHandle_t queue_handle;
QueueHandle_t semphore_handle;
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
		/*队列集创建*/
		queueset_handle = xQueueCreateSet(2);
		if(queueset_handle!=NULL){
			printf("queue set create successful\r\n");
		}
		/*队列创建*/
		queue_handle = xQueueCreate(1,sizeof(uint8_t));
		if(queue_handle!=NULL){
			printf("queue create successful\r\n");
		}
		
		/*创建二值信号量*/
		semphore_handle = xSemaphoreCreateBinary();
		if(semphore_handle!=NULL){
			printf("semphore create successful\r\n");
		}
		
		// 添加内容到队列集
		xQueueAddToSet(queue_handle,queueset_handle);
		xQueueAddToSet(semphore_handle,queueset_handle);
		
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

/* 任务一，释放信号量，向队列发送内容 */
void task1( void * pvParameters )
{
		uint8_t key=0;
		BaseType_t err=0;
    while(1)
    {
			key = key_scan(0);
			if(key==KEY0_PRES){
				err = xQueueSend(queue_handle,&key,portMAX_DELAY);
				if(err==pdPASS){
					printf("send date to queue successful\r\n");
				}
			}else if(key==KEY1_PRES){
				err = xSemaphoreGive(semphore_handle);
				if(err==pdPASS){
					printf("give semaphore successful\r\n");
				}
			}
			vTaskDelay(10);
    }
}


/* 任务二 获取队列集 */
void task2( void * pvParameters )
{
		QueueSetMemberHandle_t member_handle;
		uint8_t key;
		while(1)
    {
			// 从队列集中获取队列 返回句柄
			member_handle = xQueueSelectFromSet(queueset_handle,portMAX_DELAY);
			if(member_handle == queue_handle){
				xQueueReceive(member_handle,&key,portMAX_DELAY);
				printf("Get queue date : %d\r\n",key);
			}else if(member_handle == semphore_handle){
				xSemaphoreTake(member_handle,portMAX_DELAY);
				printf("Take Semaphore successful\r\n");
			}
		}
}