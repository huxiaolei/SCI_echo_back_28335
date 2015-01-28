/*
 * Timer.h
 *
 *  Created on: Feb 6, 2012
 *      Author: Mike
 */

#ifndef TIMER_H_
#define TIMER_H_

#define TIMER_100US 1
#define TIMER_1MS	10
#define TIMER_1S	10000

struct UTimer_Struct{
	Uint32	TimerTicks ;
	//function
	void (*start) (void);
};

typedef struct UTimer_Struct UTimer ;

void init_timeBase(UTimer *Tmr) ;
void initUserTmr1() ;
interrupt void TmrTick(void) ;

Uint32 get_Time_elapsed_100US(Uint32 PreTimerTicks);
Uint32 get_Time_elapsed_1MS(Uint32 PreTimerTicks) ;
Uint32 get_Time_elapsed_1S(Uint32 PreTimerTicks) ;

extern UTimer BaseTimer ;
extern Event UserTmr1 ;

#endif /* TIMER_H_ */
