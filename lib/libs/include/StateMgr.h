/*
 * StateMgr.h
 *
 *  Created on: Feb 17, 2012
 *      Author: Mike
*   @Copyright all reserved
 */

#ifndef STATEMGR_H_
#define STATEMGR_H_

// if State changed, State Number need to be changed accordingly
#define STATE_NUM				5
#define STATE_HOOK_DEPTH		5
#define STATE_MGR_LIST_DEPTH	5

#define NULL 0

typedef struct State_struct State ;

struct State_struct{
	char *Name ;	//Name is an option, just use for communicate and debug.
	Uint16 ID ;		//Only for Identify purpose
	Uint16	PreHookCnt ;
	Uint16	PostHookCnt ;
	Uint16	RunHookCnt ;
	//Define PreHook, which will run before enter this State
	void (*PreHook[STATE_HOOK_DEPTH]) (State *StatePrt) ;
	//Define PostHook, which will run after leave this State
	void (*PostHook[STATE_HOOK_DEPTH]) (State *StatePrt) ;
	//Define RunHook, which will run when in this State
	void (*RunHook[STATE_HOOK_DEPTH]) (State *StatePrt) ;
	//
	void (*addPreHook) (State *StatePrt, void (*PreHook) (State *StatePrt)) ;
	//
	void (*addPostHook) (State *StatePrt, void (*PostHook) (State *StatePrt)) ;
	//
	void (*addRunHook) (State *StatePrt, void (*RunHook) (State *StatePrt)) ;
};

typedef struct StateMgr_struct StateMgr ;

struct StateMgr_struct{
	State * StatePrtList[STATE_MGR_LIST_DEPTH] ; //indicates State transfer history, in this stack, prt will be stored
	State * PreStatePrt ;
	State * CurrStatePrt ;
	void (*run) (StateMgr *StateMgrPrt) ;
	void (*transferState) (StateMgr *StateMgrPrt, State * NextStatePrt) ;
	void (*initState) (State *StatePrt) ;
};

void initStateMgr(StateMgr *StateMgrPrt) ;

#endif /* STATEMGR_H_ */
