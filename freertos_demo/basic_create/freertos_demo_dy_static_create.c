#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS����*/

/* ��̬����	*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
StackType_t 						StartTask_stack[START_STK_SIZE];
StaticTask_t 						start_task_tcb;
void start_task(void *pvParameters);        /* ������ */

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO      2                   /* �������ȼ� */
#define TASK1_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
StackType_t 						Task1_stack[TASK1_STK_SIZE];
StaticTask_t 						task1_tcb;
void task1(void *pvParameters);             /* ������ */

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO      3                   /* �������ȼ� */
#define TASK2_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
StackType_t 						Task2_stack[TASK2_STK_SIZE];
StaticTask_t 						task2_tcb;
void task2(void *pvParameters);             /* ������ */

/* TASK3 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK3_PRIO      4                   /* �������ȼ� */
#define TASK3_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task3Task_Handler;  /* ������ */
StackType_t 						Task3_stack[TASK3_STK_SIZE];
StaticTask_t 						task3_tcb;
void task3(void *pvParameters);             /* ������ */


/*	��̬����	*/


/* ���������ڴ���� -> ��̬ʹ��	*/
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

/* ������ʱ���ڴ���� */
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
 * @brief       FreeRTOS������ں���
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{	
	// ��̬
	xTaskCreate(
		(TaskFunction_t) start_task,
		(char *) "_start_task",
		(configSTACK_DEPTH_TYPE) START_STK_SIZE,
		(void *) NULL,
		(UBaseType_t) START_TASK_PRIO,
		(TaskHandle_t *) &StartTask_Handler
	);
	// ��̬
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */ 
void start_task(void *pvParameters){
	while(1){
		taskENTER_CRITICAL();		// �����ٽ���
		
		/* ��̬����	*/
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
			
			
		/* ��̬����	*/
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
			
		//vTaskDelete(NULL); 		��̬�ɣ� ɾ����ǰ����
		vTaskDelete(StartTask_Handler);		// ��̬ɾ��
		taskEXIT_CRITICAL();	// �˳��ٽ���
	}
}

/**
 * @brief       task1
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void task3(void *pvParameters){
	uint8_t key=0;
	while(1){
		printf("task3 running \r\n!");
		key=key_scan(0);
		if(key==KEY0_PRES){
			if(Task1Task_Handler!=NULL){		// �ж�����ǰ�Ƿ�����
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
			vTaskSuspend(Task1Task_Handler);		// ����
		}else if(key==KEY1_PRES){
			vTaskResume(Task1Task_Handler);			// �ָ�
		}
		*/
		
		vTaskDelay(10);
	}
}