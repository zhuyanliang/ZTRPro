
#include "include.h" 

/*
 * �����ѹ������������...
 * �����ܺ���ģ��
 * ������ģ���ȫ�ֱ����Ķ���
 *
 */

volatile BatteryPackTypedef g_BatteryParameter;
volatile BatteryModeTypedef g_BatteryMode = IDLE;			

uint16_t 					g_ProtectDelayCnt;	// ������ʱ���� 
uint16_t 					g_PrechargeTimer = 0;
uint8_t 					g_EnterLowPoweModeFlg;

//----------------------------------------------------------------------------
// Function    : TskAfe_Init
// Description : ��ز��ֲ����ĳ�ʼ��
// Parameters  : none
// Returns     : 
//----------------------------------------------------------------------------
void TskBatteryPra_Init(void)
{
	uint8_t i;
	uint8_t crc = 0;
	uint8_t *ptr;
	uint8_t buff[5];

	g_BatteryParameter.voltage = 0;
	g_BatteryParameter.current = 0;
	g_BatteryParameter.SOC = 0;
	g_BatteryParameter.CellVoltMax = 0;
	g_BatteryParameter.CellVoltMin = 0;
	g_BatteryParameter.CellVoltAvg = 0;
	g_BatteryParameter.CellTemp[0] = 0;
	g_BatteryParameter.CellTemp[1] = 0;
	g_BatteryParameter.CellTemp[2] = 0;
	g_BatteryParameter.CellTemp[3] = 0;
	g_BatteryParameter.CellTempMax = 0;
	g_BatteryParameter.CellTempMin = 0;
	g_BatteryParameter.AmbientTemp = 0;
	g_BatteryParameter.BalanceCellNum = 0;
	g_BatteryParameter.BalanceFlag = 0;

	// ��ȡ��絥����侯���ȼ�
	EEPROM_ReadBlock(EEPROM_ADDR_COV_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_CellOVThr.cls_1;

		for (i = 0; i < 5; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_CellOVThr = CellOVThrDefault; 
	}

	
	// ��ȡ����ŵ��ѹ�����ȼ�
	EEPROM_ReadBlock(EEPROM_ADDR_CUV_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_CellUVThr.cls_1;

		for (i = 0; i < 5; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_CellUVThr = CellUVThrDefault; 
	}

	// ��ȡ�����¾����ȼ�
	EEPROM_ReadBlock(EEPROM_ADDR_COT_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_PACKCOTThr.cls_1;

		for (i = 0; i < 5; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_PACKCOTThr = CellCOTThrDefault; 
	}

	//��ȡ�����±����ȼ�
	EEPROM_ReadBlock(EEPROM_ADDR_CUT_THRHOLD, buff, 5);
    crc = calculate_crc8(buff, 5);

    if (crc == buff[4])
    {
        ptr = (uint8_t *)&g_PACKCUTThr.cls_1;

        for (i = 0; i < 5; i++)
        {
            *ptr++ = buff[i];
        }
    }
    else
    {
        g_PACKCUTThr = CellCUTThrDefault; 
    }
    
	// ��ȡ��ذ��ŵ����
    EEPROM_ReadBlock(EEPROM_ADDR_DOT_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_PACKDOTThr.cls_1;

		for (i = 0; i < 4; i++)
		{
		    *ptr++ = buff[i];
		}
	}
	else
	{
		g_PACKDOTThr = CellDOTThrDefault; 
	}

	// ��ȡ��ذ��ŵ����	
    EEPROM_ReadBlock(EEPROM_ADDR_DUT_THRHOLD, buff, 5);
    crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_PACKDUTThr.cls_1;

		for (i = 0; i < 4; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_PACKDUTThr = CellDUTThrDefault; 
	}

	// ��ȡ��ذ������������ȼ�
    EEPROM_ReadBlock(EEPROM_ADDR_COC_THRHOLD, buff, 5);
    crc = calculate_crc8(buff, 4);

    if (crc == buff[4])
    {
        ptr = (uint8_t *)&g_BattCOCThr.cls_1;

        for (i = 0; i < 4; i++)
        {
            *ptr++ = buff[i];
        }
    }
    else
    {
        g_BattCOCThr = BattCOCThrDefault; 
    }
	// ��ȡ��ذ��ŵ���������ȼ�
    EEPROM_ReadBlock(EEPROM_ADDR_DOC_THRHOLD, buff, 5);
    crc = calculate_crc8(buff, 4);

    if (crc == buff[4])
    {
        ptr = (uint8_t *)&g_BattDOCThr.cls_1;

        for (i = 0; i < 4; i++)
        {
            *ptr++ = buff[i];
        }
    }
    else
    {
        g_BattDOCThr = BattDOCThrDefault; 
    }

    // ��ȡ��ذ��²�ȼ�����
	EEPROM_ReadBlock(EEPROM_ADDR_DLT_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_PACKDLTThr.cls_1;

		for (i = 0; i < 4; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_PACKDLTThr = CellDLTThrDefault; 
	}

	// ��ȡ��ذ���ѹ�ȼ�
	EEPROM_ReadBlock(EEPROM_ADDR_POV_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_PackOVThr.cls_1;

		for (i = 0; i < 5; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_PackOVThr = PackOVThrDefault; 
	}
	
	// ��ȡ��ذ���ѹ�ȼ�
	EEPROM_ReadBlock(EEPROM_ADDR_PUV_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_PackUVThr.cls_1;

		for (i = 0; i < 5; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_PackUVThr = PackUVThrDefault; 
	}
	
	// ��ȡ�����һ���Եȼ�
	EEPROM_ReadBlock(EEPROM_ADDR_DLV_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_CellIBMThr.cls_1;

		for (i = 0; i < 5; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_CellIBMThr = CellIBMThrDefault; 
	}

}


//============================================================================
// Function    : TskCurMgt
// Description : ��ذ�ĸ�ߵ�������
// Parameters  : none
// Returns     : 
//============================================================================
void TskCurrentMgt(void)
{
	uint8_t i;
	static uint8_t curr_flg = 0;

	ADC_Convert(CHANNEL_IHIGH);  
	
	while(ADCON0bits.GO);  //�ȴ�ת����ɣ���Լ��Ҫ15us
	//��һ������ʱ��һ���Զ�ȡ8������ֵ
	if (!curr_flg)
	{
		for ( i = 0; i < 8; i++ )
		{
			g_AdcConvertValue.CurHighRaw[i] = ADC_GetCvtRaw();
		} 
		curr_flg = 1;    
	}
	else //�Ժ�ÿ�θ���һ����ѹֵ
	{ 
		g_AdcConvertValue.CurHighRaw[g_AdcConvertValue.CurHighIndex++ & 0x07] = ADC_GetCvtRaw();
	}
	
	g_AdcConvertValue.CurHighAvg = ADC_AverageCalculate(g_AdcConvertValue.CurHighRaw);

	//����ADC����ֵ����ϵ����ɼ��������ԣ���ȡ��ʵ�ĵ���ֵ
	g_BatteryParameter.current = (int16_t)((((int32_t)g_AdcConvertValue.CurHighAvg * 6250) >> 12) - 3125) - g_CurrentOffset;
	//g_BatteryParameter.current = (int16_t)((g_AdcConvertValue.CurHighAvg-g_CurrentOffset)/3276.0 * 5000); 
	//g_BatteryParameter.current = (int16_t)((g_AdcConvertValue.CurHighAvg-g_CurrentOffset)/4096.0 * 5000); 
	//g_BatteryParameter.current -= 2500;
	
	//����ذ������Ƿ񳬹��޶�ֵ
	DetectPackOverCurrent();
}


//============================================================================
// Function    : �¶ȴ���
// Description : �ĸ�ͨ�����¶�ֵ������ɺ�Ž���һ���¶ȼ��
// Parameters  : none
// Returns     : 
//============================================================================
void TskCellTempMgt(void)
{
	static int8_t  state = 0;

	switch(state++)
	{
	case 0:
		g_BatteryParameter.CellTemp[0] = ADCToTempVal(g_ArrayLtc6803Unit[0].Temp1);
		break;
	case 1:
		g_BatteryParameter.CellTemp[1] = ADCToTempVal(g_ArrayLtc6803Unit[0].Temp2); 
		break;
	case 2:
		g_BatteryParameter.CellTemp[2] = ADCToTempVal(g_ArrayLtc6803Unit[1].Temp1);
		break;
	case 3:
		g_BatteryParameter.CellTemp[3] = ADCToTempVal(g_ArrayLtc6803Unit[1].Temp2); 
		break;
	case 4:
		// ����¶�ֵ�����õ����������С�¶�ֵ
		DetectMaxMinCellTemp();
		DetectCellsOverTemp();
		DetectCellsUnderTemp();
		DetectCellTempDlt();
		state = 0;
		break;
	default:
		state = 0;
		break;
	}
}


//============================================================================
// Function    : TskSOCMgt
// Description : �Ե��SOC�Ĺ����漰��Ah,Soc,Accumulator�Ĵ洢
// Parameters  : 
// Returns     : none
//============================================================================
void TskSOCMgt(void)
{
   Soc_CalculateAh();	
   Soc_Update();
   Soc_StoreSoc();
}

//============================================================================
// Function    ��TskRelayMgt
// Description ���ú�������pack��״̬������Ӧ�̵����Ŀ��ر�־״̬
// Parameters  ��none 
// Returns     ��none
//============================================================================
void TskRelayMgt(void)
{
	switch(g_BatteryMode)
	{
	case IDLE:
		g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.positive = FALSE;
		g_RelayActFlg.negative = FALSE;
		g_RelayActFlg.cooling = FALSE;
		break;

	case PRECHARGE:  //Ԥ���״̬
		g_RelayActFlg.positive = FALSE;
		g_RelayActFlg.negative = TRUE;
		g_RelayActFlg.precharge = TRUE;
		break;

	case DISCHARGE:  //�ŵ�״̬
		if(g_BatteryParameter.CellTempAvg >= (DischargeNeedCoolTemp-5))
			g_RelayActFlg.cooling = TRUE;
		else
			g_RelayActFlg.cooling = FALSE;
		g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.positive = TRUE;
		g_RelayActFlg.negative = TRUE;
		break;
	
	case CHARGE:  //���״̬
		//if(g_BatteryParameter.CellTempAvg >= (ChargeNeedCoolTemp-5))
			//g_RelayActFlg.cooling = TRUE;
		//else
			//g_RelayActFlg.cooling = FALSE;	
		g_RelayActFlg.precharge = FALSE;
		if(g_BatteryParameter.SOC > 99) // ������
		{
			g_RelayActFlg.positive = FALSE;
			g_RelayActFlg.negative = FALSE;
		}
		else
		{
			g_RelayActFlg.positive = TRUE;
			g_RelayActFlg.negative = TRUE;
		}
		break;

	case HEATING:
		g_RelayActFlg.heating = TRUE;
		g_RelayActFlg.cooling = FALSE;
        g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.positive = FALSE;
		g_RelayActFlg.negative = FALSE;
		break;

	case COOLING:
		g_RelayActFlg.heating = FALSE;
		g_RelayActFlg.cooling = TRUE;
        g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.positive = FALSE;
		g_RelayActFlg.negative = FALSE;
		break;

	case PROTECTION:  //����״̬�������ǹ��ϱ������ŵ��ֹ����������ֹ�����ȵ� 
		g_RelayActFlg.precharge = FALSE;
		if (!g_ProtectDelayCnt--)
		{              
			g_RelayActFlg.positive = FALSE;
			g_RelayActFlg.negative = FALSE;
		}
        break;
	default:
		break;
	}
}



//============================================================================
// Function    : TaskFaultStore
// Description : �Դ�����Ϣ���й���
// Parameters  : none
// Returns     : none
//============================================================================
void TskFaultStoreMgt(void)
{
	static uint16_t state = 0;

	switch(state & 0x0001) // ����ѹ
	{
	case 0:
		if (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cuv < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cuv++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_CUV, (uint8_t*)&g_FaultRecord.cuv, 2);
				Nvm_UpdateRecentFaultRec(FALT_CUV);
			}
			state |= 0x0001;
		}
		break;

	case 1:
		if (g_SystemWarning.CUV != WARNING_SECOND_LEVEL)
		{
			state &= ~0x0001;
		}
		break;

	default:
		break;
	}

	switch (state & 0x0002) // ����ѹ
	{
	case 0:
		if (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cov < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cov++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_COV, (uint8_t*)&g_FaultRecord.cov, 2);
				Nvm_UpdateRecentFaultRec(FALT_COV);
			}
			state |= 0x0002;
		}
		break;

	case 0x0002:
		if (g_SystemWarning.COV != WARNING_SECOND_LEVEL)
		{
			state &= ~0x0002;
		}
		break;
	default:
		break;
	}

	switch (state & 0x0004)	// ����һ����
	{
	case 0:
		if (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cib < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cib++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_CIB, (uint8_t*)&g_FaultRecord.cib, 2);
				Nvm_UpdateRecentFaultRec(FALT_CIB);	
			}
			state |= 0x0004;
		}
		break;

	case 0x0004:
		if (g_SystemWarning.CIB != WARNING_SECOND_LEVEL)
		{
			state &= ~0x0004;
		}
		break;
	}

	switch (state & 0x0008)
	{
	case 0:
		if (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cut < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cut++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_CUT, (uint8_t*)&g_FaultRecord.cut, 2);
				Nvm_UpdateRecentFaultRec(FALT_CUT);
			}
			state |= 0x0008;
		}
		if (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.dut < FAULT_REC_LIMIT)
			{
				g_FaultRecord.dut++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_DUT, (uint8_t*)&g_FaultRecord.dut, 2);
				Nvm_UpdateRecentFaultRec(FALT_CUT);
			}
			state |= 0x0008;
		}
		break;

	case 0x0008:
		if ((g_SystemWarning.CUT != WARNING_SECOND_LEVEL)
			&& (g_SystemWarning.DUT != WARNING_SECOND_LEVEL))
		{
			state &= ~0x0008;
		}
		break;
	}

	switch (state & 0x0010)
	{
	case 0:
		if (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cot < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cot++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_COT, (uint8_t*)&g_FaultRecord.cot, 2);
				Nvm_UpdateRecentFaultRec(FALT_COT);
			}
			state |= 0x0010;
		}
		if (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.dot < FAULT_REC_LIMIT)
			{
				g_FaultRecord.dot++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_DOT, (uint8_t*)&g_FaultRecord.dot, 2);
				Nvm_UpdateRecentFaultRec(FALT_COT);
			}
			state |= 0x0010;
		}
		break;

	case 0x0010:
		if ( (g_SystemWarning.COT != WARNING_SECOND_LEVEL)
			&& (g_SystemWarning.DOT != WARNING_SECOND_LEVEL))
		{
			state &= ~0x0010;
		}
		break;
	}

	switch (state & 0x0020)
	{
	case 0:
		if (g_SystemWarning.COC == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.coc < FAULT_REC_LIMIT)
			{
				g_FaultRecord.coc++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_COC, (uint8_t*)&g_FaultRecord.coc, 2);
				Nvm_UpdateRecentFaultRec(FALT_COC);
			}
			state |= 0x0020;
		}
		if (g_SystemWarning.DOC == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.doc < FAULT_REC_LIMIT)
			{
				g_FaultRecord.doc++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_DOC, (uint8_t*)&g_FaultRecord.doc, 2);
				Nvm_UpdateRecentFaultRec(FALT_COC);
			}
			state |= 0x0020;
		}
		break;

	case 0x0020:
		if ((g_SystemWarning.COC != WARNING_SECOND_LEVEL) 
			&&(g_SystemWarning.DOC != WARNING_SECOND_LEVEL))	
		{
			state &= ~0x0020;
		}
		break;
	}

	switch (state & 0x0080)
	{
	case 0:
		if (g_SystemError.ltc_st)
		{
			if (g_FaultRecord.ltc_st < FAULT_REC_LIMIT)
			{
				g_FaultRecord.ltc_st++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_LTCST, (uint8_t*)&g_FaultRecord.ltc_st, 2);
				Nvm_UpdateRecentFaultRec(FALT_LTC);	
			}
			state |= 0x0080;
		}
		break;

	case 0x0080:
		if (!g_SystemError.ltc_st)
		{
			state &= ~0x0080;
		}
		break;
	}

	switch (state & 0x0100)
	{
	case 0:
		if(g_SystemError.ltc_com)
		{
			if(g_FaultRecord.ltc_com < FAULT_REC_LIMIT)
			{
				g_FaultRecord.ltc_com++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_LTCCOM, (uint8_t*)&g_FaultRecord.ltc_com, 2);
				Nvm_UpdateRecentFaultRec(FALT_LTC);
			}
			state |= 0x0100;
		}
		break;

	case 0x0100:
		if (!g_SystemError.ltc_com)
		{
			state &= ~0x0100;
		}
		break;
	}
}


