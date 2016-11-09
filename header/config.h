/* 
 * File:   config.h
 * Author: Administrator
 *
 * Created on 2016年7月1日, 上午11:34
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"

#define DEBUG

//顶一次项用于启用外部晶振
//#define EXSOC

// 配置的测试标志 如果进行发送数据测试，可以打开
#define SENDTEST	1

// 电芯涓流充电电压阀值
#define CELLPRECHARGEVOLT	(uint16_t)2000 // 单位 mV
// 电芯标准充电电压阀值
#define CELLSTDCHARGEVOLT	(uint16_t)2500
// 电芯恒流充电电压阀值
#define CELLCONSTCURRENTVOLT	(uint16_t)4000

/* 定义设计中ltc6803级联的数量
   该定义根据实际工程情况修改 */
#define  ModuleAmount          ((uint8_t)2)           // 定义设计中有2片ltc6803级联
#define  CellsAmount           ((uint8_t)20)          // 定义整个pack中的单体电芯数量

//配置检测温度数量
#define MAX_TEMP_SENSOR         ((uint8_t)4)

// SOC操作参数    
#define  SOC_UPDATE_PERIOD_MS         		2000    // update the soc every 2 seconds
#define	 SOC_SAMPLE_CYCLES_PER_SECOND		500     // 每秒采样次数 2ms采样一次

// 充电器和keyrun信号检测周期
#define KEY_CHK_CYCLE       ((uint8_t)10)
#define CHGR_CHK_CYCLE      ((uint8_t)10)

// 开启停止风扇控制的临界温度配置
#define FAN_ON_TEMPERATURE_CHARGE          40                // 定义充电状态风扇开启温度为40℃
#define FAN_OFF_TEMPERATURE_CHARGE         35                // 定义充电状态风扇关闭温度为35℃

#define FAN_ON_TEMPERATURE_DISCHARGE       55                // 定义充电状态风扇开启温度为55℃
#define FAN_OFF_TEMPERATURE_DISCHARGE      50                // 定义充电状态风扇关闭温度为50℃

// PCB板子温度阀值
#define PcbOverTempValue				60
#define PcbUnderTempValue				0

//============================ 定义故障报警等级 ==============================
#define  WARNING_FIRST_LEVEL                 1
#define  WARNING_SECOND_LEVEL                2

/* 电池包基本信息 */
#define  CELL_SERIES_NUM               ((uint8_t)20)     	// 定义电池串数
#define  CELL_PARALLEL_NUM			   ((uint8_t)30)        // 定义电池并数 
#define  BATTERY_CAPACITY_RATED		   ((int16_t)750)       // 新电池标称容量(扩大10倍)
#define  BATTERY_CAPACITY_TOTAL        ((int16_t)725)       // 新电池组满容量(扩大10倍)
#define  SOC_PACK_TOTAL_WH             ((int16_t)5400)    	// 电池组总容量(Wh)

//--------------------------------- 系统报警值设置 ---------------------------------
/* 系统告警值配置 */
#define  CELL_OV_WARNING               ((uint16_t)4180)     // 定义单体过压报警值 mv/bit
#define  CELL_UV_WARNING               ((uint16_t)3500)     // 定义单体欠压报警值 mv/bit
#define  CELL_IB_WARNING               ((uint16_t)100)      // 定义单体电压不一致报警值 mv/bit

#define  CELL_DOT_WARNING              ((int8_t)60)         // 定义放电超温报警值 ℃/bit
#define  CELL_DUT_WARNING              ((int8_t)-15)        // 定义放电低温报警值 ℃/bit
#define  CELL_COT_WARNING              ((int8_t)40)         // 定义充电超温报警值 ℃/bit
#define  CELL_CUT_WARNING              ((int8_t)5)          // 定义充电低温报警值 ℃/bit
#define  PACK_DOC_WARNING              ((int16_t)2000)      // 定义放电过流报警值 0.1A/bit      
#define  PACK_COC_WARNING              ((int16_t)-220)      // 定义充电过流报警值 0.1A/bit
#define  PACK_SOC_WARNING              ((uint8_t)30)
#define  RELAY_COT_WARNING             ((int8_t)60)         // 定义充电继电器超温报警值 ℃/bit
#define  RELAY_DOT_WARNING             ((int8_t)60)         // 定义放电继电器超温报警值 ℃/bit
#define  PACK_SOC_WARN_CLS_1           ((uint8_t)30)


