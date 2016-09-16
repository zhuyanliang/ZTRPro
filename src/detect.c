
#include "include.h"

//---------------------------------------------------------
volatile SysErrDef		g_SystemError;			// ��¼ϵͳ������Ϣ
volatile SysWarningDef 	g_SystemWarning;		// ��¼ϵͳ������Ϣ


CurrWarnClsDef 	g_BattCOCThr;		// ��ذ�������
CurrWarnClsDef 	g_BattDOCThr;		// �ŵ����
VoltWarnClsDef 	g_PackOVThr;
VoltWarnClsDef 	g_PackUVThr;

VoltWarnClsDef 	g_IsoThr;   	// ��Ե�ȼ���������

//---------------------------------------------------------
VoltWarnClsDef 	g_CellOVThr;
VoltWarnClsDef 	g_CellUVThr;
VoltWarnClsDef 	g_CellIBMThr;	// ����һ����

TempWarnClsDef 	g_PACKCOTThr;	// ��ذ�������
TempWarnClsDef 	g_PACKCUTThr;	// ��ذ�������
TempWarnClsDef 	g_PACKDOTThr;	// ��ذ��ŵ����
TempWarnClsDef 	g_PACKDUTThr;	// ��ذ��ŵ����
TempWarnClsDef 	g_PACKDLTThr;	// ��ذ��²��澯

// ����Ƿ��ڳ������ز���
static uint8_t chgrChkTimer = 0;
static uint8_t keyChkTimer = 0;

// 0-λ��ʾ�Ƿ��г��������
// 1-λ��ʾ�Ƿ���keyrun�ź�
static uint8_t keyChgrState = 0;

