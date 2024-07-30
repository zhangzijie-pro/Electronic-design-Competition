/*
 * FILE                : exti.h
 * DESCRIPTION         : This file is for exti.c
 * Author              : ysloveivy
 * Copyright           :
 *
 * History
 * --------------------
 * Rev                 : 0.00
 * Date                : 11/21/2015
 *
 * create.
 * --------------------
 */
#ifndef __exti_h__
#define __exti_h__

//--------------------------- Define ---------------------------//
#define KEY_INPUT  GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_15)
//----------------------- Include files ------------------------//

//-------------------------- Typedef ---------------------------//
typedef struct {
	int (* initialize)(void);
}EXTI_T;

//--------------------------- Extern ---------------------------//
extern EXTI_T exti;

#endif //__led_h__
