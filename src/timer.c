#include "include.h"

#define USETIMER1
#define USETIMER2
#define USETIMER4

//----------------------------------------------------------------------------
// Function    : Timer_Init
// Description : This function config the timer0-timer4,
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------

void Timer_Init(void)
{
#ifdef USETIMER0
	// Timer0配置，10ms产生中断
	TMR0H	= TMR0H_INIT;
	TMR0L	= TMR0L_INIT;

	T0CONbits.T0PS		= 0b000;	// 预分频比1:2
	T0CONbits.PSA		= 0b0;		// 不经过Timer0预分频器
	T0CONbits.T0SE		= 0b1;		// 在 T0CKI 引脚信号从高至低跳变时，递增计数
	T0CONbits.T0CS		= 0b0;		// 内部指令周期时钟 （ CLKO）,Fosc/4，即定时器模式
	T0CONbits.T08BIT 	= 0x0;		// Timer0 被配置为 16 位定时器 / 计数器
	T0CONbits.TMR0ON	= 0x1;		// 使能 Timer0

	INTCONbits.TMR0IE	= 0b1;		// 允许 TMR0 溢出中断
	INTCONbits.TMR0IF	= 0x0;		// 清楚TMR0中断标志位
	
#endif	

#ifdef USETIMER1
	// Timer1配置，1ms产生中断，高优先级中断

	T1CONbits.TMR1CS 		= 0b00;	// 时钟源为系统时钟(FOSC/4)
	T1CONbits.T1CKPS 		= 0b01;	// 预分频比1:2
	T1CONbits.SOSCEN 		= 0b0;	// 禁止 SOSC，它不可用于 Timer1
	T1CONbits.NOT_T1SYNC 	= 0b0;	// 同步外部时钟输入
	T1CONbits.RD16			= 0b1;	// 通过一次16位操作进行寄存器读写
	T1CONbits.TMR1ON		= 0b1;	// 时能TIMER1

	TMR1H = TMR1H_INIT;
	TMR1L = TMR1L_INIT;
	

	PIR1bits.TMR1IF	= 0b0;	// 清除外设中断请求寄存器定时器1的中断标志位
	IPR1bits.TMR1IP	= 0b1;	// 设置定时器1的中断源为高优先级
	PIE1bits.TMR1IE = 0b1;	// 允许定时器1的中断
#endif

#ifdef USETIMER2
    
	// 用于产生SPI clock
	PR2 = 18;
	T2CON = 0x04;
	PIE1bits.TMR2IE = 0b0;	// 不允许定时器2的中断

#endif	

#ifdef USETIMER3
	//Timer3配置，5ms产生中断，高优先级中断
	T3CONbits.TMR3CS	= 0b01;	// Timer3 时钟源为系统时钟（ FOSC）
	T3CONbits.T3CKPS	= 0b11;	// 1:8 预分频比
	T3CONbits.SOSCEN	= 0b0;	// 使能 SOSC，它可用于 Timer3
	T3CONbits.RD16		= 0b1;	// 使能 Timer3 通过一次 16 位操作进行寄存器读 / 写
	T3CONbits.TMR3ON	= 0b1;	// 使能 Timer3

	TMR3H = TMR3H_INIT;
	TMR3L = TMR3L_INIT;
	
	PIR2bits.TMR3IF	= 0b0;	// 清除外设中断请求寄存器定时器3的中断标志位
	PIE2bits.TMR3IE = 0b1;	// 允许定时器3的中断
	
#endif

#ifdef USETIMER4
	// Timer4配置，2.0ms产生重置
	// 用于产生Task时间片
	PR4 = 200;
	T4CON = 0x4F;
	PIR4bits.TMR4IF = 0;
	PIE4bits.TMR4IE = 0;
#endif


}

