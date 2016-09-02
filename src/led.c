
#include "include.h"

void Led_Init(void)
{
	TRISCbits.TRISC6 = 0;   // PCB板绿灯控制端口
	TRISCbits.TRISC7 = 0;   // PCB板红灯控制端口
	
	TRISAbits.TRISA6 = 0;   // 充电LED闪烁控制端口
	TRISCbits.TRISC1 = 0;	// 放电LED闪烁控制端口

    LedRedOff();
    LedGreOff();
    LedChgRedOff();
    LedNormalOff();  
}

//============================================================================
// Function    : SysStateLedMgt
// Description : 该函数为系统状态LED灯指示，包括系统正常运行、报警、故障等。
// Parameters  : none
// Returns     : none
//============================================================================
void TaskLedMgt(void)
{
	static uint8_t timeStamp = 0;

	if (g_BatteryMode == PROTECTION) // 保护模式中 LedNormal 熄灭
	{
		LedNormalOff();
	}
	else
	{
		LedNormalOn();

		if( g_BatteryMode == CHARGE ) // 充电模式中，LedChgRed闪烁
		{
			if(g_BatteryParameter.SOC > 99) // 充满电后长亮
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




