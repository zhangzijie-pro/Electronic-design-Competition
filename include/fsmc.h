/*
 * FILE								: fsmc.h
 * DESCRIPTION				: This file is iCore3 fsmc file header.
 * Author							: XiaomaGee@Gmail.com
 * Copyright					:
 *
 * History
 * --------------------
 * Rev								: 0.00
 * Date								: 01/03/2016
 *
 * create.
 * --------------------
 */
//------------------------define---------------------------//
#ifndef __fsmc_h__
#define __fsmc_h__

//---------------------Include files-----------------------//

//----------------------- Define --------------------------//
#define fpga_write(offset,data)	*((volatile unsigned short int *)(0x60000000 + (offset << 17))) = data

#define fpga_read(offset)	*((volatile unsigned short int *)(0x60000000 + (offset << 17)))

//----------------- Typedef -----------------------------//
typedef struct{
	int (* initialize)(void);
}FSMC_T;

//---------------- Extern -------------------------------//

extern FSMC_T fsmc;

#endif //__fsmc_h__
