/*
 * EvMgr.h
 *
 *  Created on: Feb 3, 2012
 *      Author: Mike
 **************************************************
 * Event Manager header file.
 *
 */

#ifndef EVMGR_H_
#define EVMGR_H_

//use fixed event depth to avoid dynamic memory allocate problem.
#define EV_MGR_DEPTH	30

//define Event type
#define EV_STA_READY	1
#define EV_STA_NONE		0
#define EV_HOOK_DEPTH	5


//Here enum is equal to the direct Define. inorder to compatible with the old version.
//But enum is more encouraged to use.
enum EV_Status{
	EvNone=0, EvReady = 1
};

//END define
#define TRUE	1
#define FALSE	0

typedef int	Bool ;

typedef struct EvMgr_Struct EvMgr;
typedef struct Event_Struct Event;
typedef enum EV_Status EvSta ;


struct Event_Struct{
	//add event obj ? TBD
	Uint16 EvID ;	// in this version, this ID will be assigned automatically based on sequence
	Uint16 HookCnt ;
	Uint16 HookCurCnt ;
	void (*EvHook[EV_HOOK_DEPTH]) (Event *EvPrt) ;
	//==============Function==========================
	EvSta (*checkEv) (Event *EvPrt) ;
	void (*addEvCallback) (Event *Ev, void (*Callback)(Event *EvPrt)) ;
};

struct EvMgr_Struct{
	//================Data definition==================
	Uint16 EvIDCnt ;
	Uint16 EvCnt ;
	Uint16 EvCurrCnt ;
	struct Event_Struct (*Ev_Stack[EV_MGR_DEPTH]) ;
	void (*addEvent) (EvMgr *EvMgrPrtIn, Event *Ev) ;
	void (*runEvMgr) (EvMgr *EvMgrPrtIn) ;
	void (*initEv) (EvMgr *EvMgrPrt, Event *EvPrt, EvSta (*ev_check) (Event *EvPrt));
};

void initEvMgr(EvMgr *EvMgrPrtIn) ;

#endif /* EVMGR_H_ */
