/* 
 * File:   config.h
 * Author: Administrator
 *
 * Created on 2016��7��1��, ����11:34
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"

#define DEBUG

//��һ�������������ⲿ����
//#define EXSOC

// ���õĲ��Ա�־ ������з������ݲ��ԣ����Դ�
#define SENDTEST	1

// ��о�������ѹ��ֵ
#define CELLPRECHARGEVOLT	(uint16_t)2000 // ��λ mV
// ��о��׼����ѹ��ֵ
#define CELLSTDCHARGEVOLT	(uint16_t)2500
// ��о��������ѹ��ֵ
#define CELLCONSTCURRENTVOLT	(uint16_t)4000

/* ���������ltc6803����������
   �ö������ʵ�ʹ�������޸� */
#define  ModuleAmount          ((uint8_t)2)           // �����������2Ƭltc6803����
#define  CellsAmount           ((uint8_t)20)          // ��������pack�еĵ����о����

//���ü���¶�����
#define MAX_TEMP_SENSOR         ((uint8_t)4)

// SOC��������    
#define  SOC_UPDATE_PERIOD_MS         		2000    // update the soc every 2 seconds
#define	 SOC_SAMPLE_CYCLES_PER_SECOND		500     // ÿ��������� 2ms����һ��

// �������keyrun�źż������
#define KEY_CHK_CYCLE       ((uint8_t)10)
#define CHGR_CHK_CYCLE      ((uint8_t)10)

// ����ֹͣ���ȿ��Ƶ��ٽ��¶�����
#define FAN_ON_TEMPERATURE_CHARGE          40                // ������״̬���ȿ����¶�Ϊ40��
#define FAN_OFF_TEMPERATURE_CHARGE         35                // ������״̬���ȹر��¶�Ϊ35��

#define FAN_ON_TEMPERATURE_DISCHARGE       55                // ������״̬���ȿ����¶�Ϊ55��
#define FAN_OFF_TEMPERATURE_DISCHARGE      50                // ������״̬���ȹر��¶�Ϊ50��

// PCB�����¶ȷ�ֵ
#define PcbOverTempValue				60
#define PcbUnderTempValue				0

//============================ ������ϱ����ȼ� ==============================
#define  WARNING_FIRST_LEVEL                 1
#define  WARNING_SECOND_LEVEL                2

/* ��ذ�������Ϣ */
#define  CELL_SERIES_NUM               ((uint8_t)20)     	// �����ش���
#define  CELL_PARALLEL_NUM			   ((uint8_t)30)        // �����ز��� 
#define  BATTERY_CAPACITY_RATED		   ((int16_t)750)       // �µ�ر������(����10��)
#define  BATTERY_CAPACITY_TOTAL        ((int16_t)725)       // �µ����������(����10��)
#define  SOC_PACK_TOTAL_WH             ((int16_t)5400)    	// �����������(Wh)

//--------------------------------- ϵͳ����ֵ���� ---------------------------------
/* ϵͳ�澯ֵ���� */
#define  CELL_OV_WARNING               ((uint16_t)4180)     // ���嵥���ѹ����ֵ mv/bit
#define  CELL_UV_WARNING               ((uint16_t)3500)     // ���嵥��Ƿѹ����ֵ mv/bit
#define  CELL_IB_WARNING               ((uint16_t)100)      // ���嵥���ѹ��һ�±���ֵ mv/bit

#define  CELL_DOT_WARNING              ((int8_t)60)         // ����ŵ糬�±���ֵ ��/bit
#define  CELL_DUT_WARNING              ((int8_t)-15)        // ����ŵ���±���ֵ ��/bit
#define  CELL_COT_WARNING              ((int8_t)40)         // �����糬�±���ֵ ��/bit
#define  CELL_CUT_WARNING              ((int8_t)5)          // ��������±���ֵ ��/bit
#define  PACK_DOC_WARNING              ((int16_t)2000)      // ����ŵ��������ֵ 0.1A/bit      
#define  PACK_COC_WARNING              ((int16_t)-220)      // �������������ֵ 0.1A/bit
#define  PACK_SOC_WARNING              ((uint8_t)30)
#define  RELAY_COT_WARNING             ((int8_t)60)         // ������̵������±���ֵ ��/bit
#define  RELAY_DOT_WARNING             ((int8_t)60)         // ����ŵ�̵������±���ֵ ��/bit
#define  PACK_SOC_WARN_CLS_1           ((uint8_t)30)


/* ϵͳ����ֵ���� */
#define  CELL_OV_FAULT                 ((uint16_t)4200)     // ���嵥���ѹ����ֵ mv/bit
#define  CELL_UV_FAULT                 ((uint16_t)3000)     // ���嵥��Ƿѹ����ֵ mv/bit
#define  CELL_IB_FAULT                 ((uint16_t)150)      // ���嵥���ѹ��һ�¹���ֵ mv/bit

