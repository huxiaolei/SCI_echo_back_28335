/*
 * CommApp.h
 *
 *  Created on: Feb 24, 2012
 *      Author: Mike
 */

#ifndef COMMAPP_H_
#define COMMAPP_H_

#define READ_START_ADD 0x0000
#define TOTAL_READ_WORDS 82

#define WRITE_START_ADD 0x0000
#define TOTAL_WRITE_WORDS 34

#define CALI_WORDS	32

#define READ_32BITS	1
#define READ_16BITS 0

#define BITS_16_MAX 2
#define In_32_MAX	2
#define OT_32_MAX	8

typedef union word_union Aword ;

struct Aword_Struct{
	Uint16 Lo:8 ;
	Uint16 Hi:8 ;
};

union word_union{
	Uint16 all ;
	struct Aword_Struct bits ;
};

struct OtRegs{
	Uint16 M1 ;
	Uint16 M2 ;
	Uint16 M3 ;
	Uint16 M4 ;
	Uint16 M5 ;
	Uint16 M6 ;
	Uint16 M7 ;
	Uint16 M8 ;
};

union OtVoltRegs{
	Uint16 all[8] ;
	struct OtRegs Moudule ;
};

union OtCurrRegs{
	Uint16 all[8] ;
	struct OtRegs Moudule ;
};

union SCellInfoSocRegs{
	Uint16 all[8] ;
	struct OtRegs Moudule ;
};

union SCellInfoTemRegs{
	Uint16 all[8] ;
	struct OtRegs Moudule ;
};

typedef struct CommRegs_struct CommRegObj ;

struct CommRegsReadstruct{
	ADCaliRegs ADCali ;
	ADInfoRegs ADInfo ;
	union OtVoltRegs OtVolt ;	//8 words
	union OtCurrRegs OtCurr ;	//8 words
	union SCellInfoSocRegs SOC ;	//8 words
	union SCellInfoTemRegs Tem ;	//8 words
	Uint16 Vfreq ;
	Uint16 CurrSetting ;
};

union CommRegsReadUnion{
	Uint16 all[TOTAL_READ_WORDS];
	struct CommRegsReadstruct words ;
};

struct CmdStatusRegs_struct{
	Uint16 Run:1 ;
	Uint16 Stop:1 ;
	Uint16 Debug:1;
	Uint16 RFault:1 ;
	Uint16 OtCellCtrl:1 ;
	Uint16 CurrSetting:1;
	Uint16 Cali:1 ;
	Uint16 DischargeDebug :1 ;
	Uint16 ChargeDebug :1 ;
	Uint16 Rsvd9 :1 ;
	Uint16 Rsvd10 :1 ;
	Uint16 Rsvd11 :1 ;
	Uint16 Rsvd12 :1 ;
	Uint16 Rsvd13 :1 ;
	Uint16 Rsvd14 :1 ;
	Uint16 Rsvd15 :1 ;
};

union CmdStatusRegs_union{
	Uint16 all ;
	struct CmdStatusRegs_struct bit ;
};

typedef union CmdStatusRegs_union CmdStatusRegs ;

struct Write_strct{
	ADCaliRegs ADCali ;
	Uint16 CurrSetting ;
	Uint16 VoltSetting ;
};

union Write_union{
	Uint16 all[TOTAL_WRITE_WORDS] ;
	struct Write_strct Module ;
};


struct CommRegs_struct
{
	union CommRegsReadUnion Read;
	union Write_union Write ;
	CmdStatusRegs CmdStatus ;
	//function
	void (*updateRead) (CommRegObj *CommRegIn) ;
	void (*update_Comm_ADInfo) (CommRegObj *CommRegIn, ADInfoRegs *ADInfoPrt) ;
	void (*upate_Comm_ADCali) (CommRegObj *CommRegIn,ADCaliRegs *ADCaliPrt) ;
	void (*parser_rx_data) (RS232Obj *RS232Prt,CommRegObj *CommRegIn,StateMgr *StateMgrPrt ,ADInfoRegs *ADInfoPrt, ADCaliRegs *ADCaliPrt) ;
};

extern CommRegObj CommReg ;
extern Event EvCommRun ;
extern Event EvCommStop ;
extern Event EvCommrecovery ;
extern Event EvCommDebug ;
extern Event EvCommCurrSetting ;
extern Event EvCommCali ;
extern Event EvCommChargeDebug ;
extern Event EvCommDischargeDebug ;

void initCommReg(EvMgr *EvMgrPrt,CommRegObj *CommRegIn);

#endif /* COMMAPP_H_ */
