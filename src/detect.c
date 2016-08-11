
#include "include.h"

//---------------------------------------------------------
volatile SysErrDef		g_SystemError;			// ��¼ϵͳ������Ϣ
volatile SysWarningDef 	g_SystemWarning;		// ��¼ϵͳ������Ϣ


CurrWarnClsDef 	g_BattCOCThr;		// ��ذ�������
CurrWarnClsDef 	g_BattDOCThr;		// �ŵ����
VoltWarnClsDef 	g_BattOVThr;
VoltWarnClsDef 	g_BattUVThr;
VoltWarnClsDef 	g_PackOVThr;
VoltWarnClsDef 	g_PackUVThr;

//---------------------------------------------------------
VoltWarnClsDef 	g_CellOVThr;
VoltWarnClsDef 	g_CellUVThr;
VoltWarnClsDef 	g_CellIBThr;	// ����һ����

TempWarnClsDef 	g_CellCOTThr;	// ���������
TempWarnClsDef 	g_CellCUTThr;	// ���������
TempWarnClsDef 	g_CellDOTThr;	// ����ŵ����
TempWarnClsDef 	g_CellDUTThr;	// ����ŵ����
TempWarnClsDef 	g_CellDLTThr;	// ����²��澯

// ����Ƿ��ڳ������ز���
static uint8_t chgrChkTimer = 0;
static uint8_t keyChkTimer = 0;
static uint8_t keyChgrState = 0;
#define KEY_CHK_CYCLE       ((uint8_t)10)
#define CHGR_CHK_CYCLE      ((uint8_t)10)



//============================================================================
// Function    : DetectPackOverCurrent
// Description : ��ذ�������⣨�����ŵ�����ͳ�磩
// Parameters  : none
// Returns     : 
//============================================================================
void DetectPackOverCurrent(void)
{
	static uint16_t OcErrCnt = 0;

	// ������
	if (g_SystemWarning.COC == WARNING_SECOND_LEVEL)
	{
		return;
	}
	//�ŵ����
	if (g_SystemWarning.DOC == WARNING_SECOND_LEVEL)
	{
		return;
	}
	
	if (g_BatteryMode == CHARGE)
	{
		if (g_BatteryParameter.current < g_BattCOCThr.cls_2)
		{
			if (OcErrCnt > PACK_COC_FAULT_DLY)
			{
				g_SystemWarning.COC = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_5S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_5S;
				}
			}
			else
			{
				OcErrCnt++;
			}
		} 
		else if (g_BatteryParameter.current < g_BattCOCThr.cls_1)
		{
			if (OcErrCnt > PACK_COC_WARNING_DLY)
			{
				g_SystemWarning.COC = WARNING_FIRST_LEVEL;
				OcErrCnt = 0;
			}
			else
			{
				OcErrCnt++;
			}
		}
		else
		{
			g_SystemWarning.COC = 0;
			OcErrCnt = 0;
		}
	}
	else
	{
		if (g_BatteryParameter.current > g_BattCOCThr.cls_2)
		{
			if (OcErrCnt > PACK_DOC_FAULT_DLY)
			{
				g_SystemWarning.DOC = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				OcErrCnt++;
			}
		}
		else if (g_BatteryParameter.current > g_BattCOCThr.cls_1)
		{
			if (OcErrCnt > PACK_DOC_WARNING_DLY)
			{
				OcErrCnt = 0;
				g_SystemWarning.DOC = WARNING_FIRST_LEVEL;
			}
			else
			{
				OcErrCnt++;
			}
		}
		else
		{
			g_SystemWarning.DOC = 0;
			OcErrCnt = 0;
		}
	}
}


//============================================================================
// Function    : DetectMaxMinCellTemp
// Description : �ú�����������С�����¶�ֵ������ȡƽ���¶Ⱥ������С�¶�λ��
// Parameters  : none
// Returns     : 
//============================================================================
void DetectMaxMinCellTemp(void)
{
	uint8_t i;
	int8_t MaxTemp = -40;
	int8_t MinTemp = 125;
	int16_t AvgTemp = 0;
    

	for (i = 0; i < MAX_TEMP_SENSOR; i++)
	{
		if (MaxTemp < g_BatteryParameter.CellTemp[i])
		{
			MaxTemp = g_BatteryParameter.CellTemp[i];
			g_BatteryParameter.MaxTempChnIdx = i;
		}

		if (MinTemp > g_BatteryParameter.CellTemp[i])
		{
			MinTemp = g_BatteryParameter.CellTemp[i];
			g_BatteryParameter.MinTempChnIdx = i;   
		}
		AvgTemp += g_BatteryParameter.CellTemp[i];
	}

	g_BatteryParameter.CellTempMax = MaxTemp;
	g_BatteryParameter.CellTempMin = MinTemp;
	g_BatteryParameter.CellTempAvg = AvgTemp / MAX_TEMP_SENSOR;

	//�����������Ϊ�¶�ģ�鿪·
    if ( (g_BatteryParameter.CellTempMax >= 125) || (g_BatteryParameter.CellTempMax <= -40) )
    {
        g_SystemError.det_oc = 1;
    }
    else
    {
        g_SystemError.det_oc = 0;
    }
}

