/*
 * PushButtonDriver.h
 *
 *  Created on: Feb 3, 2012
 *      Author: Mike
 */

#ifndef PUSHBUTTONDRIVER_H_
#define PUSHBUTTONDRIVER_H_

#define PB_Enter 	1
#define PB_ESC		2
#define PB_Enter5S 	3
#define PB_ESC5S	4

#define PB_RELEASE	0
#define PB_PRESS	1

#define PB_ACT_TIME 1000  //that is 100ms
#define PB_ACT_TIME_MAX 10000  //that is 1s
#define PB_ACT5S_TIME 50000  //that is 0.5s

#define PB_CALLBACK_DEPTH 5

struct ButtonObj_struct{
	Uint16 Status ;
	Uint32 TimeStamp ;
	Uint16 Tpye ;
	Uint16 Action ;
};

typedef struct ButtonObj_struct ButtonObj ;

void int_PushButton(EvMgr *EvMgrPrt) ;

extern Event EvPBEnter ;
extern Event EvPBEnter5S ;
extern Event EvPBESC ;
extern Event EvPBESC5S ;

#endif /* PUSHBUTTONDRIVER_H_ */
