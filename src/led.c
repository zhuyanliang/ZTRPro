
#include "include.h"

void Led_Init(void)
{
	TRISCbits.TRISC6 = 0;
	TRISCbits.TRISC7 = 0;

	/*添加SOC指示灯，板上指示灯*/
	TRISAbits.TRISA6 = 0;
	TRISCbits.TRISC1 = 0;

	//...
    LedRed = 1;
    LedGre = 1;

    LedChgRed = 1;
    LedNormal = 1;  
}

//============================================================================
// Function    : SysStateLedMgt
// Description : 该函数为系统状态LED灯指示，包括系统正常运行、报警、故障等。
// Parameters  : none
// Returns     : none
//============================================================================
void SysStateLedMgt(void)
{
	static uint8_t TimeStamp = 0;

	if (g_BatteryMode == PROTECTION)
	{
		LedNormal = 1; 
	}
	else
	{
		LedNormal = 0;  

		if( g_BatteryMode == CHARGE )
		{
			if (++TimeStamp < 50)
			{
				LedChgRed = 0;             
			}
			else if (TimeStamp < 100)
			{
				LedChgRed = 1;
			}
			else
			{
				TimeStamp = 0;
			}
		}
		else
		{
			LedChgRed = 1;   
		}
	}
}



//============================================================================
// Function    : Soc_LedIndicate
// Description : 
// Parameters  : none
// Return      : none
//============================================================================
void SysSocLedMgt(void)
{
	static uint8_t timer = 0;

	if (g_BatteryParameter.SOC> 75)
	{
		LATAbits.LATA6 = 0;
		LATAbits.LATA7 = 0;

		if (g_BatteryMode == CHARGE)
		{
			if (++timer > 25)
			{
				LATCbits.LATC1 ^= 1;
				timer = 0;
			}
		}
		else
		{
			LATCbits.LATC1 = 0;
		}
	}
	else if(g_BatteryParameter.SOC > 50)
	{
		LATAbits.LATA7 = 0;
		LATCbits.LATC1 = 1;

		if (g_BatteryMode == CHARGE)
		{
			if (++timer > 25)
			{
				LATAbits.LATA6 ^= 1;
				timer = 0;
			}
		}
		else
		{
			LATAbits.LATA6 = 0;
		}
	}
	else if (g_BatteryParameter.SOC > 25)
	{
		LATAbits.LATA6 = 1;
		LATCbits.LATC1 = 1;

		if (g_BatteryMode == CHARGE)
		{
			if (++timer > 25)
			{
				LATAbits.LATA7 ^= 1;
				timer = 0;
			}
		}
		else
		{
			LATAbits.LATA7 = 0;
		}
	}
	else if (g_BatteryParameter.SOC > 10)
	{
		LATAbits.LATA6 = 1;
		LATAbits.LATA7 = 1;
		LATCbits.LATC1 = 1;

		if (g_BatteryMode == CHARGE)
		{
			if (++timer > 25)
			{
				timer = 0;
			}
		}
	}
	else
	{
		LATAbits.LATA6 = 1;
		LATAbits.LATA7 = 1;
		LATCbits.LATC1 = 1;

		if (g_BatteryMode == CHARGE)
		{
			if (++timer > 25)
			{
				timer = 0;
			}
		}
	} 
}


void TaskLedMgt(void)
{
   SysStateLedMgt();
   SysSocLedMgt();
}



