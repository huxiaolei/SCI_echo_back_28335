/*
 * I2C.h
 *
 *  Created on: Sep 18, 2012
 *      Author: Mike
 */

#ifndef I2C_H_
#define I2C_H_

// set to Tx
#define I2CTX I2caRegs.I2CMDR.bit.TRX = 1
//set to Rx
#define I2CRX I2caRegs.I2CMDR.bit.TRX = 0
//start bi
#define I2CSTT I2caRegs.I2CMDR.bit.STT = 1
//stop condition
#define I2CSTP I2caRegs.I2CMDR.bit.STP = 1
#define DEFAULT_SALVE_ADD	0x0B
#define GET_SOC 0x0d
#define GET_TEP	0x08
//Time out
#define I2C_TIME_OUT_CNT 1000


enum CellInfoStat  {NONE_ACK = 0, OK = 1} ;

struct CellInfo_Reg{
	Uint16 SOC ;
	int16 Tem ;
	Uint16 cnt;
	Uint16 ErrCnt ;
	Uint16 SOC_S ;
	Uint16 SOC_T ;
	Uint16 Tem_S ;
	Uint16 Tem_T ;
	enum CellInfoStat Stat ;
};

struct CellInfo_All{
	struct	CellInfo_Reg M1 ;
	struct	CellInfo_Reg M2 ;
	struct	CellInfo_Reg M3 ;
	struct	CellInfo_Reg M4 ;
	struct	CellInfo_Reg M5 ;
	struct	CellInfo_Reg M6 ;
	struct	CellInfo_Reg M7 ;
	struct	CellInfo_Reg M8 ;
};

struct S_CellInfo_Reg{
	struct CellInfo_All CellInfo ;
	struct CellInfo_Reg *CellPrt ;
	Uint16 CurrentNum ;
	//function
	void (*exeI2C)(void) ;
	void (*tick)(void) ;
};


void initI2C() ;

extern struct S_CellInfo_Reg S_CellInfo ; //S for Smart

#endif /* I2C_H_ */
