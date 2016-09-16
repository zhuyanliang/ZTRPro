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
	// Timer0���ã�10ms�����ж�
	TMR0H	= TMR0H_INIT;
	TMR0L	= TMR0L_INIT;

	T0CONbits.T0PS		= 0b000;	// Ԥ��Ƶ��1:2
	T0CONbits.PSA		= 0b0;		// ������Timer0Ԥ��Ƶ��
	T0CONbits.T0SE		= 0b1;		// �� T0CKI �����źŴӸ���������ʱ����������
	T0CONbits.T0CS		= 0b0;		// �ڲ�ָ������ʱ�� �� CLKO��,Fosc/4������ʱ��ģʽ
	T0CONbits.T08BIT 	= 0x0;		// Timer0 ������Ϊ 16 λ��ʱ�� / ������
	T0CONbits.TMR0ON	= 0x1;		// ʹ�� Timer0

	INTCONbits.TMR0IE	= 0b1;		// ���� TMR0 ����ж�
	INTCONbits.TMR0IF	= 0x0;		// ���TMR0�жϱ�־λ
	
#endif	

#ifdef USETIMER1
	// Timer1���ã�1ms�����жϣ������ȼ��ж�

	T1CONbits.TMR1CS 		= 0b00;	// ʱ��ԴΪϵͳʱ��(FOSC/4)
	T1CONbits.T1CKPS 		= 0b01;	// Ԥ��Ƶ��1:2
	T1CONbits.SOSCEN 		= 0b0;	// ��ֹ SOSC������������ Timer1
	T1CONbits.NOT_T1SYNC 	= 0b0;	// ͬ���ⲿʱ������
	T1CONbits.RD16			= 0b1;	// ͨ��һ��16λ�������мĴ�����д
	T1CONbits.TMR1ON		= 0b1;	// ʱ��TIMER1

	TMR1H = TMR1H_INIT;
	TMR1L = TMR1L_INIT;
	

	PIR1bits.TMR1IF	= 0b0;	// ��������ж�����Ĵ�����ʱ��1���жϱ�־λ
	IPR1bits.TMR1IP	= 0b1;	// ���ö�ʱ��1���ж�ԴΪ�����ȼ�
	PIE1bits.TMR1IE = 0b1;	// ����ʱ��1���ж�
#endif

#ifdef USETIMER2
    
	// ���ڲ���SPI clock
	PR2 = 18;
	T2CON = 0x04;
	PIE1bits.TMR2IE = 0b0;	// ������ʱ��2���ж�

#endif	

#ifdef USETIMER3
	//Timer3���ã�5ms�����жϣ������ȼ��ж�
	T3CONbits.TMR3CS	= 0b01;	// Timer3 ʱ��ԴΪϵͳʱ�ӣ� FOSC��
	T3CONbits.T3CKPS	= 0b11;	// 1:8 Ԥ��Ƶ��
	T3CONbits.SOSCEN	= 0b0;	// ʹ�� SOSC���������� Timer3
	T3CONbits.RD16		= 0b1;	// ʹ�� Timer3 ͨ��һ�� 16 λ�������мĴ����� / д
	T3CONbits.TMR3ON	= 0b1;	// ʹ�� Timer3

	TMR3H = TMR3H_INIT;
	TMR3L = TMR3L_INIT;
	
	PIR2bits.TMR3IF	= 0b0;	// ��������ж�����Ĵ�����ʱ��3���жϱ�־λ
	PIE2bits.TMR3IE = 0b1;	// ����ʱ��3���ж�
	
#endif

#ifdef USETIMER4
	// Timer4���ã�2.0ms��������
	// ���ڲ���Taskʱ��Ƭ
	PR4 = 200;
	T4CON = 0x4F;
	PIR4bits.TMR4IF = 0;
	PIE4bits.TMR4IE = 0;
#endif


}

