/*
 * EvMgr.c
 *
 *  Created on: Feb 3, 2012
 *      Author: Mike
 */

#include "DSP2833x_Device.h"     // Device Header Include File
#include "EvMgr.h" 				 // Event Manager Include File

//For EvMgr
void addEvent(EvMgr *EvMgrPrtIn, Event *Ev);
void runEvMgr(EvMgr *EvMgrPrtIn);

//For Event
void initEv(EvMgr *EvMgrPrt, Event *EvPrt, EvSta (*ev_check) (Event *EvPrt)) ;
void addEvCallback(Event *Ev, void (*Callback)(Event *Ev));

//initiate Event manager, before using EvMgr, this method must be called
void initEvMgr(EvMgr *EvMgrPrt){
	EvMgrPrt->EvIDCnt= 0 ;
	EvMgrPrt->EvCnt = 0 ;
	EvMgrPrt->EvCurrCnt = 0 ;
	EvMgrPrt->addEvent = &addEvent;
	EvMgrPrt->runEvMgr = &runEvMgr ;
	EvMgrPrt->initEv =&initEv ;
}

// Initiate an event
void initEv(EvMgr *EvMgrPrt, Event *EvPrt, EvSta (*checkEv) (Event *EvPrt)){

	EvMgrPrt->EvIDCnt ++ ;
	EvPrt->EvID = EvMgrPrt->EvIDCnt ;
	EvPrt->HookCnt = 0;
	EvPrt->HookCurCnt	= 0 ;
	EvPrt->addEvCallback = addEvCallback ;
	EvPrt->checkEv =checkEv ;
}

//add an event to EvMgr
void addEvent(EvMgr *EvMgrPrt, Event *EvPrt){
	if( EvMgrPrt->EvCnt < EV_MGR_DEPTH){
		EvMgrPrt->Ev_Stack[EvMgrPrt->EvCnt] = EvPrt;
		EvMgrPrt->EvCnt++;
	}
}

//run EvMgr, this method should be called in the background program
void runEvMgr(EvMgr *EvMgrPrt){
	Uint16 i,j ;
	for(i=0;i<EvMgrPrt->EvCnt;i++){
		EvMgrPrt->EvCurrCnt = i ;
		if(EvMgrPrt->Ev_Stack[i]->checkEv(EvMgrPrt->Ev_Stack[i]) == EvReady){
			//check if hook exists
			for(j=0;j<EvMgrPrt->Ev_Stack[i]->HookCnt;j++){
				EvMgrPrt->Ev_Stack[i]->HookCurCnt = j ;
				EvMgrPrt->Ev_Stack[i]->EvHook[j](EvMgrPrt->Ev_Stack[i]);
			}
		}
	}
}

void addEvCallback(Event *EvPrt, void (*Callback)(Event *Ev)){
	if(EvPrt->HookCnt<EV_HOOK_DEPTH){
		EvPrt->EvHook[EvPrt->HookCnt] = Callback ;
		EvPrt->HookCnt++;
	}
}
