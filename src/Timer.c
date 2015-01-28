/*
 * Timer.c
 *
 *  Created on: Feb 6, 2012
 *      Author: Mike
 */
#include "DSP2833x_Device.h"
#include "EvMgr.h"
#include "Timer.h"

void start_timeBase(void) ;

struct CPUTIMER_VARS CpuTimer0;

// When using DSP BIOS & other RTOS, comment out CPU Timer 2 code.
struct CPUTIMER_VARS CpuTimer1;
struct CPUTIMER_VARS CpuTimer2;

UTimer BaseTimer;
Event UserTmr1 ;
Uint32 UserTmr1Stamp ;
Uint16 UserTmr1Duration ;
EvSta check_UserTmr1(Event *Ev) ;
#pragma CODE_SECTION(TmrTick, "ramfuncs");
//timer for every user define xxx ms


EvSta check_UserTmr1(Event *Ev){
	if(get_Time_elapsed_1MS(UserTmr1Stamp)>=UserTmr1Duration){
		UserTmr1Stamp = BaseTimer.TimerTicks ;
		return EvReady ;
	}
	else
		return EvNone ;
}

interrupt void TmrTick(void)
{
	GpioDataRegs.GPADAT.bit.GPIO6 = 1 ;
	BaseTimer.TimerTicks++;
	//enable for next conversion
	CpuTimer0.RegsAddr->TCR.bit.TIF = 1 ;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1 ;
	GpioDataRegs.GPADAT.bit.GPIO6 = 0 ;
}

Uint32 get_Time_elapsed_100US(Uint32 PreTimerTicks){
	return (BaseTimer.TimerTicks - PreTimerTicks)/TIMER_100US ;
}

Uint32 get_Time_elapsed_1MS(Uint32 PreTimerTicks){
	return (BaseTimer.TimerTicks - PreTimerTicks)/TIMER_1MS ;
}

Uint32 get_Time_elapsed_1S(Uint32 PreTimerTicks){
	return (BaseTimer.TimerTicks - PreTimerTicks)/TIMER_1S ;
}

//in ms
void initUserTmr1(Uint16 Time, EvMgr *EvMgrPrt){
	UserTmr1Stamp = 0 ;
	UserTmr1Duration = Time ;
	EvMgrPrt->initEv(EvMgrPrt, &UserTmr1,&check_UserTmr1) ;
}

void init_timeBase(UTimer *Tmr){
	Tmr->TimerTicks = 0 ;
	Tmr->start = start_timeBase ;
	InitCpuTimers();	//Use Timer 0
	ConfigCpuTimer(&CpuTimer0,150,100) ;

}

void start_timeBase(void){
	CpuTimer0.RegsAddr->TCR.bit.TSS = 0 ;
}

//---------------------------------------------------------------------------
// InitCpuTimers:
//---------------------------------------------------------------------------
// This function initializes all three CPU timers to a known state.
//
void InitCpuTimers(void)
{
	EALLOW ;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2
	EDIS ;
	// CPU Timer 0
	// Initialize address pointers to respective timer registers:
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	// Initialize timer period to maximum:
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;


// CpuTimer2 is reserved for DSP BIOS & other RTOS
// Do not use this timer if you ever plan on integrating
// DSP-BIOS or another realtime OS.

	// Initialize address pointers to respective timer registers:
//	CpuTimer1.RegsAddr = &CpuTimer1Regs;
//	CpuTimer2.RegsAddr = &CpuTimer2Regs;
	// Initialize timer period to maximum:
//	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
//	CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
	// Make sure timers are stopped:
//	CpuTimer1Regs.TCR.bit.TSS = 1;
//	CpuTimer2Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
//	CpuTimer1Regs.TCR.bit.TRB = 1;
//	CpuTimer2Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
//	CpuTimer1.InterruptCount = 0;
//	CpuTimer2.InterruptCount = 0;
}
//---------------------------------------------------------------------------
// ConfigCpuTimer:
//---------------------------------------------------------------------------
// This function initializes the selected timer to the period specified
// by the "Freq" and "Period" parameters. The "Freq" is entered as "MHz"
// and the period in "uSeconds". The timer is held in the stopped state
// after configuration.
//
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32  temp;

	// Initialize timer period:
	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;
	// Set pre-scale counter to divide by 1 (SYSCLKOUT):
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;
	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 1;
	Timer->RegsAddr->TCR.bit.FREE = 1;     // Timer Free Run
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
	// Reset interrupt counter:
	Timer->InterruptCount = 0;
}



