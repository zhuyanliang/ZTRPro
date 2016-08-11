#include "include.h"

//============================================================================
// Function    ��Thermal_Init
// Description ��
// Parameters  ��none 
// Returns     ��none
//============================================================================
void Thermal_Init(void)
{
   TRISBbits.TRISB5 = 0;  // ���ȿ��ƹܽ�����Ϊ���
   FanOff();
}


//============================================================================
// Function    ��TskTherMgt
// Description ����ذ����ȹ�����ƣ�Ŀǰ��Ҫ�Ƿ��ȵ���ͣ���ƣ�
// Parameters  ��none 
// Returns     ��none
//============================================================================
void TskTherMgt(void)
{
	int8_t fan_on_temp, fan_off_temp;

	/* ����ϵͳ������״̬ȷ�����ȵĿ�ͣ�¶� */
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




