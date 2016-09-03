/* 
 * File:   isr.h
 * Author: Administrator
 *
 * Created on 2016年6月20日, 上午9:17
 */

#ifndef ISR_H
#define	ISR_H

#ifdef	__cplusplus
extern "C" {
#endif

extern volatile uint32_t g_SysTickMs;


void ISR_Init(void);
void DelayMs(uint16_t nTime);


#ifdef	__cplusplus
}
#endif

#endif	/* ISR_H */

