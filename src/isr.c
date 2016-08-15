#include "led.h"
#include "common.h"
#include "ecan.h"
#include <pic18.h>

volatile uint32_t g_SysTickMs = 0;

void ISR_Init(void)
{
	INTCONbits.GIE_GIEH 	= 1;	//允许所有未屏蔽中断
	INTCONbits.PEIE_GIEL 	= 1;	//允许所有未屏蔽外设中断
	RCONbits.IPEN			= 1;	//允许中断优先级
}

//interrupt
void interrupt ISR_High_Handler(void) 
{
    CanMsgTypedef msgRx={0},msgTx = {0};
    INTCONbits.GIE   = 0;
	
	//------------------------------ 1ms timer --------------------------------
	if (PIR1bits.TMR1IF)
	{      
        TMR1H = TMR1H_INIT;
        TMR1L = TMR1L_INIT;
        PIR1bits.TMR1IF = 0;

        g_SysTickMs++;

	}
	
	INTCONbits.GIE   = 1;
}

//----------------------------------------------------------------------------
// Function    : getSysTickCounter
// Description : Returns tick count from ms timer
// Parameters  : none
// Returns     : none
//----------------------------------------------------------------------------
uint32_t GetSysTickCounter(void)
{
   return (g_SysTickMs);
}

void DelayMs(uint16_t nTime)
{ 
   uint16_t CurTime = GetSysTickCounter();

   while(!(GetSysTickCounter() - CurTime >= nTime));
}