//============================================================================
// Function    : TskBlncMgt
// Description : 
// Parameters  : none
// Returns     : none
//============================================================================
void TskBlncMgt(void)
{
	static uint8_t balFlg = 0;
	static uint8_t n = 0;
	static uint16_t timeStamp = 0;
	uint8_t i, j;

	if (g_BatteryMode == CHARGE)
	{
		if (g_SysTickMs - timeStamp > 5000)
		{
			g_ArrayLtc6803Unit[0].CellBal = 0;
			g_ArrayLtc6803Unit[1].CellBal = 0;

			if (g_BatteryParameter.CellVoltMin 
				> CELL_BALANCE_OPEN_VOLT)
			{
				for(i=0; i<ModuleAmount; i++)
				{
					for(j=0;j<(CellsAmount/ModuleAmount);j++)
					{
						if(g_ArrayLtc6803Unit[i].CellVolt[j] - g_BatteryParameter.CellVoltMin 
							> CELL_BALANCE_THRESHOLD)
						{
							g_ArrayLtc6803Unit[i].CellBal |= 0x0001 << j;
						}
					}
				}
			}

			Ltc6803_WriteCfgRegGroup(g_ArrayLtc6803Unit);

			balFlg = 1;
			timeStamp = g_SysTickMs;
		}
	}
	else if(balFlg == 1)
	{
		if(n++ < 3)
		{
			g_ArrayLtc6803Unit[0].CellBal = 0;
			g_ArrayLtc6803Unit[1].CellBal = 0;

			Ltc6803_WriteCfgRegGroup(g_ArrayLtc6803Unit);
		}
		else
		{
			balFlg = 0;
			n = 0;
		}
	}
}



