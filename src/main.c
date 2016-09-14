/* 
 * File:   main.c
 * Author: Administrator
 *
 * Created on 2016��6��20��, ����8:14
 */

#include "include.h"

// PIC18F46K80 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = LOW // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = ON     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 0         
#pragma config BORPWR = LOW 

// CONFIG2H
#pragma config WDTEN = ON  // Watchdog Timer (WDT enabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 65536  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-03FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 04000-07FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 08000-0BFFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 0C000-0FFFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 0C000-0FFFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 0C000-0FFFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)


#define TEST_PIN

void System_Init(void);

//============================================================================
// Function    ��SysClk_Init
// Description ��Initialize the system clock Fosc.
// Parameters  ��none 
// Returns     ��none
//============================================================================
void SysClk_Init(void)
{   
    OSCCONbits.SCS 		= 0b00;     //Select INOSC as system clock source
    OSCCONbits.IRCF 	= 0b111;    //������ѡHF_INTOSC���Ƶ��(16MHz)
    OSCCONbits.IDLEN 	= 0b0;      //when implement sleep function,mcu goto sleep 
    OSCTUNEbits.TUN 	= 0b0;		//����RC����(INTOSC)Ƶ�ʵ��� ��ʱ�����ڣ�ʹ��Ĭ��У��õ�
    OSCTUNEbits.PLLEN 	= 0b1;      //enable PLL  ϵͳʱ��������16MHz*4 
}

void TRIG_TEST(void)
{
	LATDbits.LATD0 ^= 0x1;   
}

//============================================================================
// Function    ��GPIO_Init
// Description ����Ƭ������IO�ܽų�ʼ��������IO�ĳ�ʼ����������Ӧģ���ʼ����
//               ����ִ��
// Parameters  ��none 
// Returns     ��none
//============================================================================ 
void GPIO_Init(void)
{
	TRISAbits.TRISA5 = 0;  // �ⲿ��Ե���PWM���ƹܽ�
	
	TRISDbits.TRISD4 = 1;  // ����keyrun��Ϊ����
	TRISDbits.TRISD5 = 1;  // No Used
	TRISDbits.TRISD6 = 1;  // Ԥ������
	TRISDbits.TRISD7 = 1;  //��������������
}

/*
 * 
 */
void main(void) 
{
	static uint8_t taskList = 0;
	
    System_Init();    
    LedGreOn();
    for(;;)
    {
        // ��ѯ���ȼ��ϸ�����
        ClrWdt();
        #ifdef TEST_PIN
        TRIG_TEST(); 
        #endif
        TskCurrentMgt();
        Soc_AhAcc();
        Soh_ChargeAhAcc();
        ClrWdt();
        TskBatteryModeMgt();
        TskRelayMgt();
        TskCanRecMsgToBuf();
        //DetectRunkey();
		ClrWdt();
        switch(taskList++)
        {
		case 0:
			TskAfeMgt();
			RelayAction();
			break;
		case 1:
			TskSOCMgt();  // �ú���������ڵ������ִ�� 
			DetectCharger();
			break;
		case 2:
			TskCellTempMgt();
			TskAmbTempMgt(); //��·���¶�
			RelayAction();
			// ��Ե�Լ��
			break;
		case 3:
			TskCanMgt();
			TskFaultStoreMgt(); 
			RelayAction();
			break;
		case 4:
			TaskLedMgt();
			RelayAction();
			break;
		case 5:
            Soh_UpdateCycleTime();
			if(g_BatteryMode == CHARGE)
			{
				Soh_StoreChargedAh();
			}
			taskList = 0;
            break;
		default:
			taskList = 0;
			break;	
        }
        while(!PIR4bits.TMR4IF);
            PIR4bits.TMR4IF = 0;       
        ClrWdt();
    }
}

void System_Init(void)
{
	ClrWdt();
    SysClk_Init();  // ϵͳʱ�ӳ�ʼ��
    GPIO_Init();	// 
    ISR_Init();     // �жϴ�������ʼ��
    SPI_Init();      // SPI��ʼ��
    ECAN_Init();    // оƬCAN���üĴ�����ʼ��
    Timer_Init();   // ��ʱ����ʼ��
    ADC_IOInit();   // ADC��ʼ��
    Led_Init();     // LED���ų�ʼ��
    Relay_Init();   // �̵����������ų�ʼ��
    
    ClrWdt();
    
    TskBatteryPra_Init();	// ��ز��ֲ����ĳ�ʼ��
    Ltc6803_Init();
    TskAdc_Init();			// ģ��ģ�������ʼ��
    TskCan_Init();
    ClrWdt();
    Soc_Init();
	Soh_Init();
	FaultStoreInit();

    ClrWdt();
    g_BatteryMode 			= IDLE;
    g_ProtectDelayCnt 		= 0xffff;
    g_EnterLowPoweModeFlg 	= 0;	// ����͹���״̬
    g_SystemWarning.all 	= 0;    // clear system warning flags
    //g_SystemError.all 		= 0xf8;

    SystemSelftest();  		// ϵͳ�Լ�
    Soc_PowerOnAdjust();	// SOC �ϵ�У׼
    CurrentZeroOffsetAdjust();  // �ϵ�ִ�е������У׼

	WDTCONbits.SWDTE = 1; 	// �������Ź�

    ClrWdt();

}


