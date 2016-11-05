/* 
 * File:   common.h
 * Author: Administrator
 *
 * Created on 2016��6��20��, ����9:08
 */

#ifndef COMMON_H
#define	COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"
    
typedef enum
{
	FALSE = 0,
	TRUE
}BOOL;

typedef   unsigned char         uint8_t;
typedef   signed   char         int8_t;
typedef   unsigned short        uint16_t;
typedef   signed   short        int16_t;
typedef   unsigned long         uint32_t;
typedef   signed   long         int32_t;
typedef   unsigned short long   uint24_t;


//FOSC = 64MHz
#define TMR0H_INIT        0x63     // 5ms interrupt / 500ns timer clock = 40,000
#define TMR0L_INIT        0xC1     // cnts  65536 - 40,000 = 25536(0x63C0)

#define TMR1H_INIT        0xE0     // 1ms interrupt 
#define TMR1L_INIT        0xC0     // 

#define TMR3H_INIT        0x63     // 5ms interrupt / 500ns timer clock = 40,000
#define TMR3L_INIT        0xC1     // cnts  65536 - 40,000 = 25536(0x63C0)

#define Fosc            64000000    //ϵͳʱ��Ƶ��64MHz     

//-----------------------------------------------------------------
//��ϵͳҪ�õ��Ķ�����ģ�����������

// �����ذ���ز����Ľṹ������
typedef struct 
{
	uint16_t 			voltage;          		// ��ذ��ܵ�ѹ 0.1v/bit 
	volatile int16_t  	current;          		// ��ذ�ĸ�ߵ��� 0.1A/bit 
	int16_t 			curr_pre;

	uint8_t  			SOC;          			// ��ذ�SOC (%)  
	uint16_t			socCyclesPerSec;		// ÿ�����SOC������Ĵ���
	int16_t				Ah;						// ʣ�లʱ��
	int32_t				Accumulator;			// ����������ۼ���
	
	uint16_t 			SOH;					//���ѭ������
	int16_t 			sohCyclesPerSec;
	uint32_t			ChargedAh;
	uint16_t			sohCycleTimes;
	
	uint16_t 			CellVoltPre[CellsAmount];
	uint16_t 			CellRes[CellsAmount];
	uint16_t 			CellVoltMax;   			// ��ذ�������ѹ
	uint16_t 			CellVoltMin;          	// ��ذ���С�����ѹ
	uint16_t 			CellVoltAvg;          	// ��ذ�ƽ�������ѹ
	uint8_t  			MaxCellNum;				// �����С��ѹ��о���
	uint8_t  			MinCellNum;
	int8_t   			CellTemp[4];          	// cell temperature ֻ��4·�¶�  
	int8_t   			CellTempMax;          
	int8_t   			CellTempMin;   
	int8_t  			CellTempAvg; 
	uint8_t 			MaxTempNum;
	uint8_t 			MinTempNum; 
	uint8_t 			MaxTempModuIdx;
	uint8_t 			MinTempModuIdx;
	uint8_t 			MaxTempChnIdx;
	uint8_t 			MinTempChnIdx;     
	int8_t   			AmbientTemp;          	// ���ϻ����¶�
	uint16_t 			BalanceCellNum;       	// ָʾ�ļ���cell�ھ���
	uint8_t  			BalanceFlag;			// �Ƿ��ھ��⴦��            
}BatteryPackTypedef;


//ϵͳ�ı�����Ϣ����
typedef union
{
	uint32_t all;

	struct 
	{
		unsigned COV :2;        // �����ع�ѹ
		unsigned CUV :2;        // ������Ƿѹ
		unsigned COT :2;		// ���������
		unsigned CUT :2;		// ���������
		unsigned DOT :2;		// ����ŵ����
		unsigned DUT :2;		// ����ŵ����
		unsigned COC :2;		// ������
		unsigned DOC :2;		// �ŵ����  	
		unsigned CIB :2;     	// ������һ����   
		unsigned PCBOT :2;    	// PCB�����     
		unsigned PCBUT :2;    	// PCB�����    
		unsigned POV :2;		// ������ѹ��
		unsigned PUV :2;		// ������ѹ��
		unsigned TIB :2;		// ����²��
		unsigned ISO :2;		// ��Ե����
		unsigned     :2;
	};
}SysWarningDef;

//��ذ�ģʽ���Ͷ���
typedef enum 
{
	IDLE = 0,
	PRECHARGE,
	DISCHARGE, 
	CHARGE,
	PROTECTION
}BatteryModeTypedef;

