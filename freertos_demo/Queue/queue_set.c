/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS ��ֲʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   128
TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128
TaskHandle_t    task1_handler;
void task1( void * pvParameters );


/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
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
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
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
    taskENTER_CRITICAL();               /* �����ٽ��� */
		/*���м�����*/
		queueset_handle = xQueueCreateSet(2);
		if(queueset_handle!=NULL){
			printf("queue set create successful\r\n");
		}
		/*���д���*/
		queue_handle = xQueueCreate(1,sizeof(uint8_t));
		if(queue_handle!=NULL){
			printf("queue create successful\r\n");
		}
		
		/*������ֵ�ź���*/
		semphore_handle = xSemaphoreCreateBinary();
		if(semphore_handle!=NULL){
			printf("semphore create successful\r\n");
		}
		
		// ������ݵ����м�
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
    taskEXIT_CRITICAL();                /* �˳��ٽ��� */
}

/* ����һ���ͷ��ź���������з������� */
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


/* ����� ��ȡ���м� */
void task2( void * pvParameters )
{
		QueueSetMemberHandle_t member_handle;
		uint8_t key;
		while(1)
    {
			// �Ӷ��м��л�ȡ���� ���ؾ��
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