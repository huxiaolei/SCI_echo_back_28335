/*
 * DA_Driver.h
 *
 *  Created on: Feb 6, 2012
 *      Author: Mike
 */

#ifndef DA_DRIVER_H_
#define DA_DRIVER_H_

#define DA_BUSY		1
#define DA_IDEL		0
#define D2A_STACK_DEPTH	10

#define DA_REG_CHN_A	0x0000
#define DA_REG_CHN_B	0x1000
#define DA_REG_CHN_C	0x2000
#define DA_REG_CHN_D	0x3000
#define DA_REG_CHN_E	0x4000
#define DA_REG_CHN_F	0x5000
#define DA_REG_CHN_G	0x6000
#define DA_REG_CHN_H	0x7000

#define DA_CHN_A	0x0001
#define DA_CHN_B	0x0002
#define DA_CHN_C	0x0004
#define DA_CHN_D	0x0008
#define DA_CHN_E	0x0010
#define DA_CHN_F	0x0020
#define DA_CHN_G	0x0040
#define DA_CHN_H	0x0080

//DA command
#define DA_WRM_MODE	0X8000	//WRM Write Register Mode
#define DA_WTM_MODE	0x9000	//WTM Write Through Mode
#define DA_UPDATE 0xA000 //update Selected channels should be followed by channel Info
#define DA_CAW	0xB000 //Channel A write and update all channels
#define DA_BROADCAST 0xC000 //Broadcast all channels and update all

//special operation
#define DA_BROADCAST_ALL(VALUE) SpiaRegs.SPITXBUF = (DA_BROADCAST | (VALUE<<2))

#define DA_STACK_DEPTH	8
#define DA_TIME_DELAY	2 //based on 100us

struct DAObj_Struct{
	Uint16 Stack[DA_STACK_DEPTH] ;
	Uint16 Cnt ;
	Uint16 CurCnt ;
	Bool Ready ;
	Uint32 TmrStamp ;
	void (*set_DA) (Uint16 CHN, Uint16 Value) ;
	void (*set_DA_flush) (Uint16 CHN, Uint16 Value) ;
	void (*flush_DA) (Uint16 CHN);
	void (*broadcast_DA) (Uint16 Value);
	void (*transmit) (void) ;
};

typedef struct DAObj_Struct DAObj ;

//functions
void InitDA(void);
//var
extern DAObj DA ;
#endif /* DA_DRIVER_H_ */
