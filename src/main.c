/* 
 * File:   main.c
 * Author: Administrator
 *
 * Created on 2016年6月20日, 上午8:14
 */

#include "include.h"

// PIC18F46K80 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = HIGH   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = SWDTDIS  // Watchdog Timer (WDT enabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

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


void System_Init(void);

//============================================================================
// Function    ：SysClk_Init
// Description ：Initialize the system clock Fosc.
// Parameters  ：none 
// Returns     ：none
//============================================================================
void SysClk_Init(void)
{   
    OSCCONbits.SCS 		= 0b00;     //Select INOSC as system clock source
    OSCCONbits.IRCF 	= 0b111;    //设置所选HF_INTOSC输出频率(16MHz)
    OSCCONbits.IDLEN 	= 0b0;      //when implement sleep function,mcu goto sleep 
    OSCTUNEbits.TUN 	= 0b0;		//快速RC振荡器(INTOSC)频率调节 暂时不调节，使用默认校验好的
    OSCTUNEbits.PLLEN 	= 0b1;      //enable PLL  系统时钟运行在16MHz*4 
}


//============================================================================
// Function    ：GPIO_Init
// Description ：单片机部分IO管脚初始化，部分IO的初始化可能在相应模块初始化函
//               数中执行
// Parameters  ：none 
// Returns     ：none
//============================================================================ 
void GPIO_Init(void)
{
	TRISAbits.TRISA1 = 0;  //外部故障清除控制管脚
	TRISAbits.TRISA5 = 0;  //外部绝缘检测PWM控制管脚
	TRISAbits.TRISA7 = 0;
	TRISBbits.TRISB5 = 0; //风扇控制输出
	TRISBbits.TRISB6 = 0; //风扇控制输出
	LATAbits.LATA5 = 0;

	TRISDbits.TRISD4 = 1;  //设置点火检测管脚为输入
	TRISDbits.TRISD5 = 1;  //设置硬件故障检测管脚为输入
	TRISDbits.TRISD6 = 1;  // 预留输入

	TRISDbits.TRISD7 = 1;  //设置充电检测管脚为输入
}


//============================================================================
// Function    ：ClearHardwareFault
// Description ：复位故障锁存状态
// Parameters  ：none 
// Returns     ：none
//============================================================================
void ClearHardwareFault(void)
{
   uint32_t time_dly = g_SysTickMs;

   LATAbits.LATA1 = 1;
   while (time_dly >= g_SysTickMs-5);
   LATAbits.LATA1 = 0;
   while (time_dly >= g_SysTickMs-10);
   LATAbits.LATA1 = 1;
} 


/*
 * 
 */
void main(void) 
{
	static uint8_t TaskList = 0;
	
    System_Init();    
    LedRed = 0;
    LedGre = 0;
    for(;;)
    {
        // 查询优先级较高任务
        ClrWdt();
        TskCurrentMgt();
        Soc_AhAcc();
        Soh_ChargeAhAcc();
        TskBatteryModeMgt();
        RelayAction();
        TskCanRecMsgToBuf();
        DetectRunkey();
		
        switch(TaskList++)
        {
		case 0:
			TskAfeMgt();
			break;
		case 1:
			TskSOCMgt();  // 该函数必须放在单体检测后执行 
			DetectCharger();
			break;
		case 2:
			TskCellTempMgt();
			TskAmbTempMgt(); //电路板温度
			// 绝缘性检测
			break;
		case 3:
			TskCanMgt();
			TskFaultStoreMgt(); 
			break;
		case 4:
			TskRelayMgt();
			TaskLedMgt();
			break;
		default:
			ClrWdt();
            TaskList = 0;  
            Soh_UpdateCycleTime();    
            break;
        }
        while(!PIR4bits.TMR4IF);
        	PIR4bits.TMR4IF = 0; 
    }
}

void System_Init(void)
{
	ClrWdt();
    SysClk_Init();  // 系统时钟初始化
    GPIO_Init();	// 
    ISR_Init();     // 中断处理程序初始化
    SPI_Init();      // SPI初始化
    ECAN_Init();    // 芯片CAN配置寄存器初始化
    Timer_Init();   // 定时器初始化
    ADC_IOInit();   // ADC初始化
    Led_Init();     // LED引脚初始化
    Relay_Init();   // 继电器控制引脚初始化
    
    ClrWdt();
    
    TskBatteryPra_Init();	// 电池部分参数的初始化
    Ltc6803_Init();
    TskAdc_Init();			// 模拟模块参数初始化
    TskCan_Init();
    ClrWdt();
    Soc_Init();
	Soh_Init();
	FaultStoreInit();

    ClrWdt();
    g_BatteryMode 			= IDLE;
    g_BatterySubMode 		= CHARGING;
    g_ProtectDelayCnt 		= 0xffff;
    g_EnterLowPoweModeFlg 	= 0;
    g_SystemWarning.all 	= 0;  // clear system warning flags
    g_SystemError.all 		= 0xf8;

	ClearHardwareFault();
    SystemSelftest();  		// 系统自检
    Soc_PowerOnAdjust();	// SOC 上电校准
    CurrentZeroOffsetAdjust();  // 上电执行电流零点校准

	WDTCONbits.SWDTE = 1; 	// 开启看门狗

}


