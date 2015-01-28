/*
 * algorithm.h
 *
 *  Created on: Apr 22, 2013
 *      Author: Mike
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#define PI 3.1415926535
#define PHASE_S (PI * 2.0) / 3.0
#define DQCoeff 0.81649658
#define DQCoeff_o 0.57735

typedef struct abc_struct abcVar ;
struct abc_struct{
	float32 a ;
	float32 b ;
	float32 c ;
} ;

typedef struct dqo_struct dqoVar ;
struct dqo_struct{
	float32 d ;
	float32 q ;
	float32 o ;
};

typedef struct PICtrl_struct PICtrl ;

struct PICtrl_struct{
	Uint16 Activate ;
	float32 Ot ;
	float32 err_1 ;
	float32 Kp ;
	float32 Ki ;
	float32 UpLmt ;
	float32 LoLmt ;
	//function
	void (*clac) (PICtrl *PICtrlPrt,float32 err); //based on per unit
	void (*reset) (PICtrl *PICtrlPrt) ;
};

typedef struct LowPassFilter_struct LowPassFilter ;

struct LowPassFilter_struct{
	float32 Ot ;
	float32 wTs ; //wTs = w*Ts w is cut off frequency Ts is sampling time
	float32 wTs_1 ;
	//method
	void (*clac) (LowPassFilter *LPFPrt, float32 input) ;
};

typedef struct PLL_struct PLL ;

struct PLL_struct{
	Uint16 Status ; //1 means PLL OK
	float32 Phase ;
	float32 Freq ;
	float32 Ts ;
	abcVar abc;
	dqoVar dqo ;
	LowPassFilter dLFP ;
	LowPassFilter qLFP ;
	PICtrl Ctrl ;
	//method
	void (*clac) (PLL *PLLPrt) ;
	void (*reset) (PLL *PLLPrt) ;
};

//function prototype
extern void abc2Dq(abcVar *abcPrt, dqoVar *dqoPrt, float32 Phase) ;
extern void dq2Abc(dqoVar *dqoPrt, abcVar *abcPrt, float32 Phase) ;
extern void initPICtrl(PICtrl *PICtrlPrt) ;
extern void initLPF(LowPassFilter *LPFPrt) ;
extern void initPLL(PLL *PLLPrt) ;

#endif /* ALGORITHM_H_ */
