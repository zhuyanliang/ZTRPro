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

// 错误信息代码
#define FALT_CUV 	0xE4
#define FALT_COV 	0xE3
#define FALT_CIB	0xE5
#define FALT_CUT	0xE7
#define FALT_COT	0xE8
#define FALT_COC	0xE1
#define FALT_LTC	0xE0


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
	uint16_t pov;
	uint16_t puv;
	uint16_t vdif; // 一致性差值过界次数
	uint16_t tdif; // 温差过界次数
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

