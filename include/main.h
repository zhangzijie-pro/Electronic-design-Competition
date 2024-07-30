#ifndef _MAIN_H
#define _MAIN_H

#define NCONFIG_ON GPIO_SetBits(GPIOG,GPIO_Pin_6)
#define NCONFIG_OFF GPIO_ResetBits(GPIOG,GPIO_Pin_6)

#define DCLK_ON GPIO_SetBits(GPIOG,GPIO_Pin_5)
#define DCLK_OFF GPIO_ResetBits(GPIOG,GPIO_Pin_5)

#define DATA0_ON GPIO_SetBits(GPIOG,GPIO_Pin_4)
#define DATA0_OFF GPIO_ResetBits(GPIOG,GPIO_Pin_4)

#define NSTATUS GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)

#define CONFIG_DONE GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15)

#endif
