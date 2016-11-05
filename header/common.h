/* 
 * File:   common.h
 * Author: Administrator
 *
 * Created on 2016年6月20日, 上午9:08
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

#define Fosc            64000000    //系统时钟频率64MHz     

//-----------------------------------------------------------------
//本系统要用到的独立于模块的数据类型

// 定义电池包相关参数的结构体数据
typedef struct 
{
	uint16_t 			voltage;          		// 电池包总电压 0.1v/bit 
	volatile int16_t  	current;          		// 电池包母线电流 0.1A/bit 
	int16_t 			curr_pre;

	uint8_t  			SOC;          			// 电池包SOC (%)  
	uint16_t			socCyclesPerSec;		// 每秒计算SOC相关量的次数
	int16_t				Ah;						// 剩余安时数
	int32_t				Accumulator;			// 计算电量的累加器
	
	uint16_t 			SOH;					//充电循环次数
	int16_t 			sohCyclesPerSec;
	uint32_t			ChargedAh;
	uint16_t			sohCycleTimes;
	
	uint16_t 			CellVoltPre[CellsAmount];
	uint16_t 			CellRes[CellsAmount];
	uint16_t 			CellVoltMax;   			// 电池包最大单体电压
	uint16_t 			CellVoltMin;          	// 电池包最小单体电压
	uint16_t 			CellVoltAvg;          	// 电池包平均单体电压
	uint8_t  			MaxCellNum;				// 最大最小电压电芯编号
	uint8_t  			MinCellNum;
	int8_t   			CellTemp[4];          	// cell temperature 只有4路温度  
	int8_t   			CellTempMax;          
	int8_t   			CellTempMin;   
	int8_t  			CellTempAvg; 
	uint8_t 			MaxTempNum;
	uint8_t 			MinTempNum; 
	uint8_t 			MaxTempModuIdx;
	uint8_t 			MinTempModuIdx;
	uint8_t 			MaxTempChnIdx;
	uint8_t 			MinTempChnIdx;     
	int8_t   			AmbientTemp;          	// 板上环境温度
	uint16_t 			BalanceCellNum;       	// 指示哪几节cell在均衡
	uint8_t  			BalanceFlag;			// 是否处于均衡处理            
}BatteryPackTypedef;


//系统的报警信息类型
typedef union
{
	uint32_t all;

	struct 
	{
		unsigned COV :2;        // 单体电池过压
		unsigned CUV :2;        // 单体电池欠压
		unsigned COT :2;		// 单体充电高温
		unsigned CUT :2;		// 单体充电低温
		unsigned DOT :2;		// 单体放电高温
		unsigned DUT :2;		// 单体放电低温
		unsigned COC :2;		// 充电过流
		unsigned DOC :2;		// 放电过流  	
		unsigned CIB :2;     	// 单体电池一致性   
		unsigned PCBOT :2;    	// PCB板高温     
		unsigned PCBUT :2;    	// PCB板低温    
		unsigned POV :2;		// 整包总压高
		unsigned PUV :2;		// 整包总压低
		unsigned TIB :2;		// 电池温差大
		unsigned ISO :2;		// 绝缘警告
		unsigned     :2;
	};
}SysWarningDef;

//电池包模式类型定义
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


//充放电过流等级
//单体电压警报等级
typedef struct
{
    int16_t cls_1;
    int16_t cls_2;
    uint8_t crc;
}CurrWarnClsDef,VoltWarnClsDef,TempWarnClsDef;


//电池包错误信息数据类型
typedef union
{
	uint8_t all;

	struct 
	{
		unsigned det_oc	    :1;         // 检测线开路
		unsigned ltc_st	    :1;         // ltc6803错误
		unsigned ltc_com	:1;         // ltc6803通信错误 
		unsigned pcb_ot	    :1;         // 板上温度过高 
		unsigned nbr_comm   :1;         // 邻近电池包通信       
		unsigned mcs_comm   :1;         // 电机控制器通信
		unsigned tdu_comm	:1;         // 温度子板通信
		unsigned ccs_comm   :1;         // 充电器CAN通信 
	};
}SysErrDef;


// 模拟前端管理数据类型
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


//独立于模块的全局变量的声明
extern volatile BatteryPackTypedef 		g_BatteryParameter; //电池参数
extern volatile SysWarningDef 	g_SystemWarning;	//系统报警
extern volatile SysErrDef 		g_SystemError;

extern volatile BatteryModeTypedef 		g_BatteryMode; 		// 电池包工作模式
extern SubModeTypedef 			g_BatterySubMode;

extern uint16_t 				g_ProtectDelayCnt;	// 保护延时计数 
extern uint16_t 				g_PrechargeTimer;	// 预充时间

extern uint8_t 					g_EnterLowPoweModeFlg; // 进入低功耗模式标志位


extern CurrWarnClsDef 	g_BattCOCThr;		//充电过流
extern CurrWarnClsDef 	g_BattDOCThr;		//放电过流
extern VoltWarnClsDef 	g_CellOVThr;
extern VoltWarnClsDef 	g_CellUVThr;
extern VoltWarnClsDef 	g_CellIBMThr;	// 单体一致性参数
extern VoltWarnClsDef 	g_PackOVThr;
extern VoltWarnClsDef 	g_PackUVThr;
extern VoltWarnClsDef  	g_IsoThr;

extern TempWarnClsDef 	g_PACKCOTThr;	// 电池包充电高温
extern TempWarnClsDef 	g_PACKCUTThr;	// 电池包充电低温
extern TempWarnClsDef 	g_PACKDOTThr;	// 电池包放电高温
extern TempWarnClsDef 	g_PACKDUTThr;	// 电池包放电低温
extern TempWarnClsDef 	g_PACKDLTThr;	// 电池包温差大告警

const  VoltWarnClsDef CellOVThrDefault = {4150,4180,0}; // mV
const  VoltWarnClsDef CellUVThrDefault = {3000,2850,0}; // mV
const  VoltWarnClsDef CellIBMThrDefault = {200,300,0};
const  VoltWarnClsDef PackOVThrDefault = {820,830,0}; // 0.1V
const  VoltWarnClsDef PackUVThrDefault = {600,570,0}; // 0.1V

extern uint16_t g_CCS_MaxVoltage;		// 控制充电器的最大电压 0.1V
extern uint16_t g_CCS_MaxCurrent;		// 控制充电器的最大电流 0.1A


//-------------------------- 常用宏定义 --------------------------

// 1.求最大值和最小值 
#define     MAX(x, y)                  (((x) > (y)) ? (x) : (y)) 
#define     MIN(x, y)                  (((x) < (y)) ? (x) : (y))
// 2. 绝对值和两个数差值
#define     ABS(x)                     ((x) >= 0 ? (x) : -(x))
#define     ABS_DIFF(x, y)             ((x) > (y) ? (x - y) : (y - x))
// 3.返回数组元素的个数 
#define     ARR_SIZE(x)                (sizeof((x)) / sizeof((x[0])))


#ifdef	__cplusplus
}
#endif

#endif	/* COMMON_H */