/* 系统故障值配置 */
#define  CELL_OV_FAULT                 ((uint16_t)4200)     // 定义单体过压故障值 mv/bit
#define  CELL_UV_FAULT                 ((uint16_t)3000)     // 定义单体欠压故障值 mv/bit
#define  CELL_IB_FAULT                 ((uint16_t)150)      // 定义单体电压不一致故障值 mv/bit

#define  CELL_DOT_FAULT                ((int8_t)65)         // 定义系统超温故障值 ℃/bit
#define  CELL_DUT_FAULT                ((int16_t)-20)        // 定义系统低温故障值 ℃/bit
#define  CELL_COT_FAULT                ((int8_t)45)         // 定义系统超温故障值 ℃/bit
#define  CELL_CUT_FAULT                ((int8_t)0)          // 定义系统低温故障值 ℃/bit
#define  PACK_DOC_FAULT                ((int16_t)2500)       // 定义放电过流故障值 0.1A/bit 
#define  PACK_COC_FAULT                ((int16_t)-250)      // 定义充电过流故障值 0.1A/bit
#define  PACK_SOC_FAULT                ((uint8_t)10)
#define  RELAY_COT_FAULT               ((int8_t)65)           // 定义充电继电器超温报警值 ℃/bit
#define  RELAY_DOT_FAULT               ((int8_t)65)           // 定义放电继电器超温报警值 ℃/bit
#define  PACK_SOC_WARN_CLS_2           ((uint8_t)10)

//======================================================================================
/* 系统报警时间设置 */
#define CELL_OV_WARNING_DLY				((uint8_t)83)
#define CELL_UV_WARNING_DLY				((uint8_t)83)
#define CELL_IB_WARNING_DLY				((uint8_t)83)
#define CELL_DOT_WARNING_DLY			((uint8_t)125)
#define CELL_DUT_WARNING_DLY			((uint8_t)125)
#define CELL_COT_WARNING_DLY			((uint8_t)125)
#define CELL_CUT_WARNING_DLY			((uint8_t)125)
#define CELL_TIB_WARNING_DLY            ((uint8_t)125)
#define PACK_DOC_WARNING_DLY			((uint16_t)250)
#define PACK_COC_WARNING_DLY			((uint16_t)2500)
#define PACK_UV_WARNING_DLY             ((uint8_t)83)
#define PACK_OV_WARNING_DLY             ((uint8_t)83)
#define SYS_ISO_WARNING_DLY             ((uint8_t)25) 

/* 系统故障时间设置 */
#define CELL_OV_FAULT_DLY				((uint8_t)83)
#define CELL_UV_FAULT_DLY				((uint8_t)83)
#define CELL_IB_FAULT_DLY				((uint8_t)83)            
#define PACK_UV_FAULT_DLY               ((uint8_t)83)
#define PACK_OV_FAULT_DLY               ((uint8_t)83)
#define CELL_DOT_FAULT_DLY				((uint8_t)125)
#define CELL_DUT_FAULT_DLY				((uint8_t)125)
#define CELL_COT_FAULT_DLY				((uint8_t)125)
#define CELL_CUT_FAULT_DLY				((uint8_t)125)
#define CELL_TIB_FAULT_DLY              ((uint8_t)125)
#define PACK_DOC_FAULT_DLY				((uint16_t)250)
#define PACK_COC_FAULT_DLY				((uint16_t)2500)
#define SYS_ISO_FAULT_DLY               ((uint8_t)50)   
#define RELAY_ACTION_DELAY_1S           ((uint16_t)500)
#define RELAY_ACTION_DELAY_3S           ((uint16_t)1500)
#define RELAY_ACTION_DELAY_5S           ((uint16_t)2500)
#define RELAY_ACTION_DELAY_10S          ((uint16_t)5000)
#define RELAY_ACTION_DELAY_20S          ((uint16_t)10000)

