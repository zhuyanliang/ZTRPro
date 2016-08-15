//============================================================================
//
//              Copyright (c) 2013, Globetools. All rights reserved.
//
//
// FileName    ��TaskCan.c
// Description ��BMS���ⲿ�豸������λ����driver�ȣ���ͨ�Ŵ��� 
// Versions    ��V1.0
// Author      ��
// Date        ��
// History     ��
// Comment     ��
//============================================================================

#include "include.h"

static CanMsgBufTypedef g_CanMsgBuf; 

static uint8_t 	canIdOffset = 0;	// ʶ�������ذ�
static uint8_t 	nbrCanId = 0;		// ͬ���ذ�ID
static uint8_t 	tduCanId = 0;		// �¶�ģ��CAN ID
static uint16_t canMcsTimeout = 0;	// MCS �������ϵͳ
static uint16_t canCcsTimeout = 0;	// CCS ���������ϵͳ 
static uint16_t canTduTimeout = 0;	// TDU �¶�ģ��
static uint16_t brdTxTimer = 0;		// �㲥ʱ�����

//----------------------------------------------------------------------------
// Function    : TskCan_Init
// Description : �ϲ� CAN ������ʼ��
// Parameters  : none
// Returns     : none
//----------------------------------------------------------------------------
void TskCan_Init(void)
{
	g_CanMsgBuf.TxBuf_Wptr = 0;
	g_CanMsgBuf.TxBuf_Rptr = 0;
	g_CanMsgBuf.RxBuf_Wptr = 0;
	g_CanMsgBuf.RxBuf_Rptr = 0;

	if (!CAN_ID_INPUT)
	{
		canIdOffset = 0;
		tduCanId = TDU;
		nbrCanId = BMS + 1;
	}
	else 
	{
		canIdOffset = 1; 
		nbrCanId = BMS;
		tduCanId = TDU+1;
	} 
	brdTxTimer = 0; 
	canMcsTimeout = 0;
	canCcsTimeout = 0;
	canTduTimeout = 0;
}



//----------------------------------------------------------------------------
// Function    ��CAN_ExtractSourceAddress
// Description ����CAN ID����ȡ���ͽڵ��Դ��ַ.
// Parameters  ��can_id - can��ID
// Returns     ��Դ��ַ
//----------------------------------------------------------------------------
uint8_t CAN_GetSourceAddr(uint32_t can_id)
{
   return ((uint8_t)can_id);
}

//----------------------------------------------------------------------------
// Function    ��CAN_ExtractFunctionCode
// Description ����CAN ID����ȡ������.
// Parameters  ��can_id - can��ID
// Returns     ��������
//----------------------------------------------------------------------------
uint8_t CAN_GetFuncCode(uint32_t can_id)
{
	uint8_t temp;

	temp = (uint8_t)(can_id >> 16);
	return(temp);
}

//----------------------------------------------------------------------------
// Function    ��CAN_GenerateID
// Description ������Ŀ���ַ�͹��������CAN ID
// Parameters  ��
// Returns     ��
//----------------------------------------------------------------------------
uint32_t CAN_GenerateID(uint8_t des_addr, uint8_t msg_fc)
{
	uint32_t temp = 0;

	temp = ((uint32_t)0x18 << 24) + ((uint32_t)msg_fc << 16) + 
		((uint32_t)des_addr << 8) + BMS + canIdOffset;
	return(temp);
}

//----------------------------------------------------------------------------
// Function    : CAN_IsTxBufFull
// Description : �������CAN���ͻ������Ƿ���
// Parameters  : 
// Returns     : 1 -- ��������  0 -- ������δ��
//----------------------------------------------------------------------------
BOOL CAN_IsTxBufFull(void)
{
	if (((g_CanMsgBuf.TxBuf_Wptr == (CAN_BUF_DEEP - 1))&&(g_CanMsgBuf.TxBuf_Rptr == 0))
	|| (g_CanMsgBuf.TxBuf_Rptr == (g_CanMsgBuf.TxBuf_Wptr + 1)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------------------------------
// Function    : CAN_IsTxBufEmpty
// Description : ���CAN���ͻ������Ƿ�Ϊ��
// Parameters  : 
// Returns     : 1 -- ��������  0 -- �������ǿ�
//----------------------------------------------------------------------------
BOOL CAN_IsTxBufEmpty(void)
{
	if (g_CanMsgBuf.TxBuf_Rptr == g_CanMsgBuf.TxBuf_Wptr)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------------------------------
// Function    : CAN_IsRxBufFull
// Description : �������CAN ���ջ������Ƿ��� 
// Parameters  : 
// Returns     : 1 -- ��   0 -- δ��
//----------------------------------------------------------------------------
BOOL CAN_IsRxBufFull(void)
{
	if (((g_CanMsgBuf.RxBuf_Wptr == (CAN_BUF_DEEP - 1))&&(g_CanMsgBuf.RxBuf_Rptr == 0))
		|| (g_CanMsgBuf.RxBuf_Rptr == (g_CanMsgBuf.RxBuf_Wptr + 1)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------------------------------
// Function    : CAN_IsRxBufEmpty
// Description : �������CAN ���ջ������Ƿ��
// Parameters  : 
// Returns     : 1 -- ��������  0 -- �������ǿ�
//----------------------------------------------------------------------------
BOOL CAN_IsRxBufEmpty(void)
{
	if (g_CanMsgBuf.RxBuf_Rptr == g_CanMsgBuf.RxBuf_Wptr)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/* 
 * ����ذ��ľ�����Ϣ���뷢�ͻ�����
 * 
 */
void CAN_PutBattWarnToTxBuf(void)
{
	if(CAN_IsTxBufFull())
		return;
		
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_BATT_WARN);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint32_t *)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_SystemWarning.all;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0;

	if ( g_SystemError.ltc_st || g_SystemError.ltc_com || g_SystemError.det_oc)
	{
		g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] |= 0x01;
	}
	if (g_SystemError.mcs_comm) 
	{
		g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] |= (0x01 << 1);
	}
	if (g_SystemError.ccs_comm)
	{
		g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] |= (0x01 << 2);
	}
	if (g_SystemError.tdu_comm)
	{
		g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] |= (0x01 << 3);
	}
	if (g_SystemError.nbr_comm)
	{
		g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] |= (0x01 << 4);
	}
	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}	

/*
 * ����ذ��ĵ�ѹ��������SOC�����̵�����Ϣ���뷢�ͻ�����
 * 
 */
void CAN_PutBattInfoToBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_BATT_INFO);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_BatteryParameter.voltage;
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint16_t)g_BatteryParameter.current;
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_BatteryParameter.SOC;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = g_RelayActFlg.positive;
	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

