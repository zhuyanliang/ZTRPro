#include "include.h"

RelayFlagTepedef g_RelayActFlg;

//============================================================================
// Function    ��Relay_Init
// Description �����Ƽ̵������ŵĳ�ʼ��
// Parameters  ��none 
// Returns     ��none
//============================================================================
void Relay_Init(void)
{
	// ���ƹܽ���Ϊ���
	TRISAbits.TRISA2 = 0;	// �����̵������ƻ��˿�
	TRISAbits.TRISA7 = 0;	// ���ȼ̵������ƻ��˿�
	TRISBbits.TRISB0 = 0;	// Ԥ��̵������ƶ˿�
	TRISBbits.TRISB5 = 0;	// ���ȼ̵������ƶ˿�
	TRISCbits.TRISC0 = 0;	// �����̵������ƶ˿�
	

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
// Function    ��RelayAction
// Description ���ú���Ϊ�̵���ִ�л�����ִ������Ϊ�̵���������־λ���ú�����Ҫ
//               ������ѭ���б����á�
// Parameters  ��none 
// Returns     ��none
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

	/* ������ͣ���� */
	if (g_BatteryParameter.CellTempMax >= fan_on_temp)
	{
		FanOn();
	}
	else if (g_BatteryParameter.CellTempMax <= fan_off_temp)
	{
		FanOff();
	}
}
























