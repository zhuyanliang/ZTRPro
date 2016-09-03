
#include "include.h"

static int16_t cellVoltTest[ModuleAmount][12] = {0};
static uint8_t ltc6803TestBuf[18] = {0};


//============================================================================
// Function    ��SystemSelftest
// Description ��ϵͳ�ϵ��Լ�
// Parameters  ��none 
// Returns     ��none
//============================================================================ 
uint8_t SystemSelftest(void)
{

	// ltc6803�Լ�
	Ltc6803_Selftest();

	// ����߿�·���
	//Ltc6803_OpenWireTest();
	// cell��ѹ�Լ�
	CellVoltSelftest();

	// ϵͳ�¶��Լ�
	CellTempSelftest();
	AmbTempSelftest();

	// pack��ѹ�Լ�
	//TskRelayVoltMgt();
	if (g_SystemError.ltc_st || g_SystemError.ltc_com || g_SystemError.det_oc)
	{
		if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_1S)
		{
			g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;
		}
	}   

	return 0;
}



//============================================================================
// Function    : Ltc6803_Selftest
// Description : ltc6803оƬ�Լ�
// Parameters  : none
// Returns     : none
//============================================================================
void Ltc6803_Selftest(void)
{
	uint8_t pec, i, j;
	/* ִ��cell voltage selftest1 */
	Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_TEST1);
	DelayMs(300);

	/* ��ȡcell voltage�Ĵ������жϼĴ����Ƿ�ȫ��Ϊ0x55, */
	/* �����ǣ����Լ첻ͨ�� */
	Ltc6803_ChipSelect();
	SPI_SendByte(RDCV_CMD);
	SPI_SendByte(0xDC);
    
	for (i=0; i<ModuleAmount; i++)
	{
		SPI_ReceiveBlock(ltc6803TestBuf, 18);
		pec = SPI_ReceiveByte();

		// PECУ�飬�ж������Ƿ���ȷ����
		if (pec == Ltc6803_BlockCrc8Cal(ltc6803TestBuf, 18))
		{
			for (j=0; j<18; j++)
			{
				if (ltc6803TestBuf[j] != 0x55)
				{
					g_SystemError.ltc_st = 1;
					break;
				}
			}
		}
		else
		{
			g_SystemError.ltc_com = 1;
		}
	}

	Ltc6803_ChipUnselect();

	/* ִ��selftest2 */
	Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_TEST2);
	DelayMs(300);

	/* ��ȡcell voltage�Ĵ������жϼĴ����Ƿ�ȫ��Ϊ0xAA, */
	/* �����ǣ����Լ첻ͨ�� */
	Ltc6803_ChipSelect();
	SPI_SendByte(RDCV_CMD);
	SPI_SendByte(0xDC);

	for (i=0; i<ModuleAmount; i++)
	{
		SPI_ReceiveBlock(ltc6803TestBuf, 18);
		pec = SPI_ReceiveByte();

		// PECУ�飬�ж������Ƿ���ȷ����
		if (pec == Ltc6803_BlockCrc8Cal(ltc6803TestBuf, 18))
		{
			for (j=0; j<18; j++)
			{
				if(ltc6803TestBuf[j] != 0xAA)
				{
					g_SystemError.ltc_st = 1;
					break;
				}
			}
		}
		else
		{
			g_SystemError.ltc_com = 1;
		}
	}

	Ltc6803_ChipUnselect();
}



