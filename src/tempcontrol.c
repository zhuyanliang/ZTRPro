#include "include.h"

//============================================================================
// Function    ：Thermal_Init
// Description ：
// Parameters  ：none 
// Returns     ：none
//============================================================================
void Thermal_Init(void)
{
   TRISBbits.TRISB5 = 0;  // 风扇控制管脚设置为输出
   FanOff();
}


//============================================================================
// Function    ：TskTherMgt
// Description ：电池包的热管理控制（目前主要是风扇的启停控制）
// Parameters  ：none 
// Returns     ：none
//============================================================================
void TskTherMgt(void)
{
	int8_t fan_on_temp, fan_off_temp;

	/* 根据系统所处的状态确定风扇的开停温度 */
	//   if ((g_BatteryMode == CHARGE)
	//    && (BatterySubMode == CHARGING))
	if(g_BatteryMode == CHARGE)
	{
		fan_on_temp = FAN_ON_TEMPERATURE_CHARGE;
		fan_off_temp = FAN_OFF_TEMPERATURE_CHARGE;
	}
	else
	{
		fan_on_temp = FAN_ON_TEMPERATURE_DISCHARGE;
		fan_off_temp = FAN_OFF_TEMPERATURE_DISCHARGE;
	}

	/* 风扇启停管理 */
	if (g_BatteryParameter.CellTempMax >= fan_on_temp)
	{
		FanOn();
	}
	else if (g_BatteryParameter.CellTempMax <= fan_off_temp)
	{
		FanOff();
	}
}