typedef enum
{
	CHARGER_IN = 0,
	RUNKEY_IN,
	CHARGING,
	CHARGE_END
}SubModeTypedef;


//��ŵ�����ȼ�
//�����ѹ�����ȼ�
typedef struct
{
    int16_t cls_1;
    int16_t cls_2;
    uint8_t crc;
}CurrWarnClsDef,VoltWarnClsDef,TempWarnClsDef;


//��ذ�������Ϣ��������
typedef union
{
	uint8_t all;

	struct 
	{
		unsigned det_oc	    :1;         // ����߿�·
		unsigned ltc_st	    :1;         // ltc6803����
		unsigned ltc_com	:1;         // ltc6803ͨ�Ŵ��� 
		unsigned pcb_ot	    :1;         // �����¶ȹ��� 
		unsigned nbr_comm   :1;         // �ڽ���ذ�ͨ��       
		unsigned mcs_comm   :1;         // ���������ͨ��
		unsigned tdu_comm	:1;         // �¶��Ӱ�ͨ��
		unsigned ccs_comm   :1;         // �����CANͨ�� 
	};
}SysErrDef;


// ģ��ǰ�˹�����������
typedef enum 
{
	AFE_VOLT_CNVT = 0,      
	AFE_READ_VOLT,
	AFE_VOLT_DETECT,
	AFE_TEMP_CNVT,
	AFE_READ_TEMP, 
	AFE_BALANCE           
}AfeStateTypedef;


//-----------------------------------------------------------------


//������ģ���ȫ�ֱ���������
extern volatile BatteryPackTypedef 		g_BatteryParameter; //��ز���
extern volatile SysWarningDef 	g_SystemWarning;	//ϵͳ����
extern volatile SysErrDef 		g_SystemError;

extern volatile BatteryModeTypedef 		g_BatteryMode; 		// ��ذ�����ģʽ
extern SubModeTypedef 			g_BatterySubMode;

extern uint16_t 				g_ProtectDelayCnt;	// ������ʱ���� 
extern uint16_t 				g_PrechargeTimer;	// Ԥ��ʱ��

extern uint8_t 					g_EnterLowPoweModeFlg; // ����͹���ģʽ��־λ


extern CurrWarnClsDef 	g_BattCOCThr;		//������
extern CurrWarnClsDef 	g_BattDOCThr;		//�ŵ����
extern VoltWarnClsDef 	g_CellOVThr;
extern VoltWarnClsDef 	g_CellUVThr;
extern VoltWarnClsDef 	g_CellIBMThr;	// ����һ���Բ���
extern VoltWarnClsDef 	g_PackOVThr;
extern VoltWarnClsDef 	g_PackUVThr;
extern VoltWarnClsDef  	g_IsoThr;

extern TempWarnClsDef 	g_PACKCOTThr;	// ��ذ�������
extern TempWarnClsDef 	g_PACKCUTThr;	// ��ذ�������
extern TempWarnClsDef 	g_PACKDOTThr;	// ��ذ��ŵ����
extern TempWarnClsDef 	g_PACKDUTThr;	// ��ذ��ŵ����
extern TempWarnClsDef 	g_PACKDLTThr;	// ��ذ��²��澯

const  VoltWarnClsDef CellOVThrDefault = {4150,4180,0}; // mV
const  VoltWarnClsDef CellUVThrDefault = {3000,2850,0}; // mV
const  VoltWarnClsDef CellIBMThrDefault = {200,300,0};
const  VoltWarnClsDef PackOVThrDefault = {820,830,0}; // 0.1V
const  VoltWarnClsDef PackUVThrDefault = {600,570,0}; // 0.1V

extern uint16_t g_CCS_MaxVoltage;		// ���Ƴ����������ѹ 0.1V
extern uint16_t g_CCS_MaxCurrent;		// ���Ƴ������������ 0.1A


//-------------------------- ���ú궨�� --------------------------

// 1.�����ֵ����Сֵ 
#define     MAX(x, y)                  (((x) > (y)) ? (x) : (y)) 
#define     MIN(x, y)                  (((x) < (y)) ? (x) : (y))
// 2. ����ֵ����������ֵ
#define     ABS(x)                     ((x) >= 0 ? (x) : -(x))
#define     ABS_DIFF(x, y)             ((x) > (y) ? (x - y) : (y - x))
// 3.��������Ԫ�صĸ��� 
#define     ARR_SIZE(x)                (sizeof((x)) / sizeof((x[0])))


#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