//======================================================================================
/* 故障保护恢复点设置 */
#define  CELL_DOT_RECOVER              ((int8_t)53)         // 定义放电超温故障恢复值
#define  CELL_DUT_RECOVER              ((int16_t)-15)        // 定义放电低温故障恢复值
#define  CELL_COT_RECOVER              ((int8_t)44)         // 定义充电超温故障恢复值
#define  CELL_CUT_RECOVER              ((int8_t)5)          // 定义充电低温故障恢复值
#define  PACK_CHARGE_LOW_TEMP          ((int8_t)6)          // 定义低温小电流充电温度值
#define  PACK_CHARGE_HIGH_TEMP         ((int8_t)43)         // 定义低温小电流充电温度值
#define  MOSFET_OT_RECOVER             ((int8_t)60)         // 定义MOSFET超温故障恢复值

//======================================================================================
/* 充放电截止点设置 */
#define  CELL_CHARGE_LIMIT_VOLT        ((uint16_t)4200)     // 单体充电截止电压 mv/bit
#define  CELL_CHARGE_FULL_VOLT         ((uint16_t)4150)     // 单体充电截止电压 mv/bit
#define  CELL_DISCHARGE_END_VOLT       ((uint16_t)3000)     // 单体放电截止电压 mv/bit

//-------------------------------- 单体均衡参数定义 --------------------------------
#define  CELL_BALANCE_THRESHOLD        ((uint8_t)20)        // 定义cell均衡启动门槛压差：10mV
#define  CELL_BALANCE_OPEN_VOLT        ((uint8_t)3650)      // 定义cell均衡启动电压最低值
#define  CELL_BALANCE_PROTECT_VOLT     ((uint16_t)4100)     // 定义单体均衡保护电压

#define MAX_CHARGER_OUTPUT_VOLT		   ((uint16_t)840)		// uint: 0.1V
#define MAX_CHARGER_OUTPUT_CURR		   ((uint16_t)200)		// uintt: 0.1A

// ========= 系统中各参数在EEPROM中存放的地址配置==============                                            
#define  EEPROM_ADDR_SOC                     ((uint16_t)0x0000)       
#define  EEPROM_ADDR_ACC                     ((uint16_t)0x0002)       
#define  EEPROM_ADDR_CHARGEDAH               ((uint16_t)0x0006)       
#define  EEPROM_ADDR_CYCLE_TIME              ((uint16_t)0x000A)       

