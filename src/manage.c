
#include "include.h" 

/*
 * 管理电压，电流，警报...
 * 管理功能函数模块
 * 独立于模块的全局变量的定义
 *
 */

volatile BatteryPackTypedef g_BatteryParameter;
volatile BatteryModeTypedef g_BatteryMode = IDLE;			

uint16_t 					g_ProtectDelayCnt;	// 保护延时计数 
uint16_t 					g_PrechargeTimer = 0;
uint8_t 					g_EnterLowPoweModeFlg;

//----------------------------------------------------------------------------
// Function    : TskAfe_Init
// Description : 电池部分参数的初始化
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

	// 读取充电单体过充警报等级
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

	
	// 读取单体放电低压报警等级
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

	// 读取充电过温警报等级
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

	//读取充电低温报警等级
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
    
	// 读取电池包放电高温
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

	// 读取电池包放电低温	
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

	// 读取电池包充电过流报警等级
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
	// 读取电池包放电过流报警等级
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

    // 读取电池包温差等级报警
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

	// 读取电池包过压等级
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
	
	// 读取电池包低压等级
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
	
	// 读取单体非一致性等级
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
// Description : 电池包母线电流计算
// Parameters  : none
// Returns     : 
//============================================================================
void TskCurrentMgt(void)
{
	uint8_t i;
	static uint8_t curr_flg = 0;

	ADC_Convert(CHANNEL_IHIGH);  
	
	while(ADCON0bits.GO);  //等待转换完成，大约需要15us
	//第一次启动时，一次性读取8个电流值
	if (!curr_flg)
	{
		for ( i = 0; i < 8; i++ )
		{
			g_AdcConvertValue.CurHighRaw[i] = ADC_GetCvtRaw();
		} 
		curr_flg = 1;    
	}
	else //以后每次更新一个电压值
	{ 
		g_AdcConvertValue.CurHighRaw[g_AdcConvertValue.CurHighIndex++ & 0x07] = ADC_GetCvtRaw();
	}
	
	g_AdcConvertValue.CurHighAvg = ADC_AverageCalculate(g_AdcConvertValue.CurHighRaw);

	//根据ADC采样值，结合电流采集器的特性，获取真实的电流值
	g_BatteryParameter.current = (int16_t)((((int32_t)g_AdcConvertValue.CurHighAvg * 6250) >> 12) - 3125) - g_CurrentOffset;
	//g_BatteryParameter.current = (int16_t)((g_AdcConvertValue.CurHighAvg-g_CurrentOffset)/3276.0 * 5000); 
	//g_BatteryParameter.current = (int16_t)((g_AdcConvertValue.CurHighAvg-g_CurrentOffset)/4096.0 * 5000); 
	//g_BatteryParameter.current -= 2500;
	
	//检查电池包电流是否超过限定值
	DetectPackOverCurrent();
}


//============================================================================
// Function    : 温度处理
// Description : 四个通道的温度值更新完成后才进行一次温度检测
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
		// 检测温度值，并得到单体最大最小温度值
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
// Description : 对电池SOC的管理，涉及到Ah,Soc,Accumulator的存储
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
// Function    ：TskRelayMgt
// Description ：该函数根据pack的状态决定相应继电器的开关标志状态
// Parameters  ：none 
// Returns     ：none
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

	case PRECHARGE:  //预充电状态
		g_RelayActFlg.positive = FALSE;
		g_RelayActFlg.negative = TRUE;
		g_RelayActFlg.precharge = TRUE;
		break;

	case DISCHARGE:  //放电状态
		if(g_BatteryParameter.CellTempAvg >= (DischargeNeedCoolTemp-5))
			g_RelayActFlg.cooling = TRUE;
		else
			g_RelayActFlg.cooling = FALSE;
		g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.positive = TRUE;
		g_RelayActFlg.negative = TRUE;
		break;
	
	case CHARGE:  //充电状态
		//if(g_BatteryParameter.CellTempAvg >= (ChargeNeedCoolTemp-5))
			//g_RelayActFlg.cooling = TRUE;
		//else
			//g_RelayActFlg.cooling = FALSE;	
		g_RelayActFlg.precharge = FALSE;
		if(g_BatteryParameter.SOC > 99) // 充电完成
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

	case PROTECTION:  //保护状态：可能是故障保护、放电截止保护、充电截止保护等等 
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
// Description : 对错误信息进行管理
// Parameters  : none
// Returns     : none
//============================================================================
void TskFaultStoreMgt(void)
{
	static uint16_t state = 0;

	switch(state & 0x0001) // 充电低压
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

	switch (state & 0x0002) // 充电过压
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

	switch (state & 0x0004)	// 单体一致性
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
// Function    : TskAfeMgt 模拟前端 analog front end
// Description : 该函数实现LTC6803-3的启动转换、读取数据、数据计算、均衡等功能
//               实现和状态切换
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
		Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_ALL);  //启动单体电压转换
		timeStamp = g_SysTickMs;  //记录转换开始时间
		AfeState = AFE_READ_VOLT;  //状态切换
		break;

	case AFE_READ_VOLT:
		if (g_SysTickMs - timeStamp >= 10)  // 转换完成需要20ms
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

				/* mcu与ltc6803 spi通信错误检测 */
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
		Ltc6803_TempCnvt(TEMP_ALL);  //启动温度转换
		timeStamp = g_SysTickMs;  //记录转换开始时间
		AfeState = AFE_READ_TEMP;  //状态切换
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
// Description : 对CAN总线发送接收缓冲区的管理
//				CAN数据首先进入发送接收缓冲区，然后进行处理
// Parameters  : 
// Returns     : 
//----------------------------------------------------------------------------

void TskCanMgt(void)
{
	TskCanProcessRxMsg();           // 处理接收数据
	CAN_BroadcastBufUpdate();       // 将要发送的广播数据更新到发送缓冲区
	TskCanSendTxBufMsg();           // 发送发送缓冲区数据
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
// Description : PCB板的温度
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
		while(ADCON0bits.GO);  //等待转换完成，大约需要15us
		g_AdcConvertValue.AmbTempRaw[g_AdcConvertValue.AmbTempIndex++] = ADC_GetConvertVal();

		// 采集满一组就计算平均值
		if(g_AdcConvertValue.AmbTempIndex >= 8)
		{
			g_AdcConvertValue.AmbTempAvg = ADC_AverageCalculate(g_AdcConvertValue.AmbTempRaw); 
			g_AdcConvertValue.AmbTempIndex = 0;

			state = 1;  // 下一周期计算全部采样值
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
// Function    ：BatteryModeSwitch
// Description ：
// Parameters  ：none
// Returns     ：none
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
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)   // cell过压故障
			|| (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.ISO == WARNING_SECOND_LEVEL)
			|| (g_SystemError.all & 0x07))    
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if(GetChargeState() && (g_BatteryParameter.SOC <100)) // 充电器接入
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

	case PRECHARGE:  //预充电状态
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

	case DISCHARGE:  //放电状态
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

	
	case CHARGE:  //充电状态  
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
				if ( !GetChargeState() )  // 检查充电插头是否拔掉
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

	case PROTECTION:  //保护状态 
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
			if( GetChargeState() )  // 检查充电插头是否拔掉
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




