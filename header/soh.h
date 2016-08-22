/* 
 * File:   soh.h
 * Author: Administrator
 *
 * Created on 2016年7月13日, 下午6:03
 */

#ifndef SOH_H
#define	SOH_H

#ifdef	__cplusplus
extern "C" {
#endif

void Soh_Init (void);
void Soh_ReadChargedAh(void);
void Soh_ReadCycleTime(void);
void Soh_UpdateCycleTime(void);
void Soh_ChargeAhAcc(void);



#ifdef	__cplusplus
}
#endif

#endif	/* SOH_H */

