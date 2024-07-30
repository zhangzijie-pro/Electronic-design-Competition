#ifndef __ADS8361_H
#define __ADS8361_H	 
#include "sys.h"
#include "stm32f4xx.h"
/*************************************************************************************************************/
//ADCֵ=32767 = REF ,
#define REF		2500//�ο���ѹ2500mV
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

/********************����ģʽѡ����鿴ads8361�����ֲ�**************************/
#define M1			PCout(8)     //���������ѡ�񡣵�M1=0ʱ��DATA_A��DATA_B����ѡ���������ݴ��䡣 �� M1=1ʱ��DATA_AΪͨ��A��B���ݣ�DATA_B	������̬
#define M0			PCout(10)    //��M0=1ʱ��ѡ����ͨ�������������ĸ�ͨ������˳��ת����A0��B0��ʼ��Ȼ����A1��B1���� M0=0ʱ��ѡ����ͨ����������A0һ�������
#define A0			PCout(13)    //����M0=0ʱ������,M0=0��A0=0��ת��ͨ�� A0��B0����M0=0��A0=1ʱ��ת��ͨ��A1��B1��

#define BUSY		PCin(5)    //
#define DATA_A	PCin(7)    //
#define DATA_B	PCin(6)    //

void ADS8361_IO_Init(void);	   //ADS8361 IO�ڳ�ʼ��
void ADS8361_Init(uint8_t mode);	   //ADS8361��ʼ��
void ADS8361_Mode_selection(uint8_t mode);//ADS8361ģʽѡ��

void ADS8361_ReadData(uint8_t A0_Select,uint16_t *data);//��ADS8361����
void ADS8361_Read_ALLData(uint16_t *data);//��ȡADS8361����ͨ������



#endif