/*
 * ����о�����С��ѹֵ��λ�÷��뷢�ͻ�����
 */
void CAN_PutCellVoltPosToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_CELL_VOLT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_BatteryParameter.CellVoltMax;
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_BatteryParameter.CellVoltMin;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_BatteryParameter.MaxCellNum+1;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = g_BatteryParameter.MinCellNum+1;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}	

/*
 * ����о�����С�¶�ֵ��λ�÷��뷢�ͻ�����
 */
void CAN_PutCellTempPosToTxBuf(void)
{
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_CELL_TEMP);
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_BatteryParameter.CellTempMax;
    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = g_BatteryParameter.CellTempMin;

    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 0;//g_BatteryParameter.MaxTempModuIdx;
    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 0;//g_BatteryParameter.MaxTempChnIdx;

    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_BatteryParameter.MaxTempChnIdx;
    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = g_BatteryParameter.MinTempChnIdx;
    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

   if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
   {
      g_CanMsgBuf.TxBuf_Wptr = 0;
   }
}	


//----------------------------------------------------------------------------
// Function    : CAN_CellVoltage1ToTxBuf
// Description : �������о��ѹֵ 1~4 ���뷢�ͻ�����
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_CellVoltage1ToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_VOLT_GRP1);//CAN_GenerateID(GUI, CAN_MSG_CELLS_VOLTAGE_1);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_ArrayLtc6803Unit[0].CellVolt[0];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_ArrayLtc6803Unit[0].CellVolt[1];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_ArrayLtc6803Unit[0].CellVolt[2];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = g_ArrayLtc6803Unit[0].CellVolt[3];

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//----------------------------------------------------------------------------
// Function    : CAN_CellVoltage2ToTxBuf
// Description :  �������о��ѹֵ 5~8 ���뷢�ͻ�����
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_CellVoltage2ToTxBuf(void)
{
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_VOLT_GRP2);//CAN_GenerateID(GUI, CAN_MSG_CELLS_VOLTAGE_2);
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_ArrayLtc6803Unit[0].CellVolt[4];
   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_ArrayLtc6803Unit[0].CellVolt[5];
   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_ArrayLtc6803Unit[0].CellVolt[6];
   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = g_ArrayLtc6803Unit[0].CellVolt[7];

   if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
   {
      g_CanMsgBuf.TxBuf_Wptr = 0;
   }
}

//----------------------------------------------------------------------------
// Function    : CAN_CellVoltage3ToTxBuf
// Description :  �������о��ѹֵ 9~12 ���뷢�ͻ�����
// Parameters  : none
// Returns     : none
//----------------------------------------------------------------------------
void CAN_CellVoltage3ToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_VOLT_GRP3);//CAN_GenerateID(GUI, CAN_MSG_CELLS_VOLTAGE_3);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_ArrayLtc6803Unit[0].CellVolt[8];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_ArrayLtc6803Unit[0].CellVolt[9];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_ArrayLtc6803Unit[1].CellVolt[0];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = g_ArrayLtc6803Unit[1].CellVolt[1];

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//----------------------------------------------------------------------------
// Function    : CAN_CellVoltage4ToTxBuf
// Description :  �������о��ѹֵ 13~16 ���뷢�ͻ�����
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_CellVoltage4ToTxBuf(void)
{
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_VOLT_GRP4);//CAN_GenerateID(GUI, CAN_MSG_CELLS_VOLTAGE_4);
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_ArrayLtc6803Unit[1].CellVolt[2];
   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_ArrayLtc6803Unit[1].CellVolt[3];
   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_ArrayLtc6803Unit[1].CellVolt[4];
   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = g_ArrayLtc6803Unit[1].CellVolt[5];
	
   if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
   {
      g_CanMsgBuf.TxBuf_Wptr = 0;
   }
}

//----------------------------------------------------------------------------
// Function    : CAN_CellVoltage5ToTxBuf
// Description :  �������о��ѹֵ 17~20 ���뷢�ͻ�����
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_CellVoltage5ToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_VOLT_GRP5);//CAN_GenerateID(GUI, CAN_MSG_CELLS_VOLTAGE_5);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_ArrayLtc6803Unit[1].CellVolt[6];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_ArrayLtc6803Unit[1].CellVolt[7];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_ArrayLtc6803Unit[1].CellVolt[8];
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = g_ArrayLtc6803Unit[1].CellVolt[9];

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//============================================================================
// Function    : CAN_CellTempToTxBuf
// Description : ����о�¶�ֵ���뷢�ͻ�����
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_CellTempToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(BCA,CAN_MSG_BMS_BCA_TEMP_GRR1);//CAN_GenerateID(GUI, CAN_MSG_BATTERY_TEMPERATURE);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_BatteryParameter.CellTemp[0];
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)g_BatteryParameter.CellTemp[1];
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)g_BatteryParameter.CellTemp[2];
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)g_BatteryParameter.CellTemp[3];
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0;


	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//----------------------------------------------------------------------------
