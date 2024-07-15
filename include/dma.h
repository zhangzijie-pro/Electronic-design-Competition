/*
 * FILE                : dma.h
 * DESCRIPTION         : This file is for dma.c
 * Author              : ysloveivy
 * Copyright           : 
 *
 * History
 * --------------------
 * Rev                 : 0.00
 * Date                : 11/29/2015
 *
 * create.
 * --------------------
 */
#ifndef __dma_h__
#define __dma_h__

//--------------------------- Define ---------------------------//

//----------------------- Include files ------------------------//

//-------------------------- Typedef----------------------------//
typedef struct {
	int (* initialize)(void);
}DMA_T;

//--------------------------- Extern ---------------------------//
extern DMA_T dma;

#endif //__led_h__