#define  CELL_DOT_FAULT                ((int8_t)65)         // ����ϵͳ���¹���ֵ ��/bit
#define  CELL_DUT_FAULT                ((int16_t)-20)        // ����ϵͳ���¹���ֵ ��/bit
#define  CELL_COT_FAULT                ((int8_t)45)         // ����ϵͳ���¹���ֵ ��/bit
#define  CELL_CUT_FAULT                ((int8_t)0)          // ����ϵͳ���¹���ֵ ��/bit
#define  PACK_DOC_FAULT                ((int16_t)2500)       // ����ŵ��������ֵ 0.1A/bit 
#define  PACK_COC_FAULT                ((int16_t)-250)      // �������������ֵ 0.1A/bit
#define  PACK_SOC_FAULT                ((uint8_t)10)
#define  RELAY_COT_FAULT               ((int8_t)65)           // ������̵������±���ֵ ��/bit
#define  RELAY_DOT_FAULT               ((int8_t)65)           // ����ŵ�̵������±���ֵ ��/bit
#define  PACK_SOC_WARN_CLS_2           ((uint8_t)10)

//======================================================================================
/* ϵͳ����ʱ������ */
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

/* ϵͳ����ʱ������ */
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
/* ���ϱ����ָ������� */
#define  CELL_DOT_RECOVER              ((int8_t)53)         // ����ŵ糬�¹��ϻָ�ֵ
#define  CELL_DUT_RECOVER              ((int16_t)-15)        // ����ŵ���¹��ϻָ�ֵ
#define  CELL_COT_RECOVER              ((int8_t)44)         // �����糬�¹��ϻָ�ֵ
#define  CELL_CUT_RECOVER              ((int8_t)5)          // ��������¹��ϻָ�ֵ
#define  PACK_CHARGE_LOW_TEMP          ((int8_t)6)          // �������С��������¶�ֵ
#define  PACK_CHARGE_HIGH_TEMP         ((int8_t)43)         // �������С��������¶�ֵ
#define  MOSFET_OT_RECOVER             ((int8_t)60)         // ����MOSFET���¹��ϻָ�ֵ

//======================================================================================
/* ��ŵ��ֹ������ */
#define  CELL_CHARGE_LIMIT_VOLT        ((uint16_t)4200)     // �������ֹ��ѹ mv/bit
#define  CELL_CHARGE_FULL_VOLT         ((uint16_t)4150)     // �������ֹ��ѹ mv/bit
#define  CELL_DISCHARGE_END_VOLT       ((uint16_t)3000)     // ����ŵ��ֹ��ѹ mv/bit

//-------------------------------- �������������� --------------------------------
#define  CELL_BALANCE_THRESHOLD        ((uint8_t)20)        // ����cell���������ż�ѹ�10mV
#define  CELL_BALANCE_OPEN_VOLT        ((uint8_t)3650)      // ����cell����������ѹ���ֵ
#define  CELL_BALANCE_PROTECT_VOLT     ((uint16_t)4100)     // ���嵥����Ᵽ����ѹ

#define MAX_CHARGER_OUTPUT_VOLT		   ((uint16_t)840)		// uint: 0.1V
#define MAX_CHARGER_OUTPUT_CURR		   ((uint16_t)200)		// uintt: 0.1A

// ========= ϵͳ�и�������EEPROM�д�ŵĵ�ַ����==============                                            
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
/* ����GUI��MCU�еĴ洢�ռ�128Bytes */
/* ��Χ��0x0280~0x02FF */
#define  EEPROM_ADDR_FOR_GUI                 ((uint16_t)0x0280)

// ==================ϵͳ�д�����Ϣ��EEPROM�д洢�ĵ�ַ����=======================
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


//������� 
#define FAULT_REC_LIMIT          0xFFFF

//============================= ϵͳ����汾�� ===============================
#define  _FIRMWARE_MAJOR_VERSION             1                 //���汾��
#define  _FIRMWARE_MINOR_VERSION             0                 //�ΰ汾��
#define  _FIRMWARE_REVISE_VERSION            1                 //�޶��汾��
#define  _FIRMWARE_DATE_VERSION              0331              //���ڰ汾��

//============================= ϵͳӲ���汾�� ===============================
#define  _HARDWARE_MAJOR_VERSION             1                 //���汾��
#define  _HARDWARE_MINOR_VERSION             0                 //�ΰ汾��

#define  PRE_CHARGE_TIME                     500       // ����Ԥ���̵����պ�ʱ��ms


//========================== application image Id ============================
#define _IMG_ID                        4
#define _IMG_APPHDR_ADDR               0x2000
    
#define CANTEST


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

