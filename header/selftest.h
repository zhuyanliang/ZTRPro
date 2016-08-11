/* 
 * File:   selftest.h
 * Author: ZhuYanliang
 *
 * Created on 2016年7月13日, 下午10:35
 */

#ifndef SELFTEST_H
#define	SELFTEST_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"    
    
uint8_t SystemSelftest(void);
void Ltc6803_Selftest(void);
void Ltc6803_OpenWireTest(void);
void BusVoltSelftest(void);
void CellVoltSelftest(void);
void CellTempSelftest(void);
void AmbTempSelftest(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SELFTEST_H */

