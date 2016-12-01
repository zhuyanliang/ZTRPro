/* 
 * File:   led.h
 * Author: Administrator
 *
 * Created on 2016年6月20日, 上午9:12
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LedRed        	LATCbits.LATC7   
#define LedGre         	LATCbits.LATC6  
#define LedCharge    	LATDbits.LATD0   
#define LedRun      	LATCbits.LATC1 
#define LedFault        LATDbits.LATD1 

#define LedRedOff()			LedRed = 1
#define LedGreOff()			LedGre = 1
#define LedChargeOff()		LedCharge = 1
#define LedRunOff()			LedRun = 1
#define LedFaultOff()       LedFault = 1

#define LedRedOn()			LedRed = 0
#define LedGreOn()			LedGre = 0
#define LedChargeOn()		LedCharge = 0
#define LedRunOn()			LedRun = 0
#define LedFaultOn()       LedFault = 0

void Led_Init(void);
void TaskLedMgt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

