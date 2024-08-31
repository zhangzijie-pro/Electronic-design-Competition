/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS ��ֲʵ��
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
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

/* ����һ��ʵ��LED0ÿ500ms��תһ�� */
void task1( void * pvParameters )
{
    while(1)
    {
        LED0_TOGGLE();
        vTaskDelay(500);
    }
}


/* ����� */
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
		
		vTaskPrioritySet(task2_handler,30);			// �޸����ȼ�
		priority_num = uxTaskPriorityGet(task1_handler);		// ��ȡ���ȼ� NULL Ϊ�Լ�
		printf("task1 %1d\r\n",priority_num);
    
		task_num = uxTaskGetNumberOfTasks();		// ��ȡ��������
	
		// ��ȡ��������״̬��Ϣ
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
		
		// ��ȡ��������״̬��Ϣ
		status_array2 = mymalloc(SRAMIN,(sizeof(TaskStatus_t)));
		vTaskGetInfo(task1_handler,status_array2,pdTRUE,eInvalid);
		printf("name:%s\r\n",status_array2->pcTaskName);
		printf("priority:%1d\r\n",status_array2->uxCurrentPriority);
		printf("id:%1d\r\n",status_array2->xTaskNumber);
		printf("status:%d\r\n",status_array2->eCurrentState);
		
		task_handler = xTaskGetHandle("task1");
		printf("handler:%#x\r\n",(int)task_handler);
		printf("task1_handler:%#x\r\n",(int)task1_handler);
		
		// ��ȡĳ���������״̬
		state = eTaskGetState(task2_handler);
		printf("task2_state:%d\r\n",state);
		
		// ��ȡ����������Ϣ
		vTaskList(task_buff);
		printf("%s\r\n",task_buff);
		
		while(1)
    {
				// ��ʷʣ����С��ջλ
				task_stack_min = uxTaskGetStackHighWaterMark(task2_handler);
				printf("stack:%1d",(int)task_stack_min);
        vTaskDelay(1000);
    }
}
