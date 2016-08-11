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
#if 0
	/* 配置INT0中断 */
	INTCON2bits.INTEDG0 = 1;  //interrupt on rising edge
	INTCONbits.INT0IF = 0;
	INTCONbits.INT0IE = 1;  //enable INT0 interrupt

	/* 开启全局中断 */
	RCON = 0x9F;
	INTCON2 = 0x80;
	INTCON3 = 0x00;
	INTCON = 0xC0;  //Enable once setup done
#endif	
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
    
    if(PIR5bits.RXB0IF)
    {
#if 0
        static uint8_t i;
        PIR5bits.RXB0IF = 0b0;
        if(ECAN_ReceiveMsg(&msgRx))
        {
            msgTx.IDE = msgRx.IDE;
            msgTx.RTR = msgRx.RTR;
            msgTx.DLC = msgRx.DLC;
            msgTx.COB_ID = msgRx.COB_ID;
            for(i=0;i<8;i++)
                msgTx.Data[i] = msgRx.Data[i];
            ECAN_TransmitMsg(&msgTx);
        }
#endif            
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