// Function    : CAN_BatteryStateToTxBuf
// Description : ��ذ�������Ϣ���뷢�ͻ�����
// Parameters  : 
// Returns     : 
//----------------------------------------------------------------------------
void CAN_BatteryStateToTxBuf(void)
{
	static uint8_t WatchDog = 0;	// ���ڶ�֡�ж�

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_BATTERY_STATE);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_BatteryParameter.voltage;// Iso_GetDltAdcRaw();
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint16_t)g_BatteryParameter.current;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_BatteryParameter.SOC;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = WatchDog++;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//----------------------------------------------------------------------------
// Function    : CAN_CellStateToTxBuf
// Description : ����о����״̬��Ϣ���뷢�ͻ�����
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_CellStateToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_CELL_STATE);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_BatteryParameter.CellVoltMax;
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_BatteryParameter.CellVoltMin;
	            g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)g_BatteryParameter.CellTempMax;
	            g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = (uint8_t)g_BatteryParameter.CellTempMin;
	            g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = g_BatteryParameter.MaxCellNum;
	            g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = g_BatteryParameter.MinCellNum;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


//----------------------------------------------------------------------------
// Function    : CAN_PackWarningToTxBuf
// Description : ����ذ�������Ϣ���뷢�ͻ�����
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_PackWarningToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PACK_WARNING);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint32_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_SystemWarning.all;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}	
}



//============================================================================
// Function    : CAN_PackSohToTxBuf
// Description : ��pack��SOH��ز����ŵ����ͻ�����
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_PackSohToTxBuf(void)
{
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PACK_SOH);
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
   g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_BatteryParameter.sohCycleTimes;
   *(uint32_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_BatteryParameter.ChargedAh;
   *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0;

   if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
   {
      g_CanMsgBuf.TxBuf_Wptr = 0;
   }
}

//============================================================================
// Function    : CAN_SysVerToTxBuf
// Description : ��ϵͳ�����Ӳ���İ汾�ŷ��뷢�ͻ�����
// Parameters  : none
// Returns     : none
//----------------------------------------------------------------------------
void CAN_SysVerToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_BMS_VERSION);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x05;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)_HARDWARE_MAJOR_VERSION;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)_HARDWARE_MINOR_VERSION;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)_FIRMWARE_MAJOR_VERSION;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)_FIRMWARE_MINOR_VERSION;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)_FIRMWARE_REVISE_VERSION;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


// -------------------------------------------------------------------
// ������ GUI ����Զ��֡Ҫ���������
// ���Ƚ����ݷ��뷢�ͻ�����
//---------------------------------------------------------------------



//============================================================================
// Function    : CAN_SendMsg_PackPra
// Description : ����Pack��Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadPackPra(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PACK_PRA);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)CELL_SERIES_NUM;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)CELL_PARALLEL_NUM;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)BATTERY_CAPACITY_RATED;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)(BATTERY_CAPACITY_RATED >> 8U);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


//============================================================================
// Function    : CAN_GUI_ReadSetCOC
// Description : ���ͳ�����������Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadSetCOC(void)
{
	uint16_t temp;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PRA_SET_COC);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	temp = 0xffff - PACK_COC_WARNING + 1;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)temp;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(temp >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = PACK_COC_WARNING_DLY;

	temp = 0xffff - PACK_COC_FAULT + 1; 
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)temp;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)(temp >> 8U);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = PACK_COC_FAULT_DLY;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//============================================================================
// Function    : CAN_GUI_ReadSetDOC
// Description : ���ͷŵ����������Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadSetDOC(void)
{
	uint16_t temp;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PRA_SET_DOC);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)PACK_DOC_WARNING;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(PACK_DOC_WARNING >> 8U);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = PACK_DOC_WARNING_DLY;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)PACK_DOC_FAULT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)(PACK_DOC_FAULT >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = PACK_DOC_FAULT_DLY;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}	
}


//============================================================================
// Function    : CAN_GUI_ReadSetCOT
// Description : ���ͳ����±�����Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadSetCOT(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PRA_SET_COT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)CELL_COT_FAULT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(CELL_COT_FAULT >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)CELL_COT_RECOVER;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)(CELL_COT_RECOVER >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//============================================================================
// Function    : CAN_GUI_ReadSetDOT
// Description : ���ͷŵ���±�����Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadSetDOT(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PRA_SET_DOT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)CELL_DOT_FAULT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(CELL_DOT_FAULT >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)CELL_DOT_RECOVER;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)(CELL_DOT_RECOVER >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


//============================================================================
// Function    : CAN_GUI_ReadSetCUT
// Description : ���ͳ����±�����Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadSetCUT(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PRA_SET_CUT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)CELL_CUT_FAULT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(CELL_CUT_FAULT >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)CELL_CUT_RECOVER;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)(CELL_CUT_RECOVER >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


//============================================================================
// Function    : CAN_GUI_ReadSetDUT
// Description : ���ͷŵ���±�����Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadSetDUT(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PRA_SET_DUT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (int8_t)((int16_t)CELL_DUT_FAULT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (int8_t)((int16_t)CELL_DUT_FAULT >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (int8_t)((int16_t)CELL_DUT_RECOVER);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (int8_t)((int16_t)CELL_DUT_RECOVER >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//============================================================================
// Function    : CAN_GUI_ReadSetOUC
// Description : ���͸ߵ�ѹ������Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadSetOUC(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PRA_SET_OUC);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)CELL_OV_FAULT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(CELL_OV_FAULT >> 8U);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)CELL_UV_FAULT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)(CELL_UV_FAULT >> 8U);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}	
}