//============================================================================
// Function    : Ltc6803_OpenWireTest
// Description : ltc6803оƬ����߿�·���
// Parameters  : none
// Returns     : none
//============================================================================
void Ltc6803_OpenWireTest(void)
{
	uint8_t i, j;

	Ltc6803_CellVoltCnvt(STOWAD_CMD, CELL_ALL);
	DelayMs(300);

	if (!Ltc6803_ReadAllCellVolt((Ltc6803_Parameter *)g_ArrayLtc6803Unit))
	{
		g_SystemError.ltc_com = 1;
		return;
	}

	for (i=0; i<2; i++)
	{
		for (j=0; j<12; j++)
		{
			cellVoltTest[i][j] = (int16_t)g_ArrayLtc6803Unit[i].CellVolt[j];
		}
	}

	for (i=0; i<15; i++)
	{
		Ltc6803_CellVoltCnvt(STOWAD_CMD, CELL_ALL);
		DelayMs(300);
	}

	if (!Ltc6803_ReadAllCellVolt((Ltc6803_Parameter *)g_ArrayLtc6803Unit))
	{
		g_SystemError.ltc_com = 1;
		return;
	}

	/* ����߿�·�����ж� */
	if ((cellVoltTest[0][0] < 0)
		|| (cellVoltTest[1][9] < 0)
		|| (g_ArrayLtc6803Unit[0].CellVolt[0] < 0)
		|| (g_ArrayLtc6803Unit[1].CellVolt[9] < 0))
	{
		g_SystemError.det_oc = 1;
	}

	for (i=1; i<10; i++)
	{
		if ((g_ArrayLtc6803Unit[0].CellVolt[i] - cellVoltTest[0][i] > 300)
			|| (g_ArrayLtc6803Unit[0].CellVolt[i] >= 5370))
		{
			g_SystemError.det_oc = 1;
		}
	}

	for (i=0; i<9; i++)
	{
		if ((g_ArrayLtc6803Unit[1].CellVolt[i] - cellVoltTest[1][i] > 300)
			|| (g_ArrayLtc6803Unit[1].CellVolt[i] >= 5370))
		{
			g_SystemError.det_oc = 1;
		}
	}
}



//============================================================================
// Function    : CellVoltSelftest
// Description : ��о��ѹ�Լ�
// Parameters  : none
// Returns     : none
//============================================================================
void CellVoltSelftest(void)
{
	uint8_t cnt = 5;

	while (cnt--)
	{
		Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_ALL);  //���������ѹת��
		DelayMs(250);;  // ת�������Ҫ20ms

		if(Ltc6803_ReadAllCellVolt((Ltc6803_Parameter *)g_ArrayLtc6803Unit))
		{
			DetectMaxMinAvgCellVolt();
			DetectCellsOverVolt();
			DetectCellsUnderVolt();
			DetectCellsVoltImba();
			return;
		}
	}

	g_SystemError.ltc_com = 1;
}



//============================================================================
// Function    : CellTempSelftest
// Description : �ϵ��Լ쵥���¶�
// Parameters  : none
// Returns     : none
//============================================================================
void CellTempSelftest(void)
{
	Ltc6803_TempCnvt(TEMP_ALL);
	DelayMs(20);
	Ltc6803_ReadAllTemp((Ltc6803_Parameter *)g_ArrayLtc6803Unit);	
	g_BatteryParameter.CellTemp[0] = ADCToTempVal(g_ArrayLtc6803Unit[0].Temp1);
	g_BatteryParameter.CellTemp[1] = ADCToTempVal(g_ArrayLtc6803Unit[0].Temp2);
	g_BatteryParameter.CellTemp[2] = ADCToTempVal(g_ArrayLtc6803Unit[1].Temp1);
	g_BatteryParameter.CellTemp[3] = ADCToTempVal(g_ArrayLtc6803Unit[1].Temp2);

	DetectMaxMinCellTemp();
	DetectCellsOverTemp();
	DetectCellsUnderTemp();

	if ( (g_BatteryParameter.CellTempMax >= 125) || (g_BatteryParameter.CellTempMin <= -40) )
	{
		g_SystemError.det_oc = 1;
		LedGreOn();
	}

}




//============================================================================
// Function    : CellTempSelftest
// Description : �ϵ��Լ��·���¶�
// Parameters  : none
// Returns     : none
//============================================================================
void AmbTempSelftest(void)
{
	uint8_t i;

	g_AdcConvertValue.AmbTempIndex = 0;

	for (i=0; i<8; i++)
	{
		ADC_Convert(CHANNEL_TBAVAL);  
		while(ADCON0bits.GO);  //�ȴ�ת����ɣ���Լ��Ҫ15us
		g_AdcConvertValue.AmbTempRaw[g_AdcConvertValue.AmbTempIndex++] = ADC_GetConvertVal();
	}

	// �ɼ���һ��ͼ���ƽ��ֵ
	g_AdcConvertValue.AmbTempAvg = ADC_AverageCalculate(g_AdcConvertValue.AmbTempRaw); 
	g_AdcConvertValue.AmbTempIndex = 0;

	g_BatteryParameter.AmbientTemp = ADCToTempVal(g_AdcConvertValue.AmbTempAvg);

	if (g_BatteryParameter.AmbientTemp > 85)
	{
		g_SystemError.pcb_ot = 1;
	}
}








