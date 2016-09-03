/* 
 * File:   detectparameter.h
 * Author: ZhuYanliang
 *
 * Created on 2016年7月12日, 下午10:32
 */

#ifndef DETECTPARAMETER_H
#define	DETECTPARAMETER_H

#ifdef	__cplusplus
extern "C" {
#endif

void 	DetectPackOverCurrent(void);
void 	DetectMaxMinCellTemp(void);
void 	DetectCellsOverTemp(void);
void 	DetectCellsUnderTemp(void);
void 	DetectPCBOverTemp(void);
void 	DetectCellTempDlt(void);
void 	DetectPackUv(void);
void 	DetectPackOv(void);
void 	DetectCellsOverVolt(void);
void 	DetectCellsUnderVolt(void);
void 	DetectCellsVoltImba(void);
uint8_t DetectPackDischargeFinish(void);
uint8_t DetectPackChargeFinish(void);
BOOL 	DetectIsChargedState(void);
void 	DetectMaxMinAvgCellVolt(void);
void 	DetectCharger(void);
void 	DetectRunkey(void);
uint8_t GetChargeState(void);
uint8_t GetKeyrunState(void);

void DetectCAN_NeighborPack(void);




#ifdef	__cplusplus
}
#endif

#endif	/* DETECTPARAMETER_H */