// �� GUI Ҫ��ȡ��soh��ѭ����������������δ�����Ϣ���뷢�ͻ�����
//
void CAN_GUI_ReadNormalRec(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_RUN_NORMAL_REC);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_BatteryParameter.sohCycleTimes;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_BatteryParameter.sohCycleTimes >> 8U);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_FaultRecentRec.code_0;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = g_FaultRecentRec.code_1;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_FaultRecentRec.code_2;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}	
}


// �� GUI Ҫ��ȡ�ĳ�ŵ����������Ϣ���뷢�ͻ�����
void CAN_GUI_ReadFaultOcRec(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_FAULT_REC_OC);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_FaultRecord.coc;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_FaultRecord.coc >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 0xFF;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 0xff;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)g_FaultRecord.doc;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = (uint8_t)(g_FaultRecord.doc >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


// �� GUI Ҫ��ȡ�ĳ���ѹ������Ϣ���뷢�ͻ�����
void CAN_GUI_ReadFaultRecOuc(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_FAULT_REC_OUC);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_FaultRecord.cov;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_FaultRecord.cov >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 0xFF;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 0xff;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)g_FaultRecord.cuv;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = (uint8_t)(g_FaultRecord.cuv >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


// �� GUI Ҫ��ȡ�ĳ���¶ȴ�����Ϣ���뷢�ͻ�����
void CAN_GUI_ReadFaultRecCt(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_FAULT_REC_CT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_FaultRecord.cot;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_FaultRecord.cot >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 0xFF;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 0xff;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)g_FaultRecord.cut;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = (uint8_t)(g_FaultRecord.cut >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


// �� GUI Ҫ��ȡ�ķŵ��¶ȴ�����Ϣ���뷢�ͻ�����
void CAN_GUI_ReadFaultRecDt(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_FAULT_REC_DT);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_FaultRecord.dot;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_FaultRecord.dot >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 0xFF;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 0xff;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = (uint8_t)g_FaultRecord.dut;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = (uint8_t)(g_FaultRecord.dut >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


// �� GUI Ҫ��ȡ��Mos������Ϣ���뷢�ͻ�����
void CAN_GUI_ReadFaultRecMos(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_FAULT_REC_MOS);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_FaultRecord.mos_ot;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_FaultRecord.mos_ot >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 0xFF;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 0xff;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


// �� GUI Ҫ��ȡ��ltc6803������Ϣ���뷢�ͻ�����
void CAN_GUI_ReadFaultRecHard(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_FAULT_REC_HARD);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_FaultRecord.ltc_com;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_FaultRecord.ltc_com >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 0xFF;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 0xff;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


//**************************************************************************//
//
//**************************************************************************//

//============================================================================
// Function    : CAN_GUI_WriteBufToE2PRom
// Description : ���������е�����д��eeprom��  Ŀǰ�ݶ������ϵ�һЩ��Ϣ
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_WriteBufToE2PRom(uint8_t *ptr)
{
	uint8_t offset, len;

	if (*ptr > 127)
	{
		return;
	}
	else
	{
		offset = *ptr;
	}

	if ((*(ptr+1) == 0)
		|| (*(ptr+1) > 6))
	{
		return;
	}
	else
	{
		len = *(ptr+1);
	}

	EEPROM_WriteBlock((EEPROM_ADDR_FOR_GUI+offset), (ptr+2), len);
}

//============================================================================
// Function    : CAN_GUI_ReadBuf
// Description : ��eeprom�ж�ȡ���ݷ��뷢�ͻ�����
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ReadBuf(uint8_t *ptr)
{
	uint8_t offset, len;

	if (*ptr > 127)
	{
		return;
	}
	else
	{
		offset = *ptr;
	}

	if ((*(ptr+1) == 0)
	|| (*(ptr+1) > 6))
	{
		return;
	}
	else
	{
		len = *(ptr+1);
	}

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(GUI, CAN_MSG_PACK_SOH);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = offset;
	*(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = len;
	EEPROM_ReadBlock((EEPROM_ADDR_FOR_GUI+offset), &g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2], len);

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}


//============================================================================
// Function    : CAN_GUI_ConfigCovThr
// Description : ���õ�о����ѹ�ȼ�����ֵ����
// Parameters  : none
// Returns     : none
//============================================================================
void CAN_GUI_ConfigCovThr(uint8_t *ptrData)
{
	g_CellOVThr.cls_1 = *(uint16_t *)ptrData;
	g_CellOVThr.cls_2 = *(uint16_t *)(ptrData+2);    
	g_CellOVThr.crc   = calculate_crc8(ptrData, 4);
	EEPROM_WriteBlock(EEPROM_ADDR_COV_THRHOLD, (uint8_t *)&g_CellOVThr.cls_1, 5);
}

// ���õ�о����ѹ�ȼ�����ֵ����
void CAN_GUI_ConfigCuvThr(uint8_t *ptrData)
{
    g_CellUVThr.cls_1 = *(uint16_t *)ptrData;
    g_CellUVThr.cls_2 = *(uint16_t *)(ptrData+2);
    g_CellUVThr.crc = calculate_crc8(ptrData, 4);
    EEPROM_WriteBlock(EEPROM_ADDR_CUV_THRHOLD, (uint8_t *)&g_CellUVThr.cls_1, 5);
}

// ���õ�о�����µȼ�����ֵ����
void CAN_GUI_ConfigCotThr(uint8_t *ptrData)
{
	g_CellCOTThr.cls_1 = *(uint8_t *)ptrData;
	g_CellCOTThr.cls_2 = *(uint8_t *)(ptrData+1);

	g_CellCOTThr.crc = calculate_crc8(ptrData, 2);
	EEPROM_WriteBlock(EEPROM_ADDR_COT_THRHOLD, (uint8_t *)&g_CellCOTThr.cls_1, 3);
}

// ���õ�о�����µȼ�����ֵ����
void CAN_GUI_ConfigCutThr(uint8_t *ptrData)
{
	g_CellCUTThr.cls_1 = *(uint8_t *)ptrData;
	g_CellCUTThr.cls_2 = *(uint8_t *)(ptrData+1);
	g_CellCUTThr.crc = calculate_crc8(ptrData, 2);
	EEPROM_WriteBlock(EEPROM_ADDR_CUT_THRHOLD, (uint8_t *)&g_CellCUTThr.cls_1, 3);
}

// ���õ�о�ŵ���µȼ�����ֵ����
void CAN_GUI_ConfigDotThr(uint8_t *ptrData)
{
    g_CellDOTThr.cls_1 = *(uint8_t *)ptrData;
    g_CellDOTThr.cls_2 = *(uint8_t *)(ptrData+1);
    g_CellDOTThr.crc = calculate_crc8(ptrData, 2);
    EEPROM_WriteBlock(EEPROM_ADDR_DOT_THRHOLD, (uint8_t *)&g_CellDOTThr.cls_1, 3);
}

// ���÷ŵ���µȼ�����ֵ����
void CAN_GUI_ConfigDutThr(uint8_t *ptrData)
{
	g_CellDUTThr.cls_1 = *(uint8_t *)ptrData;
	g_CellDUTThr.cls_2 = *(uint8_t *)(ptrData+1);
	g_CellDUTThr.crc = calculate_crc8(ptrData, 2);
	EEPROM_WriteBlock(EEPROM_ADDR_DUT_THRHOLD, (uint8_t *)&g_CellDUTThr.cls_1, 3);
}

// ���õ�ذ��������ȼ�ֵ����
void CAN_GUI_ConfigCocThr(uint8_t *ptrData)
{
    g_BattCOCThr.cls_1 = *(uint16_t *)ptrData;
    g_BattCOCThr.cls_2 = *(uint16_t *)(ptrData+2);
    g_BattCOCThr.crc = calculate_crc8(ptrData, 4);
    EEPROM_WriteBlock(EEPROM_ADDR_COC_THRHOLD, (uint8_t *)&g_BattCOCThr.cls_1, 5);
}

// ���õ�ذ��ŵ�����ȼ�ֵ����
void CAN_GUI_ConfigDocThr(uint8_t *ptrData)
{
    g_BattDOCThr.cls_1 = *(uint16_t *)ptrData;
    g_BattDOCThr.cls_2 = *(uint16_t *)(ptrData+2);
    g_BattDOCThr.crc = calculate_crc8(ptrData, 4);
    EEPROM_WriteBlock(EEPROM_ADDR_DOC_THRHOLD, (uint8_t *)&g_BattDOCThr.cls_1, 5);
}

// ���õ���һ���Եȼ�ֵ����
void CAN_GUI_ConfigDlvThr(uint8_t *ptrData)
{
    g_CellIBThr.cls_1 = *(uint16_t *)ptrData;
    g_CellIBThr.cls_2 = *(uint16_t *)(ptrData+2);
    g_CellIBThr.crc = calculate_crc8(ptrData, 4);
    EEPROM_WriteBlock(EEPROM_ADDR_DLV_THRHOLD, (uint8_t *)&g_CellIBThr.cls_1, 5);
}

// ���õ���²�澯�ȼ�ֵ
void CAN_GUI_ConfigDltThr(uint8_t *ptrData)
{
    g_CellDLTThr.cls_1 = *(uint8_t *)ptrData;
    g_CellDLTThr.cls_2 = *(uint8_t *)(ptrData+1);
    g_CellDLTThr.crc = calculate_crc8(ptrData, 2);
    EEPROM_WriteBlock(EEPROM_ADDR_DLT_THRHOLD, (uint8_t *)&g_CellDLTThr.cls_1, 3);
}


// ���õ�ذ���ѹ�ȼ�����  
void CAN_GUI_ConfigPovThr(uint8_t *ptrData)
{
    g_PackOVThr.cls_1 = *(uint16_t *)ptrData;
    g_PackOVThr.cls_2 = *(uint16_t *)(ptrData+2);
    
    g_PackOVThr.crc = calculate_crc8(ptrData, 4);
    EEPROM_WriteBlock(EEPROM_ADDR_POV_THRHOLD, (uint8_t *)&g_PackOVThr.cls_1, 5);
}


// ���õ�ذ���ѹ�ȼ�����  
void CAN_GUI_ConfigPuvThr(uint8_t *ptrData)
{
    g_PackUVThr.cls_1 = *(uint16_t *)ptrData;
    g_PackUVThr.cls_2 = *(uint16_t *)(ptrData+2);
    
    g_PackUVThr.crc = calculate_crc8(ptrData, 4);
    EEPROM_WriteBlock(EEPROM_ADDR_PUV_THRHOLD, (uint8_t *)&g_PackUVThr.cls_1, 5);
}

// ���þ�Ե�Բ��� ����Ŀ��ʱδʹ��
#if 0
void CAN_GUI_ConfigIsoThr(uint8_t *ptrData)
{
    IsoThr.cls_1 = *(uint16_t *)ptrData;
    IsoThr.cls_2 = *(uint16_t *)(ptrData+2);
    
    IsoThr.crc = calculate_crc8(ptrData, 4);
    EEPROM_WriteBlock(EEPROM_ADDR_ISO_THRHOLD, (uint8_t *)&IsoThr.cls_1, 5);
}
#endif

//============================================================================
// Function    : CAN_ClearImage
// Description : ��λ��GUI��Ƭ�������û�appʱ���͵��������flash��apphead��
//               ��Ȼ��reset������bootloader,����GUI���͵��µ��û�app image��
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void CAN_ClearImage(uint8_t *can_msg)
{
    LedRed = 1;
    LedGre = 1;
	if (can_msg[0] == 0x01 
		&& can_msg[1] == 0x23               
		&& can_msg[2] == 0x45 
		&& can_msg[3] == 0x67 
		&& can_msg[4] == 0x89 
		&& can_msg[5] == 0xAB 
		&& can_msg[6] == 0xCD 
		&& can_msg[7] == 0xEF)  // 8���ֽڵ����룬��Ҫ���������
	{
		INTCON = 0x00;                            // Disable all interrupts
		FLASH_Erase64Bytes( _IMG_APPHDR_ADDR );     // Invalidate AppImage
		Reset();                                 
	}
}


#if 0
void CAN_PaserNeighborPack(uint8_t *can_msg)
{
	NeighborPack.voltage = ((uint16_t)can_msg[1] << 8) | can_msg[0];
	NeighborPack.min_cell = ((uint16_t)can_msg[3] << 8) | can_msg[2];

	NeighborPack.soc = can_msg[4];
	NeighborPack.st_relay = can_msg[5];
//	NeighborPack.handshk  = 1;
	NeighborPack.timer	 = 0;
	g_SystemError.nbr_comm = 0;	
}
#endif

/*
 * ��ذ�֮�������Ϣ�Ĵ���
 */
void CAN_SendInterFrame(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(nbrCanId,0);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)g_BatteryParameter.voltage;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)(g_BatteryParameter.voltage >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)g_BatteryParameter.CellVoltMin;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)(g_BatteryParameter.CellVoltMin >> 8);

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_BatteryParameter.SOC;


	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = g_RelayActFlg.negative;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

//  �������������Ϣ
void CAN_PaserChargerMsg(void)
{
	g_SystemError.ccs_comm = 0;	
	canCcsTimeout = 1;    
}

// ��Ҫ���͸���������Ϣ���뷢�ͻ�����
void CAN_CharggerMsgToTxBuf(void)
{
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(CCS,CAN_MSG_BMS_TO_CHGR);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = (uint8_t)(MAX_CHARGER_OUTPUT_VOLT >> 8);
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = (uint8_t)MAX_CHARGER_OUTPUT_VOLT;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = (uint8_t)(MAX_CHARGER_OUTPUT_CURR >> 8);;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = (uint8_t)MAX_CHARGER_OUTPUT_CURR;

	if (g_BatteryMode == PROTECTION)
	{
		g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 1;
	}
	else
	{
		g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = 0;
	}

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

	if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
	{
		g_CanMsgBuf.TxBuf_Wptr = 0;
	}
}

// ������������ϵͳ��������Ϣ���뷢�ͻ�����
void CAN_SendMcsHandshakeFrame(void)
{
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(MCS,CAN_MSG_BMS_MCS_HNDSK);
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = 'G';
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[1] = 'l';
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = 'o';
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[3] = 'b';
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = 'e';
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = 0xff;
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;

   if (++g_CanMsgBuf.TxBuf_Wptr >= CAN_BUF_DEEP)
   {
      g_CanMsgBuf.TxBuf_Wptr = 0;
   }	
}

// ���������������ϵͳ����Ϣ���뷢�ͻ�����
void CAN_SendMcsBattInfoFrame(void)
{
	uint8_t warn = 0;

   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].COB_ID = CAN_GenerateID(MCS,CAN_MSG_BMS_MCS_INFO);
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].IDE = CAN_ID_EXT;
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].RTR = CAN_RTR_DATA;
   	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].DLC = 0x08;

    *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[0] = g_BatteryParameter.CellVoltMax;
    *(uint16_t*)&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[2] = g_BatteryParameter.CellVoltMin;
    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[4] = g_BatteryParameter.CellTempMax;
    g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[5] = g_BatteryParameter.CellTempMin;

	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[6] = Soc_GetSoc();
	g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Wptr].Data[7] = 0xff;
}

