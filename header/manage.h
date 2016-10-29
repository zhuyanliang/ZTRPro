/* 
 * File:   manage.h
 * Author: Administrator
 *
 * Created on 2016年7月12日, 下午4:28
 */

#ifndef MANAGE_H
#define	MANAGE_H

#ifdef	__cplusplus
extern "C" {
#endif


void TskBatteryPra_Init(void);
void TskCurrentMgt(void);
void TskCellTempMgt(void);
void TskSOCMgt(void);
void TskRelayMgt(void);
void TskFaultStoreMgt(void);
void TskCanMgt(void);
void TskBatteryModeMgt(void);
void TskBlncMgt(void);
void TskAfeMgt(void);
void TskAmbTempMgt(void);
void TskSohMgt(void);
void TskSetCCsPra(void);
void TskCPUMode(void);



#ifdef	__cplusplus
}
#endif

#endif	/* MANAGE_H */

