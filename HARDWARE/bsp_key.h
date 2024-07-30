# ifndef _BSP_KEY_H
# define _BSP_KEY_H

# include "sys.h"

# define Key1    key_Read(0, 0)
# define Key2    key_Read(0, 1)
# define Key3    key_Read(0, 2)
# define Key4    key_Read(0, 3)
# define Key5    key_Read(1, 0)
# define Key6    key_Read(1, 1)
# define Key7    key_Read(1, 2)
# define Key8    key_Read(1, 3)
# define Key9    key_Read(2, 0)
# define Key10   key_Read(2, 1)
# define Key11   key_Read(2, 2)
# define Key12   key_Read(2, 3)
# define Key13   key_Read(3, 0)
# define Key14   key_Read(3, 1)
# define Key15   key_Read(3, 2)
# define Key16   key_Read(3, 3)

// 按键结构体
typedef struct{
    uint8_t     judge;
    uint8_t     signal_flag;
}_KEYS_;

void key_Init(void);
void key_Scan(void);
void key_Test(void);
uint8_t key_Read(uint8_t row, uint8_t column);

# endif