// ������������������Ϣ
void CAN_PaserMcsInfo(uint8_t *can_msg)
{
	if ( (can_msg[0]=='G') 
			&& (can_msg[1]=='l') 
			&& (can_msg[2]=='o') 
			&& (can_msg[3]=='b') 
			&& (can_msg[4]=='e') )
	{
		g_SystemError.mcs_comm = 0;	
		canMcsTimeout = 1;		
	}
}

// �����¶�ģ�鷢������Ϣ
void CAN_PaserTduGnrInfo(uint8_t *can_msg)
{
    
    g_BatteryParameter.CellTempMax = can_msg[0];
    g_BatteryParameter.CellTempMin = can_msg[1];
    g_BatteryParameter.CellTempAvg = can_msg[2];
    g_BatteryParameter.MaxTempModuIdx = can_msg[3]+1;
    g_BatteryParameter.MaxTempChnIdx = can_msg[4]+1;
    g_BatteryParameter.MinTempModuIdx = can_msg[5]+1;
    g_BatteryParameter.MinTempChnIdx = can_msg[6]+1;

    canTduTimeout = 0;
    g_SystemError.tdu_comm = 0;
}


// 
void CAN_InterStart(void)
{
	uint8_t i = 3;

	do{
	    CAN_SendInterFrame();
	}while(i--);

	TskCanSendTxBufMsg();
}


