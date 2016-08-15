#include "include.h"

RelayFlagTepedef g_RelayActFlg;

//============================================================================
// Function    ：Relay_Init
// Description ：控制继电器引脚的初始化
// Parameters  ：none 
// Returns     ：none
//============================================================================
void Relay_Init(void)
{
	// 控制管脚设为输出
	TRISAbits.TRISA2 = 0;	// 负极继电器控制机端口
	TRISAbits.TRISA7 = 0;	// 加热继电器控制机端口
	TRISBbits.TRISB0 = 0;	// 预充继电器控制端口
	TRISBbits.TRISB5 = 0;	// 风扇继电器控制端口
	TRISCbits.TRISC0 = 0;	// 正极继电器控制端口
	

	RELAY_POSI_CTRL = 0;
	RELAY_NEGA_CTRL = 0;
	RELAY_PREC_CTRL = 0;
	RELAY_FAN_CTRL  = 0;
	RELAY_HEAT_CTRL = 0;

	g_RelayActFlg.precharge = FALSE;
	g_RelayActFlg.positive = TRUE;
	g_RelayActFlg.negative = TRUE;
}

//============================================================================
// Function    ：RelayAction
// Description ：该函数为继电器执行机构，执行条件为继电器动作标志位；该函数需要
//               周期性循环中被调用。
// Parameters  ：none 
// Returns     ：none
//============================================================================
void RelayAction(void)
{
	int8_t fan_on_temp, fan_off_temp;

	if(g_RelayActFlg.precharge == TRUE)
	{
		RELAY_PREC_CTRL = 1;
	}
	else
	{
		RELAY_PREC_CTRL = 0;
	}

	if(g_RelayActFlg.positive == TRUE)
	{
		RELAY_POSI_CTRL = 1;
	}
	else
	{
		RELAY_POSI_CTRL = 0;
	}

	if(g_RelayActFlg.negative == TRUE)
	{
		RELAY_NEGA_CTRL = 1;
	}
	else
	{
		RELAY_NEGA_CTRL = 0;
	}

	if (g_RelayActFlg.heating == TRUE)
	{
		RELAY_HEAT_CTRL = 1;
	}
	else
	{
		RELAY_HEAT_CTRL = 0;
	}   

	if (g_RelayActFlg.cooling == TRUE)
	{
		RELAY_FAN_CTRL = 1;
	}
	else
	{
		RELAY_FAN_CTRL = 0;
	}  

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
