//============================================================================
// Function    : TskAfeMgt ģ��ǰ�� analog front end
// Description : �ú���ʵ��LTC6803-3������ת������ȡ���ݡ����ݼ��㡢����ȹ���
//               ʵ�ֺ�״̬�л�
// Parameters  : none
// Returns     : none
//============================================================================
void TskAfeMgt(void)
{
	static uint32_t timeStamp;
	static uint8_t ComErrCnt = 5;
	static AfeStateTypedef AfeState = AFE_VOLT_CNVT;
	uint8_t i;
        
	switch (AfeState)
	{
	case AFE_VOLT_CNVT:
		Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_ALL);  //���������ѹת��
		timeStamp = g_SysTickMs;  //��¼ת����ʼʱ��
		AfeState = AFE_READ_VOLT;  //״̬�л�
		break;

	case AFE_READ_VOLT:
		if (g_SysTickMs - timeStamp >= 10)  // ת�������Ҫ20ms
		{
			if (Ltc6803_ReadAllCellVolt((Ltc6803_Parameter *)g_ArrayLtc6803Unit))
			{
				AfeState = AFE_VOLT_DETECT;
				g_SystemError.ltc_com = 0;
				ComErrCnt = 5;
			}
			else
			{
				AfeState = AFE_VOLT_CNVT;

				/* mcu��ltc6803 spiͨ�Ŵ����� */
				if (ComErrCnt)
				{
					ComErrCnt--;
				}
				else 
				{
					g_SystemError.ltc_com = 1;
					if (g_ProtectDelayCnt > RELAY_ACTION_DELAY_1S)
					{
						g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;
					}
				}
			}
		}
		break;
	case AFE_VOLT_DETECT:
		DetectMaxMinAvgCellVolt();
		DetectCellsOverVolt();
		DetectCellsUnderVolt();
		DetectCellsVoltImba();
		DetectPackOv();
		DetectPackUv();    
		AfeState = AFE_TEMP_CNVT;//AFE_BALANCE;
		break;
		
	case AFE_TEMP_CNVT:
		Ltc6803_TempCnvt(TEMP_ALL);  //�����¶�ת��
		timeStamp = g_SysTickMs;  //��¼ת����ʼʱ��
		AfeState = AFE_READ_TEMP;  //״̬�л�
		break;

	case AFE_READ_TEMP:
        if(g_SysTickMs - timeStamp >= 10)
        {
            Ltc6803_ReadAllTemp((Ltc6803_Parameter *)g_ArrayLtc6803Unit);
            AfeState = AFE_BALANCE;// AFE_CAL_TEMP;
        }
		break;
		
	case AFE_BALANCE:
		//TskBlncMgt();
		AfeState = AFE_VOLT_CNVT;
		break;

	default:
		AfeState = AFE_VOLT_CNVT;
		break;
   }
}