// ����Ҫ�㲥��Ϣ�ķ��ͻ�����
void CAN_BroadcastBufUpdate(void)
{ 
    switch (brdTxTimer++)
    {
        case 5:
            CAN_PutBattWarnToTxBuf();   
        break;
        case 10:
            CAN_PutBattInfoToBuf();
        break;
        case 15:
            CAN_PutCellVoltPosToTxBuf();       
        break;
        case 20:
            CAN_PutCellTempPosToTxBuf();          
        break;
        case 25:
            CAN_CellVoltage1ToTxBuf(); 
        break;
        case 30:
            CAN_CellVoltage2ToTxBuf();    
        break;
        case 35:
            CAN_CellVoltage3ToTxBuf();
        break;
        case 40:
            CAN_CellVoltage4ToTxBuf();
        break;
        case 45:   
            CAN_CellVoltage5ToTxBuf();
            break;  
        case 50:
            CAN_CellTempToTxBuf();  
            brdTxTimer = 0;          
        break;
       default: 
            if (brdTxTimer > 50)
            {
                brdTxTimer = 0;  
            }        
        break;
    }
}


// ��ذ�֮��ͨ�Ÿ��� 
void CAN_NbrTskUpdate(void)
{
	static uint8_t inter_can_timer = 0;

	if (inter_can_timer++ > 100)
	{
		inter_can_timer = 0;
		CAN_SendInterFrame();
	}
}

