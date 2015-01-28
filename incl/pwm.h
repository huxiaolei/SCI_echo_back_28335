/*
 * pwm.h
 *
 *  Created on: Feb 6, 2012
 *      Author: Mike
 */

#ifndef PWM_H_
#define PWM_H_

//DC PWM Control
#define DISABLE_DCPWM 1
#define SET_DISCHARGE 2
#define SET_CHARGE	  3

//CSI sector
#define SEC1	0.523599
#define SEC2	1.570796
#define SEC3	2.617994
#define SEC4	3.665191
#define SEC5	4.712389
#define SEC6	5.759587

//voltage Sector
#define V_SEC1	1.047198
#define V_SEC2	2.094395
#define V_SEC3	3.141593
#define V_SEC4	4.188790
#define V_SEC5	5.235988
#define V_SEC6	6.283185

   //ENABLE PWM
#define ENABLE_PWM  GpioDataRegs.GPCDAT.bit.GPIO84 = 0; //Enable Global PWM
#define DISABLE_PWM GpioDataRegs.GPCDAT.bit.GPIO84 = 1; //Disable Global PWM

#define MAX_BUCK_DUTY	0.9	//BUCK duty not larger than max
#define MIN_BUCK_DUTY	0	//BUCK duty not smaller than min

// Configure which ePWM timer interrupts are enabled at the PIE level:
// 1 = enabled,  0 = disabled
#define PWM1_INT_ENABLE  0
#define PWM2_INT_ENABLE  0
#define PWM3_INT_ENABLE  0
#define PWM4_INT_ENABLE  0
#define PWM5_INT_ENABLE  0
#define PWM6_INT_ENABLE  0

// Configure the period for each timer
#define PWM1_TIMER_TBPRD   0x1FFF
#define PWM2_TIMER_TBPRD   0x1FFF
#define PWM3_TIMER_TBPRD   0x1FFF
#define PWM4_TIMER_TBPRD   0x1FFF
#define PWM5_TIMER_TBPRD   0x1FFF
#define PWM6_TIMER_TBPRD   0x1FFF

// HI Speed Peripheral 75M
#define BUCK_TBPRD			3000
#define CSI_TBPRD		3750 // up and down mode, 40k
#define CSR_TBPRD		3750 //
#define VSI_TBPRD		3750

//discharge
#define VSI_DC_BALANCE 0//-100
#define CSI_DB	150 //1us
#define VSI_DB	1
#define AUX_DB_FE  40
#define AUX_DB_RE  200
#define AUX_ON_DELAY 40+120
#define AUX_OFF_TIME 100
#define VSI_WAIT_AUX_ON_DB 250

//charge
#define CSR_WAIT_LEAKAGE_DB 150
#define CSR_TURN_OFF_DB	100
#define AUX_ON_CHARGE_DELAY 50
#define AUX_OFF_CHARGE_DELAY 20
#define VSI_WAIT_LEAKAGE_DB 300
#define VSI_CHARGE_OFF_TIME	50
#define TRANS_SHIFT_LIMIT 300
#define VSI_DC_BALANCE_CHARGE 0


#define CSI_DEADBAND 75
#define CSR_DEADBAND 75
#define VSI_DEADBAND 75

#define BRIDGE_DEADBAND		100		// 500 ns 150 for 1us
#define BRIDGE_50DUTY		(BRIDGE_TBPRD>>1)

typedef struct CSISVM_Struct CSISVM ;

struct CSISVM_Struct{
	Uint16 PLLCnt ; //time delay for PLL
	Uint16 Sec;
	Uint16 PreSec ;
	float32 Iref ;
	float32 In;
	float32 In_1; //In+1
	float32 IH ;
	float32 Phase ;
	float32 PhaseV ;
	Uint16  SecV ;
	Uint16 SecChange ;
	Uint16 Charge_Discharge ; //1 for discharge, 2 for charge
	//method
	void (*calc) (CSISVM *CSIModPrt) ;
	void (*firePWM) (CSISVM *CSIModPrt) ;
	void (*clear) (CSISVM *CSIModPrt) ;

};

typedef struct DCMod_Struct DCMod ;

struct DCMod_Struct{
	Uint16 Sta ;	//0
	Uint16 Cmd ;	//1 off, 2 to DIScharge, 2 to charge.
	Uint16 Cnt ;
	float32 I0 ;
	float32 IH ;
	//method
	void (*firePWM) (DCMod *DCModPrt) ;
	void (*clear) (DCMod *DCModPrt) ;
};

extern void PWMInit(void);
extern void init_tripZone() ;
extern void clear_tripZone(void);
extern void initCSIMod(CSISVM *CSIModPrt) ;
void enableTripZone(void);
void disableTripZone(void);

extern CSISVM CSIMod ;
extern DCMod DCModReg ;

extern Event EvDSPTrip ;

#endif /* PWM_H_ */