//----------------------------------------------------------------------------
// Function    : TskCanMgt
// Description : ��CAN���߷��ͽ��ջ������Ĺ���
//				CAN�������Ƚ��뷢�ͽ��ջ�������Ȼ����д���
// Parameters  : 
// Returns     : 
//----------------------------------------------------------------------------

void TskCanMgt(void)
{
	TskCanProcessRxMsg();           // �����������
	CAN_BroadcastBufUpdate();       // ��Ҫ���͵Ĺ㲥���ݸ��µ����ͻ�����
	TskCanSendTxBufMsg();           // ���ͷ��ͻ���������
#if 0
	if (g_BatteryMode == CHARGE)
	{
		CAN_ChargerTskUpdate();
		CAN_ChargerTimeoutCheck();
	}	 
	else 
	{
		CAN_McsTskUpdate();
		CAN_McsTimeoutCheck();
	}

	CAN_TmpBoardTimeoutCheck();
#endif
}


//============================================================================
// Function    : TskAmbTempMgt
// Description : PCB����¶�
// Parameters  : none
// Returns     : 
//============================================================================
void TskAmbTempMgt(void)
{
	static uint8_t state = 0;

	switch(state)
	{
	case 0:  // start conversion 
		ADC_Convert(CHANNEL_TBAVAL);  
		while(ADCON0bits.GO);  //�ȴ�ת����ɣ���Լ��Ҫ15us
		g_AdcConvertValue.AmbTempRaw[g_AdcConvertValue.AmbTempIndex++] = ADC_GetConvertVal();

		// �ɼ���һ��ͼ���ƽ��ֵ
		if(g_AdcConvertValue.AmbTempIndex >= 8)
		{
			g_AdcConvertValue.AmbTempAvg = ADC_AverageCalculate(g_AdcConvertValue.AmbTempRaw); 
			g_AdcConvertValue.AmbTempIndex = 0;

			state = 1;  // ��һ���ڼ���ȫ������ֵ
		}
		break;

	case 1:
		g_BatteryParameter.AmbientTemp = ADCToTempVal(g_AdcConvertValue.AmbTempAvg);
		state = 0;
		break;
	default: 
		state = 0;
		break;
	}

	DetectPCBOverTemp();
}



