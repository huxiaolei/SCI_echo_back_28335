/*
 * main.c
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "DSP2833x_Examples.h"
#include "EvMgr.h"
#include "StateMgr.h"
#include "Timer.h"
#include "RS232.h"
#include "GPIO_Driver.h"
#include "SysCtrl.h"


//Declare EvMgr
EvMgr EvMgrObj ;
EvMgr *EvMgrPrt ;
StateMgr StateMgrObj ;
StateMgr *StateMgrPrt ;
State StReady,StSoftStart,StRun,StSoftOff,StFault,StDebug,StDischargeDebug,StChargeDebug;
//		1		2			3		4		5		6


void rs_app(Event *Ev) ;


//=============Define Parameters for SCI test=============


void main(void) {
	//0. init Sys
	//0. init Sys
	InitSysCtrl();
	InitPieCtrl();

	//1. =========================interrupt=============================================
	// Disable CPU interrupts and clear all CPU interrupt flags:
	DINT ;
	IER = 0x0000;
	IFR = 0x0000;

	//ram Funcs

	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart) ;

	InitFlash() ;
	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
	// This function is found in DSP2833x_PieVect.c.c0
	InitPieVectTable();

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &TmrTick ;
	EDIS;

	// Enable CPU INT1 which is connected to SEQ INT:
	IER |= M_INT1;
	// Enable SEQ1 INT in the PIE: Group 1 interrupt 1-1
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1 ;

	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM

	//=========================Core Module=============
	//===initialize Event Manager Create a EvMgr Obj.===
	initEvMgr(&EvMgrObj) ;
	EvMgrPrt = &EvMgrObj ;
	//====================Tmr============================
	init_timeBase(&BaseTimer) ;

	//creat State. Debug, Ready,
	//===============user module=======================
	//initialize GPIO
	InitGpio();

	//================init RS232======================================
	init_SCI(EvMgrPrt) ;
	EvMgrPrt->addEvent(EvMgrPrt,&EvRSRx) ;
	EvRSRx.addEvCallback(&EvRSRx,rs_app) ;

	//Start Tmr
	BaseTimer.start();


	for(;;){

		RS232.tx(&RS232) ;
		RS232.rx(&RS232) ;
//		StateMgr.run(&StateMgr) ;
		EvMgrPrt->runEvMgr(EvMgrPrt) ;

	}
}


void rs_app(Event *Ev){
	Uint16 i;
	if(RS232.RxStatus == RX_CRC_ERROR){
		//RS232.tx_frame_word(&RS232,0x0004) ;	//Error request repeat
		//RS232.tx_a_Frame(&RS232) ;	//transmit the frame ;
		//reset RX
		RS232.rest_Rx(&RS232) ;
	}
	else if(RS232.RxStatus == RX_TIMEOUT||RS232.RxStatus ==RX_TAIL_ERROR){
		//reset RX
		RS232.rest_Rx(&RS232) ;
	}
	else if(RS232.RxStatus == RX_READY){
		//echo back
		for(i=1;i<RS232.RxCnt-1;i++){
			RS232.tx_frame_word(&RS232,RS232.RxBuf[i]) ;
		}

		RS232.tx_a_Frame(&RS232) ;	//transmit the frame ;

		RS232.rest_Rx(&RS232) ;
	}
}
