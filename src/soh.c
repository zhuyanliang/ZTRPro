
//----------------------------------------------------------------------------
//  File name   ： Tasksoh.c
//  Description ： This module is used to estimate battery SOH value and accumulate
//  Versions    ： V1.0
//  Copyright   ： 
//  Author      ： 
//  Date        ： 
//  Modification：
//  Comment     ：
//----------------------------------------------------------------------------

#include "include.h"

#define CYCLE_AH      		((uint32_t)(1800000 * BATTERY_CAPACITY_TOTAL))

//============================================================================
// Function    : Soc_Init
// Description : 
// Parameters  : None
// Return      : None
//============================================================================
void Soh_Init(void)
{
	Soh_ReadChargedAh();
	Soh_ReadCycleTime();
}

//============================================================================
// Function    : Soh_StoreChargedAh
// Description : 存储系统ChargedAh到EEPROM中,
// Parameters  : none
// Return      : none
//============================================================================
void Soh_StoreChargedAh(void)
{
	EEPROM_WriteBlock(EEPROM_ADDR_CHARGEDAH, (uint8_t*)&g_BatteryParameter.ChargedAh, 4);
}

//============================================================================
// Function    : Soh_ReadChargedAh
// Description : 从EEPROM中读取ChargedAh值
// Parameters  : none
// Return      : soc
//============================================================================
void Soh_ReadChargedAh(void)
{
	uint32_t tmp;

	EEPROM_ReadBlock(EEPROM_ADDR_CHARGEDAH, (uint8_t*)&tmp, 4);

	if (tmp > CYCLE_AH)
	{
		tmp = 0;
	}

	g_BatteryParameter.ChargedAh = tmp;
}

//============================================================================
// Function    : Soh_StoreCycleTime
// Description : 存储系统充放电循环次数到EEPROM中,
// Parameters  : none
// Return      : none
//============================================================================
void Soh_StoreCycleTime(void)
{
	EEPROM_WriteBlock(EEPROM_ADDR_CYCLE_TIME, (uint8_t*)&g_BatteryParameter.sohCycleTimes, 2);
}

//============================================================================
// Function    : Soh_ReadCycleTime
// Description : 从EEPROM中读取ChargedAh值
// Parameters  : none
// Return      : soc
//============================================================================
void Soh_ReadCycleTime(void)
{
   uint16_t tmp;

   EEPROM_ReadBlock(EEPROM_ADDR_CYCLE_TIME, (uint8_t *)&tmp, 2);

   if (tmp == 0xFFFF)
   {
      tmp = 0;
   }

   g_BatteryParameter.sohCycleTimes = tmp;
}

//============================================================================
// Function    : Soh_ChargeAhAcc
// Description : 此函数要在充电时2ms调用一次，用于累计充电量
// Parameters  : none
// Return      : none
//============================================================================
void Soh_ChargeAhAcc(void)
{
	// 注意!!!
	if (g_BatteryMode == CHARGE)
	{
		g_BatteryParameter.ChargedAh -= g_BatteryParameter.current;
	}
}

//============================================================================
// Function    : Soh_UpdateCycleTime
// Description : 
// Parameters  : none
// Return      : none
//============================================================================
void Soh_UpdateCycleTime(void)
{
   if (g_BatteryParameter.ChargedAh > CYCLE_AH)
   {
      g_BatteryParameter.ChargedAh -= CYCLE_AH;
      g_BatteryParameter.sohCycleTimes++;
      Soh_StoreCycleTime();
   }
}