// �������Ϣ����
void CAN_ChargerTskUpdate(void)
{
	static uint8_t chgr_can_timer = 0;

	if (chgr_can_timer++ > 200)
	{
		chgr_can_timer = 0;
		CAN_CharggerMsgToTxBuf();	
	}			
}

// ����������ϵͳ��Ϣ����
void CAN_McsTskUpdate(void)
{
	static uint16_t mcs_can_timer = 0;

	if ( !(mcs_can_timer % 200) )
	{
		CAN_SendMcsHandshakeFrame();
	}

	mcs_can_timer++;	

	if (g_SystemError.mcs_comm) 
		return;



	if ( !(mcs_can_timer % 100) )
	{
		CAN_SendMcsBattInfoFrame();
	}

	if (mcs_can_timer > 200)
	{
		mcs_can_timer = 0;
	}
}


// ����������ͨ�ų�ʱ���
void CAN_McsTimeoutCheck(void)
{
	if (canMcsTimeout++ > CAN_MCS_COMM_TIMEOUT)
	{
		g_SystemError.mcs_comm = 1;	
		canMcsTimeout = 0;
		if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_1S)
		{
			g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;
		}        
	}
}


// ���¶�ģ��ͨ�ų�ʱ���
void CAN_TmpBoardTimeoutCheck(void)
{
	if (canTduTimeout++ > CAN_TDU_COMM_TIMEOUT)
	{
		g_SystemError.tdu_comm = 1;	
		canTduTimeout = 0;
	}
}

// ������ͨ�ų�ʱ���
void CAN_ChargerTimeoutCheck(void)
{
	if (canCcsTimeout++ > CAN_CCS_COMM_TIMEOUT)
	{
		g_SystemError.ccs_comm = 1;	
		canCcsTimeout = 0;
        if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_1S)
        {
            g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;
        }  
	}    
}


//============================================================================
// Function    ��ToggleTestPin
// Description ���л����ԹܽŸߵ͵�ƽ������������岨��
// Parameters  ��none 
// Returns     ��none
//============================================================================  
void ToggleCanSendTest(void)
{
	LATBbits.LATB6 ^= 1;
}


//----------------------------------------------------------------------------
// Function    ��CAN_SendTxBufMsg
// Description �������ͻ����������ݷ��ͳ�ȥ
//               
// Parameters  ��none
// Returns     ��none
//----------------------------------------------------------------------------
void TskCanSendTxBufMsg(void)
{
	/* ���������Ƿ��д����͵�֡ */
	if ( !CAN_IsTxBufEmpty())
	{
		if (ECAN_TransmitMsg(&g_CanMsgBuf.TxBuf[g_CanMsgBuf.TxBuf_Rptr]))
		{
			if (++g_CanMsgBuf.TxBuf_Rptr >= CAN_BUF_DEEP)
			{
				g_CanMsgBuf.TxBuf_Rptr = 0;
			}
#if SENDTEST
			ToggleCanSendTest();
#endif
			}
		else
		{
			//break;
		}
	}
}
//----------------------------------------------------------------------------
// Function    ��CAN_RecMsgToBuf
// Description �������ջ�����RX0�е�CAN֡������ջ������У����������Ľ���
// Parameters  ��none
// Returns     ��none
//----------------------------------------------------------------------------
void TskCanRecMsgToBuf(void)
{
	if (CAN_IsRxBufFull())
	{
		return;
	}

	if (ECAN_ReceiveMsg(&g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Wptr]))
	{
		if (++g_CanMsgBuf.RxBuf_Wptr >= CAN_BUF_DEEP)
		{
			g_CanMsgBuf.RxBuf_Wptr = 0;
		}
	}
}

