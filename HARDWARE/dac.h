#ifndef __dac_h__
#define __dac_h__

//----------------------- Include files ------------------------//

//--------------------------- Define ---------------------------//				
						
//-------------------------- Typedef ---------------------------//
typedef struct{
	int (* initialize)(void);
	void(* set_voltage)(double);
}DAC_T;

//-------------------------- Extern ----------------------------//
extern DAC_T dac1;

#endif // __dac_h__