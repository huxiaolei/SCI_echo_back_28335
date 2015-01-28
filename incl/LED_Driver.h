/*
 * LED.h
 *
 *  Created on: Feb 7, 2012
 *      Author: Mike
 *  Used for LED Driver
 *  LED 1 Green and is assigned to GPIO10
 *  LED 2 RED and is assigned to GPIO11
 */

#ifndef LED_H_
#define LED_H_

#define LED_GREEN0	1
#define LED_RED0	2

#define LED_ON	0
#define LED_OFF	1
#define LED_BLINK 2

#define ON_LED1 GpioDataRegs.GPADAT.bit.GPIO14 = 0	// GREEN
#define OFF_LED1 GpioDataRegs.GPADAT.bit.GPIO14 = 1 //GREEN
#define ON_LED2 GpioDataRegs.GPADAT.bit.GPIO15 = 0 //RED
#define OFF_LED2 GpioDataRegs.GPADAT.bit.GPIO15 = 1 //RED

#define TOGGLE_LED1 GpioDataRegs.GPATOGGLE.bit.GPIO14 = 1  //Toggle GPIO
#define TOGGLE_LED2 GpioDataRegs.GPATOGGLE.bit.GPIO15 = 1  //Toggle GPIO

#define BLINK_500MS	5000	//based on 100us
#define BLINK_1S	10000
#define BLINK_2S	20000
#define BLINK_3S	30000
#define BLINK_4S	40000
#define BLINK_5S	50000

typedef struct LED_struct LEDObj ;

struct LED_struct{
	Uint16	LEDID ;
	Uint16	LEDStatus ;
	Uint32	BlinkInterval ;
	Uint32	BlinkTmrStamp ;
	Bool	Toggle ;
	//==================function========================================
	void (*set_led) (LEDObj *LED,Uint16 LED_STATUS) ;
	void (*blink_led) (LEDObj *LED, Uint16 BlinkTime) ; //BlinkTime based on 100us
};

void init_LED(EvMgr *EvMgrPrt) ;

extern Event LEDTick ;
extern LEDObj LEDGreen ;
extern LEDObj LEDRed ;

#endif /* LED_H_ */
