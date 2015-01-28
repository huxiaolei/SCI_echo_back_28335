/*
 * SysCtrl.h
 *
 *  Created on: Oct 25, 2013
 *      Author: Mike
 */

#ifndef SYSCTRL_H_
#define SYSCTRL_H_

//for system control.

#define ENABLE_12V GpioDataRegs.GPBDAT.bit.GPIO57 = 1
#define DISABLE_12V GpioDataRegs.GPBDAT.bit.GPIO57 = 0

#endif /* SYSCTRL_H_ */
