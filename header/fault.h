/* 
 * File:   fault.h
 * Author: Administrator
 *
 * Created on 2016年7月14日, 上午10:30
 */

#ifndef FAULT_H
#define	FAULT_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct
{
	uint16_t cuv;
	uint16_t cov;
	uint16_t cib;
	uint16_t cer;

	uint16_t cut;	
	uint16_t cot;
	uint16_t dut;
	uint16_t dot;
	uint16_t coc;
	uint16_t doc;
	uint16_t ltc_st;

	uint16_t ltc_com;
	uint16_t mos_ot;
}FaultRecordTpdef;


typedef struct
{
	uint8_t code_0;
	uint8_t code_1;
	uint8_t code_2;
}FaultRecentRecordTpdef;

extern FaultRecordTpdef 		g_FaultRecord;
extern FaultRecentRecordTpdef 	g_FaultRecentRec;

/* Function Prototype */
void FaultStoreInit(void);
void Nvm_ReadRecentFaultRec(void);
void Nvm_WriteRecentFaultRec(void);
uint16_t Nvm_ReadFaultRec(uint16_t addr);
void Nvm_UpdateRecentFaultRec(uint8_t code);


#ifdef	__cplusplus
}
#endif

#endif	/* FAULT_H */