#define EEPROM_ADDR_COV_THRHOLD             ((uint16_t)0x0010)
#define EEPROM_ADDR_CUV_THRHOLD             ((uint16_t)(EEPROM_ADDR_COV_THRHOLD + 5))
#define EEPROM_ADDR_DLV_THRHOLD             ((uint16_t)(EEPROM_ADDR_CUV_THRHOLD + 5))
#define EEPROM_ADDR_COT_THRHOLD             ((uint16_t)(EEPROM_ADDR_DLV_THRHOLD + 5))
#define EEPROM_ADDR_CUT_THRHOLD             ((uint16_t)(EEPROM_ADDR_COT_THRHOLD + 5))
#define EEPROM_ADDR_DOT_THRHOLD             ((uint16_t)(EEPROM_ADDR_CUT_THRHOLD + 5))
#define EEPROM_ADDR_DUT_THRHOLD             ((uint16_t)(EEPROM_ADDR_DOT_THRHOLD + 5))
#define EEPROM_ADDR_DLT_THRHOLD             ((uint16_t)(EEPROM_ADDR_DUT_THRHOLD + 5))
#define EEPROM_ADDR_COC_THRHOLD             ((uint16_t)(EEPROM_ADDR_DLT_THRHOLD + 5))
#define EEPROM_ADDR_DOC_THRHOLD             ((uint16_t)(EEPROM_ADDR_COC_THRHOLD + 5))
#define EEPROM_ADDR_POV_THRHOLD             ((uint16_t)(EEPROM_ADDR_DOC_THRHOLD + 5))
#define EEPROM_ADDR_PUV_THRHOLD             ((uint16_t)(EEPROM_ADDR_POV_THRHOLD + 5))
#define EEPROM_ADDR_ISO_THRHOLD             ((uint16_t)(EEPROM_ADDR_PUV_THRHOLD + 5))
/* 定义GUI在MCU中的存储空间128Bytes */
/* 范围：0x0280~0x02FF */
#define  EEPROM_ADDR_FOR_GUI                 ((uint16_t)0x0280)

// ==================系统中错误信息在EEPROM中存储的地址配置=======================
#define EEPROM_ADDR_RECENT_FAULT_1		((uint16_t)0x03D7)
#define EEPROM_ADDR_RECENT_FAULT_2		((uint16_t)0x03D8)
#define EEPROM_ADDR_RECENT_FAULT_3		((uint16_t)0x03D9)	
#define EEPROM_ADDR_RECENT_FLT_PTR		EEPROM_ADDR_RECENT_FAULT_1	

#define  EEPROM_ADDR_FAULT_CUV   		((uint16_t)0x03DA)
#define  EEPROM_ADDR_FAULT_COV         	((uint16_t)0x03DC)
#define  EEPROM_ADDR_FAULT_VDIF         ((uint16_t)0x03DE)
#define  EEPROM_ADDR_FAULT_CUT         	((uint16_t)0x03E0)
#define  EEPROM_ADDR_FAULT_DUT         	((uint16_t)0x03E2)
#define  EEPROM_ADDR_FAULT_COT         	((uint16_t)0x03E4)
#define  EEPROM_ADDR_FAULT_DOT         	((uint16_t)0x03E6)
#define  EEPROM_ADDR_FAULT_COC          ((uint16_t)0x03E8)
#define  EEPROM_ADDR_FAULT_DOC       	((uint16_t)0x03EA)
#define  EEPROM_ADDR_FAULT_CER         	((uint16_t)0x03EC)
#define  EEPROM_ADDR_FAULT_LTCCOM    	((uint16_t)0x03EE)
#define  EEPROM_ADDR_FAULT_LTCST    	((uint16_t)0x03F0)
#define  EEPROM_ADDR_FAULT_POV      	((uint16_t)0x03F2) 
#define  EEPROM_ADDR_FAULT_PUV      	((uint16_t)0x03F4)
#define  EEPROM_ADDR_FAULT_TDIF      	((uint16_t)0x03F6)


//错误参数 
#define FAULT_REC_LIMIT          0xFFFF

//============================= 系统软件版本号 ===============================
#define  _FIRMWARE_MAJOR_VERSION             1                 //主版本号
#define  _FIRMWARE_MINOR_VERSION             0                 //次版本号
#define  _FIRMWARE_REVISE_VERSION            1                 //修订版本号
#define  _FIRMWARE_DATE_VERSION              0331              //日期版本号

//============================= 系统硬件版本号 ===============================
#define  _HARDWARE_MAJOR_VERSION             1                 //主版本号
#define  _HARDWARE_MINOR_VERSION             0                 //次版本号

#define  PRE_CHARGE_TIME                     500       // 定义预充电继电器闭合时间ms


//========================== application image Id ============================
#define _IMG_ID                        4
#define _IMG_APPHDR_ADDR               0x2000
    
#define CANTEST


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