//============================================================================
// Function    : DetectCellsOverTemp
// Description : ����¶ȹ���
// Parameters  : none
// Returns     : 
//============================================================================
void DetectCellsOverTemp(void)
{
	static uint8_t OtErrCnt = 0;

	//�¶�ģ��ͨѶ����
	if ( g_SystemError.tdu_comm ) 
		return;

	if (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
	{
		if (g_BatteryParameter.CellTempMax < g_CellCOTThr.cls_2)
		{
			if (OtErrCnt > CELL_COT_FAULT_DLY)
			{
				OtErrCnt = 0;
				g_SystemWarning.COT = 0;
			}
			else
			{
				OtErrCnt++;
			}
		}
		return;
	}
	if (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
	{
		if (g_BatteryParameter.CellTempMax < g_CellDOTThr.cls_2)
		{   
			if (OtErrCnt > CELL_DOT_FAULT_DLY)
			{
				OtErrCnt = 0;
				g_SystemWarning.DOT = 0;
			}
			else
			{
				OtErrCnt++;
			}
		}
		return;
	}

	if (g_BatteryMode == CHARGE)
	{
		if (g_BatteryParameter.CellTempMax > g_CellCOTThr.cls_2)
		{
			if (OtErrCnt > CELL_COT_FAULT_DLY)
			{
				g_SystemWarning.COT = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				OtErrCnt++;
			}
		}
		else if (g_BatteryParameter.CellTempMax > g_CellCOTThr.cls_1)
		{
			if (OtErrCnt > CELL_COT_WARNING_DLY)
			{
				g_SystemWarning.COT = WARNING_FIRST_LEVEL;
				OtErrCnt = 0;
			}
			else
			{
				OtErrCnt++;
			}
		}
		else
		{
			OtErrCnt = 0;
			g_SystemWarning.COT = 0;
		}
	}
	else
	{
		if (g_BatteryParameter.CellTempMax > g_CellDOTThr.cls_2)
		{   
			if (OtErrCnt > CELL_DOT_FAULT_DLY)
			{
				g_SystemWarning.DOT = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				OtErrCnt++;
			}
		}
		else if (g_BatteryParameter.CellTempMax > g_CellDOTThr.cls_1)
		{
			if (OtErrCnt > CELL_DOT_WARNING_DLY)
			{
				g_SystemWarning.DOT = WARNING_FIRST_LEVEL;
				OtErrCnt = 0;
			}
			else
			{
				OtErrCnt++;
			}
		}
		else
		{
			OtErrCnt = 0;
			g_SystemWarning.DOT = 0;
		}
	}
}

//============================================================================
// Function    : DetectCellsUnderTemp
// Description : ����¶��Ƿ����
// Parameters  : none
// Returns     : 
//============================================================================
void DetectCellsUnderTemp(void)
{
	static uint8_t UtErrCnt = 0;

	if ( g_SystemError.tdu_comm ) 
		return;

	if (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
	{
		if (g_BatteryParameter.CellTempMin > g_CellCUTThr.cls_2)
		{       
			if (UtErrCnt > CELL_CUT_FAULT_DLY)
			{
				UtErrCnt = 0;
				g_SystemWarning.CUT = 0;
			}
			else
			{
				UtErrCnt++;
			}
		}
		return;
	}
   if (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
   {
      if (g_BatteryParameter.CellTempMin > g_CellDUTThr.cls_2)
      {
            if (UtErrCnt > CELL_DUT_FAULT_DLY)
            {
                UtErrCnt = 0;
                g_SystemWarning.DUT = 0;
            }
            else
            {
                UtErrCnt++;
            }
      }
      return;
   }

	if(g_BatteryMode == CHARGE)
	{
		if (g_BatteryParameter.CellTempMin < g_CellCUTThr.cls_2)
		{       
			if (UtErrCnt > CELL_CUT_FAULT_DLY)
			{
				g_SystemWarning.CUT = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				UtErrCnt++;
			}
		}
		else if (g_BatteryParameter.CellTempMin < g_CellCUTThr.cls_1)
		{
			if (UtErrCnt > CELL_CUT_WARNING_DLY)
			{
				UtErrCnt= 0;
				g_SystemWarning.CUT = WARNING_FIRST_LEVEL;
			}
			else
			{
				UtErrCnt++;
			}
		}
		else
		{
			UtErrCnt = 0;
			g_SystemWarning.CUT = 0;
		}
	}
	else
	{
		if (g_BatteryParameter.CellTempMin < g_CellDUTThr.cls_2)
		{

			if (UtErrCnt > CELL_DUT_FAULT_DLY)
			{
				g_SystemWarning.DUT = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				UtErrCnt++;
			}
		}
		else if (g_BatteryParameter.CellTempMin < g_CellDUTThr.cls_1)
		{
			if (UtErrCnt > CELL_DUT_WARNING_DLY)
			{
				UtErrCnt = 0;
				g_SystemWarning.DUT = WARNING_FIRST_LEVEL;
			}
			else
			{
				UtErrCnt++;
			}
		}
		else
		{
			UtErrCnt = 0;
			g_SystemWarning.DUT = 0;
		}
	}
}


//============================================================================
// Function    : DetectCellsUnderTemp
// Description : ����¶Ȳ�
// Parameters  : none
// Returns     : 
//============================================================================

void DetectCellTempDlt(void)
{  
	static uint8_t tImbErrCnt = 0;
	int8_t temp;

	temp = g_BatteryParameter.CellTempMax - g_BatteryParameter.CellTempAvg;
	if (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
	{
		if(temp < g_CellDLTThr.cls_2)
		{
			if (tImbErrCnt > CELL_TIB_FAULT_DLY)
			{
				tImbErrCnt = 0;
				g_SystemWarning.TIB = 0;
			}
			else
			{
				tImbErrCnt++;
			}
		}
		return;
	}

	if(temp > g_CellDLTThr.cls_2)
	{
		if (tImbErrCnt > CELL_TIB_FAULT_DLY)
		{
			g_SystemWarning.TIB = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_5S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_5S;
			}
		}
		else
		{
			tImbErrCnt++;
		}
	}
	else if(temp > g_CellDLTThr.cls_1)
	{
		if (tImbErrCnt > CELL_TIB_WARNING_DLY)
		{
			tImbErrCnt = 0;
			g_SystemWarning.TIB = WARNING_FIRST_LEVEL;
		}
		else
		{
			tImbErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.TIB = 0;
		tImbErrCnt = 0;
	}
}



//============================================================================
// Function    : DetectCellsOverVolt
// Description : ��ⵥ���ع�ѹ
// Parameters  : none
// Returns     : none
//============================================================================
void DetectCellsOverVolt(void)
{
	static uint8_t OvErrCnt = 0;

	if (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
	{    
		return;
	}

	if(g_BatteryParameter.CellVoltMax > g_CellOVThr.cls_2)
	{
		if (OvErrCnt > CELL_OV_FAULT_DLY)
		{
			g_SystemWarning.COV = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_1S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;
			}
		}
		else
		{
			OvErrCnt++;
		}
	}
	else if(g_BatteryParameter.CellVoltMax > g_CellOVThr.cls_1)
	{
		if (OvErrCnt > CELL_OV_WARNING_DLY)
		{
			OvErrCnt = 0;
			g_SystemWarning.COV = WARNING_FIRST_LEVEL;
		}
		else
		{
			OvErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.COV = 0;
		OvErrCnt = 0;
	}
}


//============================================================================
// Function    : DetectCellsUnderVolt
// Description : ��ⵥ����Ƿѹ
// Parameters  : none
// Returns     : none
//============================================================================
void DetectCellsUnderVolt(void)
{
	static uint8_t UvErrCnt = 0;

	if (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
	{
		return;
	}

	if(g_BatteryParameter.CellVoltMin < g_CellUVThr.cls_2)
	{
		if (UvErrCnt > CELL_UV_FAULT_DLY)
		{
			//if (ProtectDelayCnt > RELAY_ACTION_DELAY_20S)
			//{
			//    ProtectDelayCnt = RELAY_ACTION_DELAY_20S;
			//}
			g_SystemWarning.CUV = WARNING_SECOND_LEVEL;
		}
		else
		{
			UvErrCnt++;
		}
	}
	else if(g_BatteryParameter.CellVoltMin < g_CellUVThr.cls_1)
	{
		if (UvErrCnt > CELL_UV_WARNING_DLY)
		{
			g_SystemWarning.CUV = WARNING_FIRST_LEVEL;
			UvErrCnt = 0;
		}
		else
		{
			UvErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.CUV = 0;
		UvErrCnt = 0;
	}
}

//============================================================================
// Function    : DetectCellsVoltImba
// Description : ��ⵥ���ѹ��һ����
// Parameters  : none
// Returns     : none
//============================================================================
void DetectCellsVoltImba(void)
{
	static uint8_t ImbErrCnt = 0;
	uint16_t temp;

	if (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
	{
		return;
	}

   	temp = g_BatteryParameter.CellVoltMax - g_BatteryParameter.CellVoltAvg;

	if(temp > CELL_IB_FAULT)
	{
		if (ImbErrCnt > g_CellIBThr.cls_2)
		{
			g_SystemWarning.CIB = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
			}
		}
		else
		{
			ImbErrCnt++;
		}
	}
	else if(temp > g_CellIBThr.cls_1)
	{
		if (ImbErrCnt > CELL_IB_WARNING_DLY)
		{
			ImbErrCnt = 0;
			g_SystemWarning.CIB = WARNING_FIRST_LEVEL;
		}
		else
		{
			ImbErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.CIB = 0;
		ImbErrCnt = 0;
	}
	
	//TO-DO ��ӵ�о��Сֵ�ü��
	// ......
}

//��ذ��Ĺ�ѹ���
void DetectPackOv(void)
{
	static uint8_t POvErrCnt = 0;

	if (g_SystemWarning.POV == WARNING_SECOND_LEVEL)
	{
		return;
	}

	if(g_BatteryParameter.voltage > g_PackOVThr.cls_2)//CELL_OV_FAULT)
	{
		if (POvErrCnt > PACK_OV_FAULT_DLY)
		{
			g_SystemWarning.POV = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
			}
		}
		else
		{
			POvErrCnt++;
		}
	}
	else if(g_BatteryParameter.voltage > g_PackOVThr.cls_1)
	{
		if (POvErrCnt > PACK_OV_WARNING_DLY)
		{
			POvErrCnt = 0;
			g_SystemWarning.POV = WARNING_FIRST_LEVEL;
		}
		else
		{
			POvErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.POV = 0;
		POvErrCnt = 0;
	}    
}


//��ذ���Ƿѹ���
void DetectPackUv(void)
{
	static uint8_t pUvErrCnt = 0;

	if (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
	{
		return;
	}

	if(g_BatteryParameter.voltage < g_PackUVThr.cls_2)//CELL_OV_FAULT)
	{
		if (pUvErrCnt > PACK_UV_FAULT_DLY)
		{
			g_SystemWarning.PUV = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
			}
		}
		else
		{
			pUvErrCnt++;
		}
	}
	else if(g_BatteryParameter.voltage < g_PackUVThr.cls_1)
	{
		if (pUvErrCnt > PACK_UV_WARNING_DLY)
		{
			pUvErrCnt = 0;
			g_SystemWarning.PUV = WARNING_FIRST_LEVEL;
		}
		else
		{
			pUvErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.PUV = 0;
		pUvErrCnt = 0;
	}    
}


//============================================================================
// Function    : PackDischargeFinish
// Description : ��ⵥ���ѹ�Ƿ�ﵽ�ŵ��ֹ��ѹ
// Parameters  : none
// Returns     : 1:�޷������ŵ� 0:���Լ����ŵ�
//============================================================================
uint8_t DetectPackDischargeFinish(void)
{
	if ( (g_BatteryParameter.CellVoltMin <= CELL_DISCHARGE_END_VOLT) 
		|| (g_BatteryParameter.SOC < 1) || (g_SystemWarning.PUV == WARNING_SECOND_LEVEL))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//============================================================================
// Function    : PackChargeFinish
// Description : ��ⵥ���ѹ�Ƿ�ﵽ����ֹ��ѹ
// Parameters  : none
// Returns     : 1:�����ɣ�0:δ���
//============================================================================
uint8_t DetectPackChargeFinish(void)
{
	static uint16_t chgEndTimer = 0;

	if (g_BatteryParameter.CellVoltMax >= CELL_CHARGE_FULL_VOLT)
	{
		if ( chgEndTimer++ < 3000 ) // 5S
		{
			return 0;
		}
		else 
		{
			return 1;
		}
	}
	else 
	{
		chgEndTimer = 0;
		return 0;    
	}
}

//============================================================================
// Function    ��TskChargerCheck
// Description ������ذ��Ƿ��ڳ��״̬
// Parameters  ��none 
// Returns     ��
//============================================================================
BOOL DetectIsChargedState(void)
{
	//��ʵ�ʵļ�⹤��
	// .....
	return TRUE;
}


//============================================================================
// Function    : GetMaxMinAvgCellVolt
// Description : ��ȡ��о�����С��ѹֵ����оƽ����ѹֵ
// Parameters  : none
// Returns     : none
//============================================================================
void DetectMaxMinAvgCellVolt(void)
{
	uint8_t i,j;
	uint16_t MaxVolt = 0;
	uint16_t MinVolt = 5000;
	uint32_t AvgVolt = 0;

	for(i=0; i<ModuleAmount; i++)
	{
		for(j=0; j<(CellsAmount/ModuleAmount); j++)
		{
			if(MaxVolt < g_ArrayLtc6803Unit[i].CellVolt[j])
			{
				MaxVolt = g_ArrayLtc6803Unit[i].CellVolt[j];
				g_BatteryParameter.MaxCellNum = i * CellsAmount / ModuleAmount + j;
			}

			if(MinVolt > g_ArrayLtc6803Unit[i].CellVolt[j])
			{
				MinVolt = g_ArrayLtc6803Unit[i].CellVolt[j];
				g_BatteryParameter.MinCellNum = i * CellsAmount / ModuleAmount + j;
			}

			AvgVolt += g_ArrayLtc6803Unit[i].CellVolt[j];
		}
	}

	g_BatteryParameter.CellVoltMax = MaxVolt;
	g_BatteryParameter.CellVoltMin = MinVolt;
	g_BatteryParameter.voltage = AvgVolt / 100;
	g_BatteryParameter.CellVoltAvg = (uint16_t)(AvgVolt / CellsAmount);   
}




//============================================================================
// Function    ��TskChargerCheck
// Description ������ذ��Ƿ��ڳ��״̬
// Parameters  ��none 
// Returns     ��
//============================================================================
void DetectCharger(void)
{
	if (!PORTDbits.RD7)
	{
		if (++chgrChkTimer > CHGR_CHK_CYCLE)
		{
			keyChgrState |= 0x01;
			chgrChkTimer = CHGR_CHK_CYCLE;
		}
	}
	else
	{
		if (keyChgrState & 0x01)    
		{
			if( !chgrChkTimer--)
			{
				keyChgrState &= ~0x01;
			}
		} 
		else
		{
			keyChgrState &= ~0x01;
		}
	}       
}



//============================================================================
// Function    ��TskRunkeyCheck
// Description �������Կ��״̬
// Parameters  ��none 
// Returns     ��
//============================================================================
void DetectRunkey(void)
{
	if (PORTDbits.RD4)
	{
		if (++keyChkTimer > CHGR_CHK_CYCLE)
		{
			keyChgrState |= 0x02;
			keyChkTimer = CHGR_CHK_CYCLE;
		}
	}
	else
	{
		if (keyChgrState & 0x02)    
		{
			if( !keyChkTimer--)
			{
				keyChgrState &= ~0x02;
			}
		} 
		else
		{
			keyChgrState &= ~0x02;
		}
	}       
}

#if 0
// �������һ����ذ���ͨѶ״̬ ��ʱδʹ��
void DetectCAN_NeighborPack(void)
{
	if (g_NeighborPack.timer++ > CAN_NEIGHBOR_PACK_TIMEOUT)
	{
		g_SystemError.nbr_comm = 1;
		NeighborPack.timer = 0;
	}
}
#endif


//============================================================================
// Function    ��GetChargeState
// Description ����ȡ��ذ��Ƿ��ڳ��״̬
// Parameters  ��none 
// Returns     ��1,charge signal effective; 0, no charge
//============================================================================
uint8_t GetChargeState(void)
{
   return (keyChgrState & 0x01);
}


//============================================================================
// Function    ��GetKeyrunState
// Description ����ȡ���Կ��״̬
// Parameters  ��none 
// Returns     ��
//============================================================================
uint8_t GetKeyrunState(void)
{
   return (keyChgrState & 0x02);
}














