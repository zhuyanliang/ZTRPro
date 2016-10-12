#include "include.h"

FaultRecordTpdef 		g_FaultRecord;
FaultRecentRecordTpdef 	g_FaultRecentRec;

//============================================================================
// Function    : FaultStoreInit
// Description : 
// Parameters  : none
// Returns     : none
//============================================================================
void FaultStoreInit(void)
{
	g_FaultRecord.cuv 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_CUV);
	g_FaultRecord.cov 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_COV);
	g_FaultRecord.cib 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_CIB);
	g_FaultRecord.cut 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_CUT);
	g_FaultRecord.dut 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_DUT);
	g_FaultRecord.cot 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_COT);
	g_FaultRecord.dot 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_DOT);
	g_FaultRecord.coc 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_COC);
	g_FaultRecord.doc 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_DOC);		
	g_FaultRecord.cer 		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_CER);
	g_FaultRecord.ltc_com 	= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_LTCCOM);
	g_FaultRecord.ltc_st 	= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_LTCST);
/*	g_FaultRecord.pov		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_POV);
	g_FaultRecord.puv		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_PUV);
	g_FaultRecord.vdif		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_VDIF);
	g_FaultRecord.tdif		= Nvm_ReadFaultRec(EEPROM_ADDR_FAULT_TDIF);
*/
	g_FaultRecord.pov		= 0;
	g_FaultRecord.puv		= 0;
	g_FaultRecord.vdif		= 0;
	g_FaultRecord.tdif		= 0;
	Nvm_ReadRecentFaultRec();	
}

	
//============================================================================
// Function    : Nvm_ReadFaultRec
// Description : 从地址 addr 读出2字节的错误数据 
// Parameters  : none
// Returns	: none
//============================================================================
uint16_t Nvm_ReadFaultRec(uint16_t addr)
{
	uint16_t rec;

	EEPROM_ReadBlock(addr, (uint8_t*)&rec, 2);

	if (rec > 50000)
	{
		rec = 0;
	}

	return (rec);
}


/*
 * 从地址EEPROM_ADDR_RECENT_FLT_PTR 
 * 读取最近的三次错出错记录
 *
 */
void Nvm_ReadRecentFaultRec(void)
{
	EEPROM_ReadBlock(EEPROM_ADDR_RECENT_FLT_PTR, (uint8_t *)(&g_FaultRecentRec.code_0), 3);
}

/*
 * 写入 EEPROM_ADDR_RECENT_FLT_PTR 地址3字节错误记录 
 *
 */
void Nvm_WriteRecentFaultRec(void)
{
	EEPROM_WriteBlock(EEPROM_ADDR_RECENT_FLT_PTR, (uint8_t *)(&g_FaultRecentRec.code_0), 3);
}


/*
 * 更新最近的三次记录
 *
 */
void Nvm_UpdateRecentFaultRec(uint8_t code)
{
	g_FaultRecentRec.code_2 = g_FaultRecentRec.code_1;
	g_FaultRecentRec.code_1 = g_FaultRecentRec.code_0;
	g_FaultRecentRec.code_0 = code;
}