//============================================================================
// Function    ��BatteryModeSwitch
// Description ��
// Parameters  ��none
// Returns     ��none
//============================================================================
void TskBatteryModeMgt(void)
{
	switch (g_BatteryMode)
	{
	case IDLE:  
		if ( (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)   // cell��ѹ����
			|| (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.ISO == WARNING_SECOND_LEVEL)
			|| (g_SystemError.all & 0x07))    
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if(GetChargeState() && (g_BatteryParameter.SOC <100)) // ���������
			{
				g_BatteryMode = CHARGE;
			}
            else
			{
				g_BatteryMode = PRECHARGE;
				g_PrechargeTimer =0;  
			}
		}
		break;

	case PRECHARGE:  //Ԥ���״̬
		if((g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBUT == WARNING_SECOND_LEVEL)
			|| (g_SystemError.all & 0x07))
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if(++g_PrechargeTimer > PRE_CHARGE_TIME)
			{
				g_BatteryMode = DISCHARGE;
				g_PrechargeTimer = 0;
			}
			else
			{
				g_BatteryMode = PRECHARGE;		
			}
		}
		break;

	case DISCHARGE:  //�ŵ�״̬
		if ((g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBUT == WARNING_SECOND_LEVEL) 
			|| (g_SystemError.all & 0x07))
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			g_BatteryMode = DISCHARGE;
			if ( DetectPackDischargeFinish() )
			{
				g_BatteryMode = PROTECTION;
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_20S;
			}
			if(GetChargeState())
			{
				g_BatteryMode = PROTECTION;
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;                    
			}  
		}
		break;

	
	case CHARGE:  //���״̬  
			if ( (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.PCBOT == WARNING_SECOND_LEVEL)
				|| (g_SystemWarning.PCBUT == WARNING_SECOND_LEVEL)  
				|| (g_SystemError.all & 0x07))
			{
				g_BatteryMode = PROTECTION;
			}
			else
			{
				if ( !GetChargeState() )  // ������ͷ�Ƿ�ε�
				{
					g_BatteryMode = IDLE;
				}
				else
				{
					g_BatteryMode = CHARGE;
				}
			}
			break;

	case HEATING:
		if ( (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBUT == WARNING_SECOND_LEVEL) 
			|| (g_SystemError.all & 0x07) )
		{
			g_BatteryMode = PROTECTION;
		}
		if ((g_BatteryParameter.CellTempMin > ChargeNeedHeatTemp) && GetChargeState())
		{
			g_BatteryMode = CHARGE;
		}
		else if((g_BatteryParameter.CellTempMin > ChargeNeedHeatTemp) 
				&& (!GetChargeState()))
		{
			g_BatteryMode = IDLE;
		}             
		   
		break;

	case COOLING:
		if ( (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBUT == WARNING_SECOND_LEVEL) 
			|| (g_SystemError.all & 0x07) )
		{
			g_BatteryMode = PROTECTION;
		}
		else if ((g_BatteryParameter.CellTempMax<ChargeNeedCoolTemp)
				&& GetChargeState())
		{
			g_BatteryMode = CHARGE;
		}
		else if((g_BatteryParameter.CellTempMax<DischargeNeedCoolTemp) 
				&&(!GetChargeState()))
		{
			g_BatteryMode = IDLE;
		}
		break;

	case PROTECTION:  //����״̬ 
		if((g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.COT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBOT == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.PCBUT == WARNING_SECOND_LEVEL)
			|| g_SystemError.all)
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if( GetChargeState() )  // ������ͷ�Ƿ�ε�
			{
				if(DetectPackChargeFinish())
					g_BatteryMode = PROTECTION;
				else	
					g_BatteryMode = CHARGE;
			}
			else
			{
				if(g_BatteryParameter.SOC > 0)
					g_BatteryMode = IDLE;
			}
		}
		
		break;

	default:  
		break;
    }
}