//============================================================================
// Function    : DetectPackOverCurrent
// Description : ��ذ�������⣨�����ŵ�����ͳ�������
// Parameters  : none
// Returns     : 
//============================================================================
void DetectPackOverCurrent(void)
{
	static uint16_t ocErrCnt = 0;
	// �Ѿ����ù����������ľ�ֱ�ӷ��أ��ȴ�����ʱ�䵽�Ͽ��̵�������
	// ������
	if (g_SystemWarning.COC == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.current > g_BattCOCThr.cls_2)
		{
			ocErrCnt++;
			if(ocErrCnt > PACK_COC_WARNING_DLY)
			{
				g_SystemWarning.COC = 0;
				ocErrCnt = 0;
			}
		}
		return;
	}
	
	// �ŵ����
	if (g_SystemWarning.DOC == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.current > g_BattDOCThr.cls_2)
		{
			ocErrCnt++;
			if(ocErrCnt > PACK_DOC_WARNING_DLY)
			{
				g_SystemWarning.DOC = 0;
				ocErrCnt = 0;
			}
		}
		return;
	}
	
	if (g_BatteryMode == CHARGE)
	{
		if (g_BatteryParameter.current < g_BattCOCThr.cls_2)
		{
			if (ocErrCnt > PACK_COC_FAULT_DLY)
			{
				g_SystemWarning.COC = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_5S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_5S;
				}
			}
			else
			{
				ocErrCnt++;
			}
		} 
		else if (g_BatteryParameter.current < g_BattCOCThr.cls_1)
		{
			if (ocErrCnt > PACK_COC_WARNING_DLY)
			{
				g_SystemWarning.COC = WARNING_FIRST_LEVEL;
				ocErrCnt = 0;
			}
			else
			{
				ocErrCnt++;
			}
		}
		else
		{
			g_SystemWarning.COC = 0;
			ocErrCnt = 0;
		}
	}
	else  // �ŵ����
	{
		if (g_BatteryParameter.current > g_BattDOCThr.cls_2)
		{
			if (ocErrCnt > PACK_DOC_FAULT_DLY)
			{
				g_SystemWarning.DOC = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				ocErrCnt++;
			}
		}
		else if (g_BatteryParameter.current > g_BattDOCThr.cls_1)
		{
			if (ocErrCnt > PACK_DOC_WARNING_DLY)
			{
				ocErrCnt = 0;
				g_SystemWarning.DOC = WARNING_FIRST_LEVEL;
			}
			else
			{
				ocErrCnt++;
			}
		}
		else
		{
			g_SystemWarning.DOC = 0;
			ocErrCnt = 0;
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
	int16_t maxTemp = -40;
	int16_t minTemp = 125;
	int16_t avgTemp = 0;
    

	for (i = 0; i < MAX_TEMP_SENSOR; i++)
	{
		if (maxTemp < g_BatteryParameter.CellTemp[i])
		{
			maxTemp = g_BatteryParameter.CellTemp[i];
			g_BatteryParameter.MaxTempChnIdx = i+1;
		}

		if (minTemp > g_BatteryParameter.CellTemp[i])
		{
			minTemp = g_BatteryParameter.CellTemp[i];
			g_BatteryParameter.MinTempChnIdx = i+1;   
		}
		avgTemp += g_BatteryParameter.CellTemp[i];
	}

	g_BatteryParameter.CellTempMax = maxTemp;
	g_BatteryParameter.CellTempMin = minTemp;
	g_BatteryParameter.CellTempAvg = avgTemp / MAX_TEMP_SENSOR;
}

//============================================================================
// Function    : DetectCellsOverTemp
// Description : ����¶ȹ���
// Parameters  : none
// Returns     : 
//============================================================================
void DetectCellsOverTemp(void)
{
	static uint8_t otErrCnt = 0;

	//�¶�ģ��ͨѶ����
	if (g_SystemError.tdu_comm ) 
		return;

	if(g_SystemWarning.COT == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.CellTempMax < g_PACKCOTThr.cls_2)
		{
			otErrCnt++;
			if (otErrCnt > CELL_COT_FAULT_DLY)
			{
				otErrCnt = 0;
				g_SystemWarning.COT = 0;
			}	
		}
		return;
	}
	if(g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
	{
		if (g_BatteryParameter.CellTempMax < g_PACKCOTThr.cls_2)
		{ 
			otErrCnt++;
			if (otErrCnt > CELL_DOT_FAULT_DLY)
			{
				otErrCnt = 0;
				g_SystemWarning.DOT = 0;
			}	
		}
		return;
	}

	if(g_BatteryMode == CHARGE)
	{
		if(g_BatteryParameter.CellTempMax > g_PACKCOTThr.cls_2)
		{
			if(otErrCnt > CELL_COT_FAULT_DLY)
			{
				g_SystemWarning.COT = WARNING_SECOND_LEVEL;
				if(g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				otErrCnt++;
			}
		}
		else if(g_BatteryParameter.CellTempMax > g_PACKCOTThr.cls_1)
		{
			if (otErrCnt > CELL_COT_WARNING_DLY)
			{
				g_SystemWarning.COT = WARNING_FIRST_LEVEL;
				otErrCnt = 0;
			}
			else
			{
				otErrCnt++;
			}
		}
		else
		{
			otErrCnt = 0;
			g_SystemWarning.COT = 0;
		}
	}
	else
	{
		if(g_BatteryParameter.CellTempMax > g_PACKDOTThr.cls_2)
		{   
			if(otErrCnt > CELL_DOT_FAULT_DLY)
			{
				g_SystemWarning.DOT = WARNING_SECOND_LEVEL;
				if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				otErrCnt++;
			}
		}
		else if(g_BatteryParameter.CellTempMax > g_PACKDOTThr.cls_1)
		{
			if(otErrCnt > CELL_DOT_WARNING_DLY)
			{
				g_SystemWarning.DOT = WARNING_FIRST_LEVEL;
				otErrCnt = 0;
			}
			else
			{
				otErrCnt++;
			}
		}
		else
		{
			otErrCnt = 0;
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
	static uint8_t utErrCnt = 0;

	if(g_SystemError.tdu_comm) 
		return;

	if(g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.CellTempMin > g_PACKCUTThr.cls_2)
		{       
			if(utErrCnt > CELL_CUT_FAULT_DLY)
			{
				utErrCnt = 0;
				g_SystemWarning.CUT = 0;
			}
			else
			{
				utErrCnt++;
			}
		}
		return;
	}
	if(g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.CellTempMin > g_PACKCUTThr.cls_2)
		{
			if(utErrCnt > CELL_DUT_FAULT_DLY)
			{
				utErrCnt = 0;
				g_SystemWarning.DUT = 0;
			}
			else
			{
				utErrCnt++;
			}
		}
		return;
	}

	if(g_BatteryMode == CHARGE)
	{
		if(g_BatteryParameter.CellTempMin < g_PACKCUTThr.cls_2)
		{       
			if(utErrCnt > CELL_CUT_FAULT_DLY)
			{
				g_SystemWarning.CUT = WARNING_SECOND_LEVEL;
				if(g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				utErrCnt++;
			}
		}
		else if(g_BatteryParameter.CellTempMin < g_PACKCUTThr.cls_1)
		{
			if(utErrCnt > CELL_CUT_WARNING_DLY)
			{
				utErrCnt= 0;
				g_SystemWarning.CUT = WARNING_FIRST_LEVEL;
			}
			else
			{
				utErrCnt++;
			}
		}
		else
		{
			utErrCnt = 0;
			g_SystemWarning.CUT = 0;
		}
	}
	else
	{
		if(g_BatteryParameter.CellTempMin < g_PACKDUTThr.cls_2)
		{

			if(utErrCnt > CELL_DUT_FAULT_DLY)
			{
				g_SystemWarning.DUT = WARNING_SECOND_LEVEL;
				if(g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
				}
			}
			else
			{
				utErrCnt++;
			}
		}
		else if(g_BatteryParameter.CellTempMin < g_PACKDUTThr.cls_1)
		{
			if(utErrCnt > CELL_DUT_WARNING_DLY)
			{
				utErrCnt = 0;
				g_SystemWarning.DUT = WARNING_FIRST_LEVEL;
			}
			else
			{
				utErrCnt++;
			}
		}
		else
		{
			utErrCnt = 0;
			g_SystemWarning.DUT = 0;
		}
	}
}

void DetectPCBOverTemp(void)
{
	static uint8_t otErrCnt = 0;

	if(g_SystemWarning.PCBOT == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.AmbientTemp < PcbOverTempValue)
		{
			otErrCnt++;
			if(otErrCnt >= 5)
			{
				otErrCnt = 0;
				g_SystemWarning.PCBOT = 0;
			}
		}
	}
	else
	{
		if(g_BatteryParameter.AmbientTemp >= PcbOverTempValue)
		{
			otErrCnt++;
			if(otErrCnt >= 5)
			{
				otErrCnt = 0;
				g_SystemWarning.PCBOT = WARNING_SECOND_LEVEL;
				if(g_ProtectDelayCnt > RELAY_ACTION_DELAY_1S)
				{
					g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;
				}
			}
			else
			{
				otErrCnt++;
			}
		}
		else
		{
			otErrCnt = 0;
			g_SystemWarning.PCBOT = 0;
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
		if(temp < g_PACKDLTThr.cls_2)
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

	if(temp > g_PACKDLTThr.cls_2)
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
	else if(temp > g_PACKDLTThr.cls_1)
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
	static uint8_t ovErrCnt = 0;

	if (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
	{    
		if(g_BatteryParameter.CellVoltMax < g_CellOVThr.cls_2)
		{
			if (ovErrCnt > CELL_TIB_FAULT_DLY)
			{
				ovErrCnt = 0;
				g_SystemWarning.COV = 0;
			}
			else
			{
				ovErrCnt++;
			}
		}
		return;
	}

	if(g_BatteryParameter.CellVoltMax > g_CellOVThr.cls_2)
	{
		if (ovErrCnt > CELL_OV_FAULT_DLY)
		{
			g_SystemWarning.COV = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_1S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;
			}
		}
		else
		{
			ovErrCnt++;
		}
	}
	else if(g_BatteryParameter.CellVoltMax > g_CellOVThr.cls_1)
	{
		if (ovErrCnt > CELL_OV_WARNING_DLY)
		{
			ovErrCnt = 0;
			g_SystemWarning.COV = WARNING_FIRST_LEVEL;
		}
		else
		{
			ovErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.COV = 0;
		ovErrCnt = 0;
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
	static uint8_t uvErrCnt = 0;

	if (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.CellVoltMin > g_CellUVThr.cls_2)
		{
			if (uvErrCnt > CELL_UV_FAULT_DLY)
			{
				uvErrCnt = 0;
				g_SystemWarning.CUV = 0;
			}
			else
			{
				uvErrCnt++;
			}
		}
		return;
	}

	if(g_BatteryParameter.CellVoltMin < g_CellUVThr.cls_2)
	{
		if (uvErrCnt > CELL_UV_FAULT_DLY)
		{
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_20S)
			{
			    g_ProtectDelayCnt = RELAY_ACTION_DELAY_20S;
			}
			g_SystemWarning.CUV = WARNING_SECOND_LEVEL;
		}
		else
		{
			uvErrCnt++;
		}
	}
	else if(g_BatteryParameter.CellVoltMin < g_CellUVThr.cls_1)
	{
		if (uvErrCnt > CELL_UV_WARNING_DLY)
		{
			g_SystemWarning.CUV = WARNING_FIRST_LEVEL;
			uvErrCnt = 0;
		}
		else
		{
			uvErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.CUV = 0;
		uvErrCnt = 0;
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
	static uint8_t imbErrCnt = 0;
	uint16_t temp;

   	temp = g_BatteryParameter.CellVoltMax - g_BatteryParameter.CellVoltAvg;

	if (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
	{
		if(temp < g_CellIBMThr.cls_2)
		{
			if (imbErrCnt > CELL_UV_FAULT_DLY)
			{
				imbErrCnt = 0;
				g_SystemWarning.CIB = 0;
			}
			else
			{
				imbErrCnt++;
			}
		}
		return;
	}
	
	if(temp > g_CellIBMThr.cls_2)
	{
		if (imbErrCnt > CELL_IB_WARNING_DLY)
		{
			g_SystemWarning.CIB = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
			}
		}
		else
		{
			imbErrCnt++;
		}
	}
	else if(temp > g_CellIBMThr.cls_1)
	{
		if (imbErrCnt > CELL_IB_WARNING_DLY)
		{
			imbErrCnt = 0;
			g_SystemWarning.CIB = WARNING_FIRST_LEVEL;
		}
		else
		{
			imbErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.CIB = 0;
		imbErrCnt = 0;
	}
	
	//TO-DO ��ӵ�о��Сֵ�ü��
	// ......
}

//��ذ��Ĺ�ѹ���
void DetectPackOv(void)
{
	static uint8_t pOvErrCnt = 0;

	if (g_SystemWarning.POV == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.voltage < g_PackOVThr.cls_2)
		{
			if (pOvErrCnt > PACK_OV_FAULT_DLY)
			{
				pOvErrCnt = 0;
				g_SystemWarning.POV = 0;
			}
			else
			{
				pOvErrCnt ++;
			}
		}
		return;
	}

	if(g_BatteryParameter.voltage > g_PackOVThr.cls_2)
	{
		if (pOvErrCnt > PACK_OV_FAULT_DLY)
		{
			g_SystemWarning.POV = WARNING_SECOND_LEVEL;
			if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_10S)
			{
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_10S;
			}
		}
		else
		{
			pOvErrCnt++;
		}
	}
	else if(g_BatteryParameter.voltage > g_PackOVThr.cls_1)
	{
		if (pOvErrCnt > PACK_OV_WARNING_DLY)
		{
			pOvErrCnt = 0;
			g_SystemWarning.POV = WARNING_FIRST_LEVEL;
		}
		else
		{
			pOvErrCnt++;
		}
	}
	else
	{
		g_SystemWarning.POV = 0;
		pOvErrCnt = 0;
	}    
}


//��ذ���Ƿѹ���
void DetectPackUv(void)
{
	static uint8_t pUvErrCnt = 0;

	if (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
	{
		if(g_BatteryParameter.voltage > g_PackUVThr.cls_2)
		{
			if(pUvErrCnt > PACK_UV_FAULT_DLY)
			{
				pUvErrCnt = 0;
				g_SystemWarning.PUV = 0;
			}
			else
			{
				pUvErrCnt++;
			}
		}
		return;
	}

	if(g_BatteryParameter.voltage < g_PackUVThr.cls_2)
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
// Description : ����ذ���ѹ�Ƿ�ﵽ�ŵ��ֹ��ѹ
// Parameters  : none
// Returns     : 1:�޷������ŵ� 0:���Լ����ŵ�
//============================================================================
uint8_t DetectPackDischargeFinish(void)
{
	if ( (g_BatteryParameter.CellVoltMin <= g_PackUVThr.cls_2) 
		|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
		|| (g_BatteryParameter.SOC == 0))
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
// Description : ����ذ��Ƿ�ﵽ����ֹ��ѹ
// Parameters  : none
// Returns     : 1:�����ɣ�0:δ���
//============================================================================
uint8_t DetectPackChargeFinish(void)
{
	static uint16_t chgEndTimer = 0;

	if ((g_BatteryParameter.CellVoltMax >= g_CellOVThr.cls_2)
		|| (g_BatteryParameter.SOC == 100))
	{
		if ( chgEndTimer++ < 10 ) 
		{
			return 0;
		}
		else 
		{
			chgEndTimer = 15;
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
// Function    : GetMaxMinAvgCellVolt
// Description : ��ȡ��о�����С��ѹֵ����оƽ����ѹֵ
// Parameters  : none
// Returns     : none
//============================================================================
void DetectMaxMinAvgCellVolt(void)
{
	uint8_t i,j;
	uint16_t maxVolt = 0;
	uint16_t minVolt = 5000;
	uint32_t sumVolt = 0;

	for(i=0; i<ModuleAmount; i++)
	{
		for(j=0; j<(CellsAmount/ModuleAmount); j++)
		{
			if(maxVolt < g_ArrayLtc6803Unit[i].CellVolt[j])
			{
				maxVolt = g_ArrayLtc6803Unit[i].CellVolt[j];
				g_BatteryParameter.MaxCellNum = i * CellsAmount / ModuleAmount + j;
			}

			if(minVolt > g_ArrayLtc6803Unit[i].CellVolt[j])
			{
				minVolt = g_ArrayLtc6803Unit[i].CellVolt[j];
				g_BatteryParameter.MinCellNum = i * CellsAmount / ModuleAmount + j;
			}

			sumVolt += g_ArrayLtc6803Unit[i].CellVolt[j];
		}
	}

	g_BatteryParameter.CellVoltMax = maxVolt;
	g_BatteryParameter.CellVoltMin = minVolt;
	g_BatteryParameter.voltage = sumVolt / 100;
	g_BatteryParameter.CellVoltAvg = (uint16_t)(sumVolt / CellsAmount);   
}


//============================================================================
// Function    ��TskChargerCheck
// Description ������ذ��Ƿ��ڳ��״̬
// Parameters  ��none 
// Returns     ��
//============================================================================
void DetectCharger(void)
{
	if (!PORTDbits.RD7) // �г��������
	{
		if (++chgrChkTimer > CHGR_CHK_CYCLE)
		{
			keyChgrState |= 0x01;
			chgrChkTimer = CHGR_CHK_CYCLE;
		}
	}
	else  // û�г��������
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
	if (PORTDbits.RD4) // keyrun��ͨ
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
