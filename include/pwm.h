/*
 * FILE                : pwm.h
 * DESCRIPTION         : This file is for pwm.c
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
#ifndef __pwm_h__
#define __pwm_h__

//--------------------------- Define ---------------------------//

//----------------------- Include files ------------------------//

//-------------------------- Typedef ---------------------------//
typedef struct {
	int (* initialize)(void);
	int (* set_compare)(int);
}PWM_T;

//--------------------------- Extern ---------------------------//
extern PWM_T pwm;

#endif //__led_h__
