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
TaskHandle_t    low_task_handler;
void low_task( void * pvParameters );


/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128
TaskHandle_t    middle_task_handler;
void middle_task( void * pvParameters );


/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128
TaskHandle_t    high_task_handler;
void high_task( void * pvParameters );

/******************************************************************************************************/
QueueHandle_t mutex_semphore_handle;
/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    mutex_semphore_handle = xSemaphoreCreateMutex();    /* 创建互斥信号量，并且主动释放一次信号量 */
    if(mutex_semphore_handle != NULL)
    {
        printf("互斥信号量创建成功！！！\r\n");
    }
    
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
    xTaskCreate((TaskFunction_t         )   low_task,
                (char *                 )   "low_task",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &low_task_handler );
                
    xTaskCreate((TaskFunction_t         )   middle_task,
                (char *                 )   "middle_task",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &middle_task_handler );
                
    xTaskCreate((TaskFunction_t         )   high_task,
                (char *                 )   "high_task",
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIO,
                (TaskHandle_t *         )   &high_task_handler );
                             
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/* 任务一，低优先级任务 */
void low_task( void * pvParameters )
{
    while(1) 
    {
        printf("low_task获取信号量\r\n");
        xSemaphoreTake(mutex_semphore_handle,portMAX_DELAY);
        printf("low_task正在运行！！！\r\n");
        delay_ms(3000);
        printf("low_task释放信号量\r\n");
        xSemaphoreGive(mutex_semphore_handle); 
        vTaskDelay(1000);
    }
}

/* 任务二，中优先级任务 */
void middle_task( void * pvParameters )
{
    while(1)
    {
        printf("middle_task正在运行！！！\r\n");
        vTaskDelay(1000);
    }
}

/* 任务三，高优先级任务 */
void high_task( void * pvParameters )
{
    while(1)
    {
        printf("high_task获取信号量\r\n");
        xSemaphoreTake(mutex_semphore_handle,portMAX_DELAY);
        printf("high_task正在运行！！！\r\n");
        delay_ms(1000);
        printf("high_task释放信号量\r\n");
        xSemaphoreGive(mutex_semphore_handle); 
        vTaskDelay(1000);
    }
}


