
#include "include.h"

void Led_Init(void)
{
	TRISCbits.TRISC6 = 0;   // PCB���̵ƿ��ƶ˿�
	TRISCbits.TRISC7 = 0;   // PCB���ƿ��ƶ˿�
	
	TRISAbits.TRISA6 = 0;   // ���LED��˸���ƶ˿�
	TRISCbits.TRISC1 = 0;	// �ŵ�LED��˸���ƶ˿�

    LedRedOff();
    LedGreOff();
    LedChgRedOff();
    LedNormalOff();  
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
		LedNormalOff();
	}
	else
	{
		LedNormalOn();

		if( g_BatteryMode == CHARGE ) // ���ģʽ�У�LedChgRed��˸
		{
			if(g_BatteryParameter.SOC > 99) // ���������
			{
				LedNormalOn(); 
				return;
			}
			if (++timeStamp < 50)
			{
				LedNormalOn();            
			}
			else if (timeStamp < 100)
			{
				LedNormalOff();
			}
			else
			{
				timeStamp = 0;
			}
		}
		else
		{
			LedNormalOff(); 
		}
	}
}




