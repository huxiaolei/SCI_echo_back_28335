/*
 * AD.h
 *
 *  Created on: Feb 7, 2012
 *      Author: Mike
 */

#ifndef AD_H_
#define AD_H_

#include "algorithm.h"

#define TS_SAMPLE	0.00005

#define IN_VOLT_AD_CAL_K	((119.9-59.2)/1554)
#define IN_VOLT_AD_CAL_B	(59.2-OT_VOLT_AD_CAL_K*1650)

#define IN_CURR__CAL_K	(3.0*3.0/4095.0)
#define IN_CURR_CAL_B	320

#define OT_VOLT_AD_CAL_K	((12.60-4.22)/1701)
#define OT_VOLT_AD_CAL_B	(4.22-OT_VOLT_AD_CAL_K*988)

#define OT_CURR__CAL_K	(18.0/4095.0)
#define OT_CURR_CAL_B	2160

#define IN_VOLT_K	(160.0/4095.0)
#define IN_CURR_K	(3.0*3.0/4095.0)
#define OT_VOLT_K	(20.0/4095.0)
#define OT_CURR_K	(18.0/4095.0)

#define AD_Group_Volt		1
#define AD_Group_Curr		2

#define ADC_Cal	(void (*) (void) )	0x380080 // define ADC Cal from OTP memory

#define START_SOC_SEQ1		AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1	// software start SEQ1
#define START_SOC_SEQ2		AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 1	// software start SEQ2

typedef struct SysSetting_struct SysSetting;

struct SysSetting_struct{
	float32 Idc ;
	float32 Vdc ;
};

typedef union SysCmdUnion SysCmdInt ;

struct SysCmdBits{
	Uint16 Debug:1 ;		//0
	Uint16 PLL:1 ;			//1
	Uint16 DCCharge:1 ;		//2
	Uint16 DCDisCharge:1 ;	//3
	Uint16 Run:1 ;			//4
	Uint16 SoftOn:1 ;		//5
	Uint16 SoftOff:1 ;		//6
	Uint16 Ch2Dis:1 ;		//7
	Uint16 Dis2Ch:1;		//8
	Uint16 resvd:7 ;
};

union SysCmdUnion{
	Uint16 all;
	struct SysCmdBits bits;
};


typedef union ADCaliRegsUnion ADCaliRegs ;
//should be Q12
struct ADCali_struct{
	int16 K;
	int16 B;
};

struct ADCaliRegs{
	struct ADCali_struct AD0 ;
	struct ADCali_struct AD1 ;
	struct ADCali_struct AD2 ;
	struct ADCali_struct AD3 ;
	struct ADCali_struct AD4 ;
	struct ADCali_struct AD5 ;
	struct ADCali_struct AD6 ;
	struct ADCali_struct AD7 ;
	struct ADCali_struct AD8 ;
	struct ADCali_struct AD9 ;
	struct ADCali_struct AD10 ;
	struct ADCali_struct AD11 ;
	struct ADCali_struct AD12;
	struct ADCali_struct AD13 ;
	struct ADCali_struct AD14 ;
	struct ADCali_struct AD15 ;
};

union ADCaliRegsUnion{
	int16 all[32] ;
	struct ADCaliRegs Moudule ;
};

typedef union ADRawInfoUnion ADRawRegs ;

struct ADRawInfo_struct{
	Uint16 AD0 ;
	Uint16 AD1 ;
	Uint16 AD2 ;
	Uint16 AD3 ;
	Uint16 AD4 ;
	Uint16 AD5 ;
	Uint16 AD6 ;
	Uint16 AD7 ;
	Uint16 AD8 ;
	Uint16 AD9 ;
	Uint16 AD10 ;
	Uint16 AD11 ;
	Uint16 AD12 ;
	Uint16 AD13 ;
	Uint16 AD14 ;
	Uint16 AD15 ;
};

union ADRawInfoUnion{
	Uint16 all[16] ;
	struct ADRawInfo_struct Moudule ;
};

typedef union ADInfoUnion ADInfoRegs ;

struct ADInfo_struct{
	int16 AD0 ;
	int16 AD1 ;
	int16 AD2 ;
	int16 AD3 ;
	int16 AD4 ;
	int16 AD5 ;
	int16 AD6 ;
	int16 AD7 ;
	int16 AD8 ;
	int16 AD9 ;
	int16 AD10 ;
	int16 AD11 ;
	int16 AD12 ;
	int16 AD13 ;
	int16 AD14 ;
	int16 AD15 ;
};

union ADInfoUnion{
	int16 all[16] ;
	struct ADInfo_struct Moudule ;
};

struct ADUpdateBGRead_Struct{
	//this struct will be updated in interrupt and read in background
	float32 Data[6] ;
	float32 shadowData[6] ;
	Uint16 index ;
	Uint16 CntNum ;
	Uint16 Status ;
	int16 Vfreq ;
};

typedef struct ADUpdateBGRead_Struct ADUpdateBGRead ;

typedef struct DCCtrl_Struct DCCtrl ;

struct DCCtrl_Struct{
	float32 Idc_AC ;
	float32 Idc_DC ;
	float32 Vdc ;
};

extern ADCaliRegs ADCali ;
extern ADRawRegs ADRaw ;
extern ADInfoRegs ADInfo ;
extern ADInfoRegs ADCommInfo ;
extern PLL PLLSGen ;
extern ADUpdateBGRead ADBGRead ;
extern SysCmdInt SysCmdReg ;
extern DCCtrl DCCtrlReg;
extern SysSetting SysSettingReg ;

void init_AD(void) ;
void ADC_SEQ1_EOC_isr(void);

#endif /* AD_H_ */
