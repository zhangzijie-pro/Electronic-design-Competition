#include "stm32f4xx.h"
#include "LED.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"


int main()
{

	led.initialize();
	LED_RED_ON;
	/*float t=0;
	delay_init(168);
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);*/

	while(1)
	{
		/*
		LCD_ShowChinese(0,0,"电子",RED,WHITE,24,0);
		LCD_ShowString(24,30,"LCD_W:",RED,WHITE,16,0);
		LCD_ShowIntNum(72,30,LCD_W,3,RED,WHITE,16);
		LCD_ShowString(24,50,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowIntNum(72,50,LCD_H,3,RED,WHITE,16);
		LCD_ShowFloatNum1(20,80,t,4,RED,WHITE,16);
		t+=0.11;
		LCD_ShowPicture(65,80,40,40,gImage_1);*/
	}
}