#ifndef __systick_h__
#define __systick_h__

//--------------------------- Define ---------------------------//

//----------------------- Include files ------------------------//

//-------------------------- Typedef ---------------------------//
typedef struct {
	int (* initialize)(void);
	int second_flag;
}SYSTICK_T;

//--------------------------- Extern ---------------------------//
extern SYSTICK_T systick;

#endif //__systick_h__
