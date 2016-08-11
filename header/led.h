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

#define LedRed                LATCbits.LATC7   
#define LedGre                LATCbits.LATC6  
#define LedChgRed             LATAbits.LATA6    
#define LedNormal             LATCbits.LATC1 


void Led_Init(void);
void SysStateLedMgt(void);
void SysSocLedMgt(void);
void TaskLedMgt(void);



#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

