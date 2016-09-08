
#include "include.h"

void Led_Init(void)
{
	TRISCbits.TRISC6 = 0;   // PCB���̵ƿ��ƶ˿�
	TRISCbits.TRISC7 = 0;   // PCB���ƿ��ƶ˿�
	
	TRISAbits.TRISA6 = 0;   // ���LED��˸���ƶ˿�
	TRISCbits.TRISC1 = 0;	// �ŵ�LED���ƶ˿�

    LedRedOff();
    LedGreOff();
    LedChargeOff();
    LedRunOff();  
}

//============================================================================
// Function    : SysStateLedMgt
// Description : �ú���Ϊϵͳ״̬LED��ָʾ������ϵͳ�������С����������ϵȡ�
// Parameters  : none
// Returns     : none
//============================================================================
void TaskLedMgt(void)
{
	static uint8_t timeStamp = 0;

	if (g_BatteryMode == PROTECTION) // ����ģʽ�� LedNormal Ϩ��
	{
		LedRunOff();
		LedChargeOff();
	}
	else if(g_BatteryMode == DISCHARGE)
	{
		LedRunOn();
		LedChargeOff();
	}
	else if(g_BatteryMode == CHARGE)
	{
		LedRunOff();
		if(g_BatteryParameter.SOC > 99) // ���������
		{
			LedChargeOn();
			return;
		}
		if (++timeStamp < 50)
		{
			LedChargeOn();           
		}
		else if (timeStamp < 100)
		{
			LedChargeOff();
		}
		else
		{
			timeStamp = 0;
		}
	}
}




