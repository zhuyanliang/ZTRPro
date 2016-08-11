//-------------------------------------------------------------------------------
// taskcan.h head file, 
//-------------------------------------------------------------------------------

#ifndef __TASKCAN_H
#define __TASKCAN_H

#ifdef __cplusplus
   extern "C" {
#endif

#include <pic18.h>
#include "common.h"


//=============================================================================================
// ����CAN�����ϸ��ڵ�ĵ�ַ
#define  BMS                                    0xF4 
#define  GUI                                    0xFD
#define  MCS                                    0xEF
#define  TDU                                    0xE6
#define  CCS                                    0xE5
#define  BCA                                    0x50

#define CAN_ID_INPUT                            PORTDbits.RD4

#define CAN_MSG_BMS_TO_CHGR					    0x06
#define CAN_MSG_BMS_MCS_HNDSK					0x10
#define CAN_MSG_BMS_MCS_INFO					0x01

#define CAN_ID_CHGR_BRDC						0x18FF50E5

#define CAN_MSG_BMS_BCA_BATT_WARN                0x01
#define CAN_MSG_BMS_BCA_BATT_INFO                0x02
#define CAN_MSG_BMS_BCA_CELL_VOLT                0x03
#define CAN_MSG_BMS_BCA_CELL_TEMP                0x04  
 
#define CAN_MSG_BMS_BCA_VOLT_GRP1                0x10   
#define CAN_MSG_BMS_BCA_VOLT_GRP2                0x11
#define CAN_MSG_BMS_BCA_VOLT_GRP3                0x12
#define CAN_MSG_BMS_BCA_VOLT_GRP4                0x13
#define CAN_MSG_BMS_BCA_VOLT_GRP5                0x14

#define CAN_MSG_BMS_BCA_TEMP_GRR1                0x20


#define CAN_NEIGHBOR_PACK_TIMEOUT				((uint16_t)(5000 / 5))//((uint16_t)2500)
#define CAN_MCS_COMM_TIMEOUT                    ((uint16_t)(3000 / 5))
#define CAN_TDU_COMM_TIMEOUT                    ((uint16_t)(3000 / 5))
#define CAN_CCS_COMM_TIMEOUT                    ((uint16_t)(3000 / 5))
//=================================== CAN function code =======================================
// ���º궨�嶨���˸���CAN�ڵ�֮�����Ϣ֡������
// 1. BMS��GUI֮��ͨ�Ź�����
#define  CAN_MSG_BMS_VERSION                   ((uint8_t)0x00)
#define  CAN_MSG_BATTERY_STATE                 ((uint8_t)0x01)
#define  CAN_MSG_CELL_STATE                    ((uint8_t)0x02)
#define  CAN_MSG_CELLS_VOLTAGE_1               ((uint8_t)0x03)
#define  CAN_MSG_CELLS_VOLTAGE_2               ((uint8_t)0x04)
#define  CAN_MSG_CELLS_VOLTAGE_3               ((uint8_t)0x05)
#define  CAN_MSG_CELLS_VOLTAGE_4               ((uint8_t)0x06)
#define  CAN_MSG_CELLS_VOLTAGE_5               ((uint8_t)0x07)

#define  CAN_MSG_PACK_SOH                      ((uint8_t)0x0C)
#define  CAN_MSG_PACK_WARNING				   ((uint8_t)0x0D)
#define  CAN_MSG_BATTERY_TEMPERATURE           ((uint8_t)0x0E)


#define  CAN_GUI_CONFIG_COV_TH                 ((uint8_t)0x10)  
#define  CAN_GUI_CONFIG_CUV_TH                 ((uint8_t)0x11) 
#define  CAN_GUI_CONFIG_COT_TH                 ((uint8_t)0x12)   
#define  CAN_GUI_CONFIG_CUT_TH                 ((uint8_t)0x13) 
#define  CAN_GUI_CONFIG_DOT_TH                 ((uint8_t)0x14) 
#define  CAN_GUI_CONFIG_DUT_TH                 ((uint8_t)0x15) 
#define  CAN_GUI_CONFIG_COC_TH                 ((uint8_t)0x16) 
#define  CAN_GUI_CONFIG_DOC_TH                 ((uint8_t)0x17) 
#define  CAN_GUI_CONFIG_DLV_TH                 ((uint8_t)0x18)
#define  CAN_GUI_CONFIG_DLT_TH                 ((uint8_t)0x19)
#define  CAN_GUI_CONFIG_POV_TH                  0x1A
#define  CAN_GUI_CONFIG_PUV_TH                  0x1B
#define  CAN_GUI_CONFIG_ISO_TH                  0x1C

#define  CAN_MSG_PACK_PRA                      ((uint8_t)0x20)  // ����ϵͳ������pack����
#define  CAN_MSG_PRA_SET_COC                   ((uint8_t)0x22)  // �����������������
#define  CAN_MSG_PRA_SET_DOC                   ((uint8_t)0x23)  // ����ŵ������������
#define  CAN_MSG_PRA_SET_COT                   ((uint8_t)0x24)  // ��������²�������
#define  CAN_MSG_PRA_SET_DOT                   ((uint8_t)0x25)  // ����ŵ���²�������
#define  CAN_MSG_PRA_SET_CUT                   ((uint8_t)0x26)  // ��������²�������
#define  CAN_MSG_PRA_SET_DUT                   ((uint8_t)0x27)  // ����ŵ���²�������
#define  CAN_MSG_PRA_SET_OUC                   ((uint8_t)0x28)  // ����cell����Ƿѹ��������

#define  CAN_MSG_GUI_BUF_WRITE                 ((uint8_t)0x30)  // ����GUIдBUF����
#define  CAN_MSG_GUI_BUF_READ                  ((uint8_t)0x31)  // ����GUI��BUF����

#define  CAN_MSG_RUN_NORMAL_REC                ((uint8_t)0x40)  // �����������в�����¼
#define  CAN_MSG_FAULT_REC_OC                  ((uint8_t)0x41)  // ����������ϼ�¼
#define  CAN_MSG_FAULT_REC_OUC                 ((uint8_t)0x42)  // ����cell����Ƿѹ���ϼ�¼
#define  CAN_MSG_FAULT_REC_CT                  ((uint8_t)0x43)  // ������ߵ��¹��ϼ�¼
#define  CAN_MSG_FAULT_REC_DT                  ((uint8_t)0x44)  // ����ŵ�ߵ��¹��ϼ�¼
#define  CAN_MSG_FAULT_REC_MOS                 ((uint8_t)0x45)  // ����MOS�ܸ��¹��ϼ�¼
#define  CAN_MSG_FAULT_REC_HARD                ((uint8_t)0x46)  // ����ϵͳӲ�����ϼ�¼

#define  CAN_MSG_IMAGE_ERASE                   ((uint8_t)0xF0)
#define  CAN_MSG_IMAGE_REQUEST                 ((uint8_t)0xF1)
#define  CAN_MSG_IMAGE_LOAD                    ((uint8_t)0xF2)


// 2. BMS��DRIVE֮��ͨ�Ź�����




// 3. BMS��CHARGER֮��ͨ�Ź�����

#define CAN_MSG_BRDCST_INFO                 0xFF

// 4. BMS��TDU(�¶�ģ��)֮��ͨ�Ź�����
#define CAN_MSG_TEMP_GNR_INFO               0
#define CAN_MSG_TEMP_MODULE_1               0x01
#define CAN_MSG_TEMP_MODULE_2               0x02
#define CAN_MSG_TEMP_MODULE_3               0x03
#define CAN_MSG_TEMP_MODULE_4               0x04
#define CAN_MSG_TEMP_MODULE_5               0x05
#define CAN_MSG_TEMP_MODULE_6               0x06
#define CAN_MSG_TEMP_MODULE_7               0x07


//=================================== struct definition =======================================
#define  CAN_BUF_DEEP                    ((uint8_t)10)  //CAN���������

typedef struct
{
   CanMsgTypedef TxBuf[CAN_BUF_DEEP];    //define CAN message transmission buffer
   CanMsgTypedef RxBuf[CAN_BUF_DEEP];    //define CAN message receive buffer

   uint8_t TxBuf_Wptr;                   //define CAN message transmission buffer pointer
   uint8_t TxBuf_Rptr;                   //define CAN message transmission buffer pointer
   uint8_t RxBuf_Wptr;                   //define CAN message receive buffer pointer
   uint8_t RxBuf_Rptr;                   //define CAN message receive buffer pointer
}CanMsgBufTypedef;


//================================== function prototypes ======================================
void TskCan_Init(void);
uint32_t CAN_Generate_ID(uint8_t des_addr, uint8_t msg_fc);
BOOL CAN_IsTxBufFull(void);
void CAN_BatteryStateToTxBuf(void);
void CAN_BatteryStateToTxBuf(void);
void CAN_PutBattWarnToTxBuf(void);
void CAN_PutBattInfoToBuf(void);
void CAN_PutCellVoltPosToTxBuf(void);
void CAN_PutCellTempPosToTxBuf(void); 
void CAN_CellVoltage1ToTxBuf(void);
void CAN_CellVoltage2ToTxBuf(void);
void CAN_CellVoltage3ToTxBuf(void);
void CAN_CellVoltage4ToTxBuf(void);
void CAN_CellVoltage5ToTxBuf(void);
void CAN_CellTempToTxBuf(void);
void CAN_SysVerToTxBuf(void);
//void CAN_SendMsg_HeartBeat(void);
void CAN_AutoTxTask_Update(void);
void TskCanSendTxBufMsg(void);
void TskCanRecMsgToBuf(void);
void CAN_InterStart(void);
void CAN_NbrTskUpdate(void);
void TskCanProcessRxMsg(void);
void CAN_ChargerTskUpdate(void);
void CAN_McsTskUpdate(void);
void CAN_McsTimeoutCheck(void);
void CAN_BroadcastBufUpdate(void);
void CAN_ChargerTimeoutCheck(void);
void CAN_TmpBoardTimeoutCheck(void);
//=============================================================================================


#ifdef __cplusplus
}
#endif
#endif /*__TASKCAN_H */