//----------------------------------------------------------------------------
// Function    ��CAN_ProcessRxMsg
// Description ��������յ���CAN����֡
// Parameters  ��none
// Returns     ��none
//----------------------------------------------------------------------------
void TskCanProcessRxMsg(void)
{
	uint8_t can_sa, can_fc;

	while (!CAN_IsRxBufEmpty())
	{
		if (CAN_IsTxBufFull())  // �жϷ��ͻ������Ƿ�������ȷ���Ƿ��пռ����Ӧ֡���ͣ�
		{                      // ��û�пռ䷢����Ӧ֡�����ݲ�������յ�������֡��
			return;             
		}

		if( CAN_ID_EXT == g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].IDE)  // ��Ϣ֡Ϊ��չ֡����ʱֻ����GUIͨ�Ų�����չ֡��
		{
			can_sa = CAN_GetSourceAddr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].COB_ID);
			can_fc = CAN_GetFuncCode(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].COB_ID);

            if (tduCanId == can_sa) 
            {
                if(can_fc == CAN_MSG_TEMP_GNR_INFO)
					CAN_PaserTduGnrInfo(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);           
            }   
			else if(GUI == can_sa)
			{
				switch(can_fc)
				{
				case CAN_MSG_BATTERY_STATE:
					CAN_BatteryStateToTxBuf();
					break;

				case CAN_MSG_CELL_STATE:
					CAN_CellStateToTxBuf();
					break;
#if 0
				case CAN_MSG_CELLS_VOLTAGE_1:
					CAN_CellVoltage1ToTxBuf();
					break;

				case CAN_MSG_CELLS_VOLTAGE_2:
					CAN_CellVoltage2ToTxBuf();
					break;

				case CAN_MSG_CELLS_VOLTAGE_3:
					CAN_CellVoltage3ToTxBuf();
					break;

				case CAN_MSG_CELLS_VOLTAGE_4:
					CAN_CellVoltage4ToTxBuf();
					break;

				case CAN_MSG_CELLS_VOLTAGE_5:
					CAN_CellVoltage5ToTxBuf();
					break;
#endif
				case CAN_MSG_PACK_WARNING	:
					CAN_PackWarningToTxBuf();
					break;

				case CAN_MSG_BATTERY_TEMPERATURE:
					//CAN_CellTempToTxBuf();
					break;

				case CAN_MSG_PACK_SOH:
					CAN_PackSohToTxBuf();
					break;
				case CAN_MSG_BMS_VERSION:
					CAN_SysVerToTxBuf();
					break;

				// ����ΪGUI����������
				case CAN_MSG_PACK_PRA :
					CAN_GUI_ReadPackPra();
					break;

				case CAN_MSG_PRA_SET_COC :
					CAN_GUI_ReadSetCOC();
					break;
				case CAN_MSG_PRA_SET_DOC :
					CAN_GUI_ReadSetDOC();
					break;
				case CAN_MSG_PRA_SET_COT :
					CAN_GUI_ReadSetCOT();
					break;
				case CAN_MSG_PRA_SET_DOT :
					CAN_GUI_ReadSetDOT();	
					break;
				case CAN_MSG_PRA_SET_CUT :
					CAN_GUI_ReadSetCUT();
					break;
				case CAN_MSG_PRA_SET_DUT :
					CAN_GUI_ReadSetDUT();
					break;
				case CAN_MSG_PRA_SET_OUC :
					CAN_GUI_ReadSetOUC();
					break;

				// ����ΪGUI��д�ڲ�BUF����
				case CAN_MSG_GUI_BUF_WRITE:
					CAN_GUI_WriteBufToE2PRom(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_MSG_GUI_BUF_READ:
					CAN_GUI_ReadBuf(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;

				// ����ΪGUI�����ϼ�¼������
				case CAN_MSG_RUN_NORMAL_REC :
					CAN_GUI_ReadNormalRec();
					break;
				case CAN_MSG_FAULT_REC_OC :
					CAN_GUI_ReadFaultOcRec();
					break;
				case CAN_MSG_FAULT_REC_OUC :
					CAN_GUI_ReadFaultRecOuc();
					break;
				case CAN_MSG_FAULT_REC_CT :
					CAN_GUI_ReadFaultRecCt();
					break;
				case CAN_MSG_FAULT_REC_DT :
					CAN_GUI_ReadFaultRecDt();
					break;
				case CAN_MSG_FAULT_REC_MOS :
					CAN_GUI_ReadFaultRecMos();
					break;
				case CAN_MSG_FAULT_REC_HARD :
					CAN_GUI_ReadFaultRecHard();
					break;
				case CAN_GUI_CONFIG_COV_TH:
					CAN_GUI_ConfigCovThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_CUV_TH:
					CAN_GUI_ConfigCuvThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_COT_TH:
					CAN_GUI_ConfigCotThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_CUT_TH:
					CAN_GUI_ConfigCutThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_DOT_TH:
					CAN_GUI_ConfigDotThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_DUT_TH:
					CAN_GUI_ConfigDutThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_COC_TH:
					CAN_GUI_ConfigCocThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data); 
					break;
				case CAN_GUI_CONFIG_DOC_TH:
					CAN_GUI_ConfigDocThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_DLV_TH:
					CAN_GUI_ConfigDlvThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case CAN_GUI_CONFIG_DLT_TH:
					CAN_GUI_ConfigDltThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				case  CAN_GUI_CONFIG_POV_TH:
					CAN_GUI_ConfigPovThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;  
				case  CAN_GUI_CONFIG_PUV_TH:
					CAN_GUI_ConfigPuvThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;      
				case  CAN_GUI_CONFIG_ISO_TH:
					//CAN_GUI_ConfigIsoThr(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					//break;          
				// GUI���͵�FLASH�������bootloader��������
				case CAN_MSG_IMAGE_ERASE:
					CAN_ClearImage(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
					break;
				default:
					break;
				}

			}
			else if(CCS == can_sa)
			{
				if (can_fc == CAN_MSG_BRDCST_INFO)
					CAN_PaserChargerMsg();
			}
			else if(MCS == can_sa)
				CAN_PaserMcsInfo(g_CanMsgBuf.RxBuf[g_CanMsgBuf.RxBuf_Rptr].Data);
		}

		// readָ��ָ����һ�����������Ϣ
		if (++g_CanMsgBuf.RxBuf_Rptr >= CAN_BUF_DEEP)
		{
			g_CanMsgBuf.RxBuf_Rptr = 0;
		}

   }
}


