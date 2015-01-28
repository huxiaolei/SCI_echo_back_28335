/*
 * RS232.c
 *
 *  Created on: Feb 21, 2012
 *      Author: Mike
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "EvMgr.h" 				 // Event Manager Include File
#include "Timer.h"
#include "DA_Driver.h"
#include "usDelay.h"
#include "RS232.h"
#include "CRC16.h"

#define SCI_HEADER	"SS"	//this character will be used as the header of SCI communication, only one ASCII is alowed, if multiple characters are desired, pls change the src code
#define SCI_TAIL	"EE"	//this will be used for check the ending of SCI communication
#define SCI_LENGTH	4	//define in Uint16

#define ENABLE_CRC16
#undef ENABLE_CRC16

#ifdef ENABLE_CRC16
#define CRC_LENGTH 1
#else
#define CRC_LENGTH 0
#endif

#define SCI_FRAME_LENGTH (2+SCI_LENGTH+CRC_LENGTH)

void push_buf(RsBufObj *RsBuf, Uint16 InData) ;
Uint16 pop_buf(RsBufObj *RsBuf) ;
void tx_RS232(RS232Obj *Rs232) ;
void tx_word(RS232Obj *RS232,Uint16 word) ;
void tx_frame_word(RS232Obj *RS232, Uint16 word) ;
void tx_a_Frame(RS232Obj *RS232) ;
void rx_RS232(RS232Obj *Rs232) ;
void rest_Rx (RS232Obj *Rs232) ;
void save_bytes(RS232Obj *Rs232, Uint16 RxNum) ;
Bool delOverLap(RS232Obj *RS232) ;
Uint16 CRC16(Uint16 * message, Uint16 length, Uint16 crc) ;
void check_tail(RS232Obj *Rs232) ;
EvSta check_RS_event(Event *Ev) ;

RS232Obj RS232 ;
Event	EvRSRx ;

void init_SCI(EvMgr *EvMgrPrt){
	Uint16 i ;
	//Init GPIO
	EALLOW;
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
	// Set qualification for selected pins to asynch only
	// Inputs are synchronized to SYSCLKOUT by default.
	// This will select asynch (no qualification) for the selected pins.
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	//Configure SCI-A pins using GPIO regs
	// This specifies which of the possible GPIO pins will be SCI functional pins.
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
	//enable SCI module clk
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
	EDIS;
   //Init FIFO
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x204f;
    SciaRegs.SCIFFCT.all=0x0;
    //config SCI regs
    SciaRegs.SCIPRI.all = 0x08 ;
    SciaRegs.SCICCR.bit.SCICHAR = 0x7 ; //8 characters no parity bit
    //enable Tx and Rx
    SciaRegs.SCICTL1.bit.TXENA = 1 ;
    SciaRegs.SCICTL1.bit.RXENA = 1 ;
    //set Baud rate
    SciaRegs.SCIHBAUD = ((SCI_BAUDRATE_REG & 0xFF00))>>8 ;
    SciaRegs.SCILBAUD = (SCI_BAUDRATE_REG & 0x00FF) ;
    SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset

    //Init SCI obj
    init_RS_BUF(&RS232.TxBuf) ;
    RS232.TxFrameCnt = 0 ;
    RS232.RxCnt = 0 ;
    RS232.RxStatus = RX_IDEL ;
    RS232.TimeStamp = 0 ;
    RS232.rx = &rx_RS232 ;
    RS232.tx = &tx_RS232 ;
    RS232.tx_frame_word = &tx_frame_word ;
    RS232.tx_a_Frame = &tx_a_Frame ;
    RS232.rest_Rx = &rest_Rx ;
    for(i=0;i<RS232_BUF_DEPTH>>1;i++){
    	RS232.RxBuf[i] = 0 ;
    	RS232.TxFrameBuf[i] = 0 ;
    }



    //init Rx event ;
    EvMgrPrt->initEv(EvMgrPrt,&EvRSRx,check_RS_event) ;
}

//check event
EvSta check_RS_event(Event *Ev){
	if(RS232.RxStatus == RX_READY ||RS232.RxStatus ==RX_CRC_ERROR || RS232.RxStatus ==RX_TIMEOUT||RS232.RxStatus==RX_TAIL_ERROR ){
		return EvReady ;
	}
	else
		return EvNone ;
}
Bool delOverLap(RS232Obj *RS232){
	Uint16 Length,NewLength,i ;
	Bool RS = FALSE ;
	//if find any overlap frame then delete!
	Length = (RS232->RxBuf[0] & 0x00ff)>>1 ;
	if(RS232->RxCnt > Length){
		NewLength = (RS232->RxBuf[Length] &0x00ff)>>1 ;
		if(RS232->RxCnt>=(Length+NewLength)){
			//Now we know there is overlap.
			//try to left shift all rest data and overwrite the overlap one
			RS232->RxCnt -= NewLength ;
			for(i=0;i<RS232->RxCnt-Length;i++){
				RS232->RxBuf[Length+i] = RS232->RxBuf[Length+NewLength+i] ;
				RS = TRUE ;
			}
		}
	}
	return RS ;
}

void rx_RS232(RS232Obj *RS232){
	Uint16 RxNum;
	char *SCI_Header=SCI_HEADER;
	RxNum = SciaRegs.SCIFFRX.bit.RXFFST ;
	//RxNum >>= 1 ;
	if(RxNum>0){
		if(RS232->RxStatus==RX_IDEL){
			if(RxNum>=2){
				while(RxNum){
					// search for the header
					if(((char) (SciaRegs.SCIRXBUF.bit.RXDT & 0x00ff))==*SCI_Header){
						RxNum--;
						if(((char) (SciaRegs.SCIRXBUF.bit.RXDT & 0x00ff))==*(SCI_Header+1)){
							RxNum--;
							// rx start
							//set start info
								RS232->RxStatus = RX_START ;
								RS232->TimeStamp = BaseTimer.TimerTicks ;
								RS232->RxCnt=1;
								//save HEADER to rx buf
								RS232->RxBuf[0] = (Uint16) (((*(SCI_Header+1))&0x00FF)<<8)|((*(SCI_Header)&0x00FF)) ;
								//save the rest
								save_bytes(RS232,RxNum) ;
								check_tail(RS232);
							break;
						}
					}
				}
			}
		}
		else if(RS232->RxStatus==RX_START){
			//After start, just read data and check the tail
			save_bytes(RS232,RxNum) ;
			check_tail(RS232);
		}
		else
		{
			while(RxNum){
				SciaRegs.SCIRXBUF.bit.RXDT ;
				RxNum--;
			}
		}
	}
	//check time out
	if(RS232->RxStatus==RX_START){
		if(get_Time_elapsed_100US(RS232->TimeStamp)>=RX_FRAME_TIMEOUT){
			RS232->RxStatus = RX_TIMEOUT ;
		}
	}
}

void save_bytes(RS232Obj *Rs232, Uint16 RxNum){
	while(RxNum>1 && Rs232->RxCnt<SCI_FRAME_LENGTH) {
		//first lower byte then Hi byte
		Rs232->RxBuf[Rs232->RxCnt] = SciaRegs.SCIRXBUF.bit.RXDT & 0x00ff ;
		Rs232->RxBuf[Rs232->RxCnt] = Rs232->RxBuf[Rs232->RxCnt] | ((SciaRegs.SCIRXBUF.bit.RXDT & 0x00ff)<<8) ;
		Rs232->RxCnt ++ ;
		RxNum -= 2 ;
	}
}

void check_tail(RS232Obj *Rs232){
#ifdef ENABLE_CRC16
	Uint16 CRC ;
#endif
	char *tail=SCI_TAIL;
	Uint16 UintTail = (Uint16) (((*(tail+1))&0x00ff)<<8)|(*(tail)&0x00ff);
	if(Rs232->RxCnt>=SCI_FRAME_LENGTH){
		//check the tail character
		if((Rs232->RxBuf[SCI_FRAME_LENGTH-1])==UintTail){
#ifdef ENABLE_CRC16
			//check CRC16;
			CRC = Rs232->RxBuf[SCI_FRAME_LENGTH-1]  ;
			if(CRC == CRC16(RS232->RxBuf,(SCI_FRAME_LENGTH-1)*2,0xffff)){
				RS232->RxStatus = RX_READY ;
			}
			else{
				RS232->RxStatus = RX_CRC_ERROR ;
			}
#else
			Rs232->RxStatus = RX_READY ;
#endif
		}
		else
			Rs232->RxStatus = RX_TAIL_ERROR;
	}
}

void rest_Rx (RS232Obj *Rs232){
	Uint16 i ;
    for(i=0;i<RS232.RxCnt;i++){
    	RS232.RxBuf[i] = 0 ;
    }
	RS232.RxCnt = 0 ;
	RS232.RxStatus = RX_IDEL ;
	RS232.TimeStamp = 0 ;
}


//RS232 Obj
void tx_RS232(RS232Obj *RS232){
	Uint16 temp,i ;
	//first get available TX FIFO
	temp = 16 - SciaRegs.SCIFFTX.bit.TXFFST ;
	//get what's left in the Tx Buf
	if(temp>RS232->TxBuf.Cnt){
		temp = RS232->TxBuf.Cnt ;
	}
	//then write into FIFO
	for(i=0;i<temp;i++){
		SciaRegs.SCITXBUF = RS232->TxBuf.pop(&RS232->TxBuf)&0x00ff ;
	}
}

void tx_word(RS232Obj *RS232,Uint16 word){
	RS232->TxBuf.push(&RS232->TxBuf,(word&0x00ff)) ;
	RS232->TxBuf.push(&RS232->TxBuf,(word&0xff00)>>8) ;
}

void tx_a_Frame(RS232Obj *RS232){
	Uint16 i ;
	//append tail
	char *tail=SCI_TAIL;
	Uint16 UintTail =(Uint16) (((*(tail+1))&0x00ff)<<8)|(*(tail)&0x00ff);
	tx_frame_word(RS232, UintTail);
	//calculate CRC
#ifdef ENABLE_CRC16
	if(RS232->TxFrameCnt>0){
		RS232->TxFrameBuf[RS232->TxFrameCnt] = CRC16(RS232->TxFrameBuf,RS232->TxFrameCnt<<1,0xffff) ;
		RS232->TxFrameCnt++ ;
	}

#endif
	//transmit
	for(i=0;i<RS232->TxFrameCnt;i++){
		tx_word(RS232,RS232->TxFrameBuf[i]) ;
		RS232->TxFrameBuf[i] = 0 ;
	}
	RS232->TxFrameCnt = 0 ;
}

void tx_frame_word(RS232Obj *RS232, Uint16 word){
	char *header=SCI_HEADER;
	Uint16 UintHeader = (Uint16) (((*(header+1))&0x00ff)<<8)|((*header)&0x00ff) ;
	//Uint16 UintHeader = (Uint16) (((*(header+1))&0x00ff)<<8) ;
	//first insert header
	if(RS232->TxFrameCnt==0){
		RS232->TxFrameBuf[0] = UintHeader ;
		RS232->TxFrameCnt++ ;
	}
	if(RS232->TxFrameCnt<(RS232_BUF_DEPTH>>1)){
		RS232->TxFrameBuf[RS232->TxFrameCnt] = word ;
		RS232->TxFrameCnt++ ;
	}
}

//========================Buffer Function==============================================
void init_RS_BUF(RsBufObj *RsBuf){
	int i ;
	RsBuf->Cnt = 0 ;
	for(i=0;i<RS232_BUF_DEPTH;i++){
		RsBuf->Buf[i] = 0 ;
	}
	RsBuf->pop = &pop_buf ;
	RsBuf->push = &push_buf ;
}

void push_buf (RsBufObj *RsBuf, Uint16 InData){
	if(RsBuf->Cnt<RS232_BUF_DEPTH){
		RsBuf->Buf[RsBuf->Cnt] = InData ;
		RsBuf->Cnt++ ;
	}
}

Uint16 pop_buf (RsBufObj *RsBuf){
	Uint16 Temp,i ;
	Temp = 0 ;
	if(RsBuf->Cnt>0&&RsBuf->Cnt<=RS232_BUF_DEPTH){
		Temp = RsBuf->Buf[0] ;
		for(i=0;i<RsBuf->Cnt-1;i++){
			RsBuf->Buf[i] = RsBuf->Buf[i+1] ;
		}
		RsBuf->Buf[RsBuf->Cnt-1] = 0 ;
		RsBuf->Cnt-- ;
	}
	return Temp ;
}

//=========================CRC16===========================================================
//ROUTINE:
//DESCRIPTION:
//message: the address of message string
//length: length of message string in bytes to calculate crc16
//crcal: to input the initial value of crc16 calculation buffer. If it is the first time to
//       calculate the string CRC value, the initial value shall be 0xffff.
Uint16 CRC16(Uint16 * message, Uint16 length, Uint16 crc){
	Uint16 	i,j,data,crcal ; 			/* loop variable */

	i = 0;
	crcal = crc;

	while(i < length)
	{
		data = *(message + (i >> 1));
		if(i & 0x0001)
			j = (data&0x00ff) ^ ((crcal&0xff00)>>8);		//low byte
		else
			j = ((data&0xff00)>>8) ^ ((crcal&0xff00)>>8);		//high byte
		crcal = ((tbcrch[j&0x00ff] ^ (crcal&0x00ff))<<8) & 0xff00;	//MSB
		crcal = (tbcrcl[j&0x00ff] & 0x00ff) | crcal;					//LSB
		i++;
	}
	return crcal;
}
