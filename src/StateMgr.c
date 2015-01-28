/*
 * StateMgr.c
 *
 *  Created on: Feb 17, 2012
 *      Author: Mike
 */
#include "DSP2833x_Device.h"     // Device Header Include File
#include "EvMgr.h"
#include "StateMgr.h"

//function prototype
void initState(State *StatePrt) ;
void addPreHook(State *StatePrt, void (*PreHook) (State *StatePrt)) ;
void addPostHook(State *StatePrt, void (*PostHook) (State *StatePrt)) ;
void addRunHook(State *StatePrt, void (*RunHook) (State *StatePrt)) ;
void runStateMgr (StateMgr *StateMgr) ;
void transferState(StateMgr *StateMgr, State *NextStatePrt) ;

//--------------------------------For State class-----------------------------------------
//Init Function for State
void initState(State *StatePrt){
	StatePrt->PostHookCnt = 0 ;
	StatePrt->PreHookCnt = 0 ;
	StatePrt->RunHookCnt = 0 ;
	StatePrt->addPreHook = &addPreHook ;
	StatePrt->addPostHook = &addPostHook ;
	StatePrt->addRunHook = &addRunHook ;
}

void addPreHook(State *StatePrt, void (*PreHook) (State *StatePrt)){
	if(StatePrt->PreHookCnt<STATE_HOOK_DEPTH){
		StatePrt->PreHook[StatePrt->PreHookCnt] = PreHook ;
		StatePrt->PreHookCnt++ ;
	}
}

void addPostHook(State *StatePrt, void (*PostHook) (State *StatePrt)){
	if(StatePrt->PostHookCnt<STATE_HOOK_DEPTH){
		StatePrt->PostHook[StatePrt->PostHookCnt] = PostHook ;
		StatePrt->PostHookCnt++ ;
	}
}
void addRunHook(State *StatePrt, void (*RunHook) (State *StatePrt)){
	if(StatePrt->RunHookCnt<STATE_HOOK_DEPTH){
		StatePrt->RunHook[StatePrt->RunHookCnt] = RunHook ;
		StatePrt->RunHookCnt++ ;
	}
}


//-----------------------------------For StateMgr---------------------------------------------
void initStateMgr(StateMgr *StateMgrPrt){
	int i ;

	StateMgrPrt->CurrStatePrt = 0 ;
	StateMgrPrt->PreStatePrt = 0 ;
	//init History list
	for(i=0;i<STATE_MGR_LIST_DEPTH;i++){
		StateMgrPrt->StatePrtList[i] = 0 ;
	}

	StateMgrPrt->run = &runStateMgr ;
	StateMgrPrt->transferState = &transferState ;
	StateMgrPrt->initState = &initState ;
}

void runStateMgr (StateMgr *StateMgrPrt){
	int i;
	if(StateMgrPrt->CurrStatePrt!=0){
		if(StateMgrPrt->CurrStatePrt !=0){
			for(i=0;i<StateMgrPrt->CurrStatePrt->RunHookCnt;i++){
				StateMgrPrt->CurrStatePrt->RunHook[i](StateMgrPrt->CurrStatePrt);
			}
		}
	}
}

void transferState(StateMgr *StateMgrPrt, State *NextStatePrt){
	//first run post funcs of current state
	State *CurStatePrt ;
	int i ;

	//first copy the current state from StateMgr
	CurStatePrt = StateMgrPrt->CurrStatePrt ;

	//check current state
	if(CurStatePrt!=0){
		//exit current
		for(i=0;i<CurStatePrt->PostHookCnt;i++){
			CurStatePrt->PostHook[i](CurStatePrt) ;
		}
	}

	//set New PreState ;
	StateMgrPrt->PreStatePrt = CurStatePrt ;

	if(NextStatePrt!=0){
		for(i=0;i<NextStatePrt->PreHookCnt;i++){
			NextStatePrt->PreHook[i](NextStatePrt) ;
		}
	}

	StateMgrPrt->CurrStatePrt = NextStatePrt ;
	//set State List
	for(i=0;i<(STATE_MGR_LIST_DEPTH-1);i++){
		StateMgrPrt->StatePrtList[i] = StateMgrPrt->StatePrtList[i+1] ;
	}
	StateMgrPrt->StatePrtList[STATE_MGR_LIST_DEPTH-1] = StateMgrPrt->PreStatePrt ;
}


