#ifndef __ADS8361_H
#define __ADS8361_H	 
#include "sys.h"
#include "stm32f4xx.h"
/*************************************************************************************************************/
//ADC值=32767 = REF ,
#define REF		2500//参考电压2500mV
#define ADS8361_LSB  REF/32767

#define CH_AB0	0/**/
#define CH_AB1	1/**/

#define Mode_I 		0/**/
#define Mode_II 	1/**/
#define Mode_III 	2/**/
#define Mode_IV 	3/**/


#define CLOCK		PCout(3)     //
#define CS			PCout(4)     //
#define RD			PCout(12)    //
#define CONVST	PCout(11)    //

/********************具体模式选择请查看ads8361数据手册**************************/
#define M1			PCout(8)     //串行输出脚选择。当M1=0时，DATA_A和DATA_B都被选择用于数据传输。 当 M1=1时，DATA_A为通道A和B数据，DATA_B	处于三态
#define M0			PCout(10)    //当M0=1时，选择四通道操作，所有四个通道都按顺序转换，A0和B0开始，然后是A1和B1。当 M0=0时，选择两通道操作并与A0一起操作。
#define A0			PCout(13)    //仅当M0=0时起作用,M0=0和A0=0，转换通道 A0和B0。当M0=0和A0=1时，转换通道A1和B1。

#define BUSY		PCin(5)    //
#define DATA_A	PCin(7)    //
#define DATA_B	PCin(6)    //

void ADS8361_IO_Init(void);	   //ADS8361 IO口初始化
void ADS8361_Init(uint8_t mode);	   //ADS8361初始化
void ADS8361_Mode_selection(uint8_t mode);//ADS8361模式选择

void ADS8361_ReadData(uint8_t A0_Select,uint16_t *data);//读ADS8361数据
void ADS8361_Read_ALLData(uint16_t *data);//读取ADS8361所有通道数据



#endif















