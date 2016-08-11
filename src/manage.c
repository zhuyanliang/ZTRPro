
#include "include.h" 

/*
 * 管理电压，电流，警报...
 * 管理功能函数模块
 * 独立于模块的全局变量的定义
 *
 */

volatile BatteryPackTypedef g_BatteryParameter;
BatteryModeTypedef 			g_BatteryMode;
SubModeTypedef 				g_BatterySubMode;			

uint16_t 					g_ProtectDelayCnt;	// 保护延时计数 
uint16_t 					g_PrechargeTimer;
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


	//以下是报警值设置，从EEPROM中读取，如果没有读取到或者读取错误，使用默认值
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

	EEPROM_ReadBlock(EEPROM_ADDR_DLV_THRHOLD, buff, 5);
	crc = calculate_crc8(buff, 4);

	if (crc == buff[4])
	{
		ptr = (uint8_t *)&g_CellIBThr.cls_1;

		for (i = 0; i < 5; i++)
		{
			*ptr++ = buff[i];
		}
	}
	else
	{
		g_CellIBThr = CellIBThrDefault; 
	}

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
		g_BatteryParameter.CellTemp[0] = ADCToTempVal((uint16_t)g_ArrayLtc6803Unit[0].Temp1);
		break;
	case 1:
		g_BatteryParameter.CellTemp[1] = ADCToTempVal((uint16_t)g_ArrayLtc6803Unit[0].Temp2); 
		break;
	case 2:
		g_BatteryParameter.CellTemp[2] = ADCToTempVal((uint16_t)g_ArrayLtc6803Unit[1].Temp1);
		break;
	case 3:
		g_BatteryParameter.CellTemp[3] = ADCToTempVal((uint16_t)g_ArrayLtc6803Unit[1].Temp2); 
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
#ifdef RELAY_TEST

	g_RelayActFlg.precharge = TRUE;
	g_RelayActFlg.positive = TRUE;
	g_RelayActFlg.negative = TRUE;

#else

	switch(g_BatteryMode)
	{
	case IDLE:
		g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.positive = FALSE;
		g_RelayActFlg.negative = FALSE;
		g_RelayActFlg.cooling = FALSE;
		break;

	case HEATING:
		g_RelayActFlg.heating = TRUE;
		g_RelayActFlg.cooling = FALSE;
		break;
		
	case PRECHARGE:  //预充电状态
		g_RelayActFlg.positive = FALSE;
		g_RelayActFlg.negative = TRUE;
		g_RelayActFlg.precharge = TRUE;
		break;

	case CHARGE:  //充电状态
		g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.cooling = TRUE;
		if (g_BatterySubMode == CHARGING)  //正在充电
		{
			g_RelayActFlg.positive = TRUE;
			g_RelayActFlg.negative = TRUE;
		}
		else                             //充电完成
		{
			g_RelayActFlg.positive = FALSE;
			g_RelayActFlg.negative = FALSE;
		}
		break;

	case DISCHARGE:  //放电状态
		g_RelayActFlg.cooling = TRUE;
		g_RelayActFlg.precharge = FALSE;
		g_RelayActFlg.positive = TRUE;
		g_RelayActFlg.negative = TRUE;
		break;

	case PROTECTION:  //保护状态：可能是故障保护、放电截止保护、充电截止保护等等 
		g_RelayActFlg.precharge = FALSE;

		if (!g_ProtectDelayCnt--)
		{              
			g_RelayActFlg.positive = FALSE;
			g_RelayActFlg.negative = FALSE;
		}
	default:
		break;
	}

#endif
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

	switch (state & 0x0001)
	{
	case 0:
		if (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cuv < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cuv++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_CUV, (uint8_t*)&g_FaultRecord.cuv, 2);
				Nvm_UpdateRecentFaultRec(0xE4);
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

	switch (state & 0x0002)
	{
	case 0:
		if (g_SystemWarning.COV == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cov < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cov++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_COV, (uint8_t*)&g_FaultRecord.cov, 2);
				Nvm_UpdateRecentFaultRec(0xE3);
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

	switch (state & 0x0004)
	{
	case 0:
		if (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.cib < FAULT_REC_LIMIT)
			{
				g_FaultRecord.cib++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_CIB, (uint8_t*)&g_FaultRecord.cib, 2);
				Nvm_UpdateRecentFaultRec(0xE5);	
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
				Nvm_UpdateRecentFaultRec(0xE7);
			}
			state |= 0x0008;
		}
		if (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.dut < FAULT_REC_LIMIT)
			{
				g_FaultRecord.dut++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_DUT, (uint8_t*)&g_FaultRecord.dut, 2);
				Nvm_UpdateRecentFaultRec(0xE7);
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
				Nvm_UpdateRecentFaultRec(0xE8);
			}
			state |= 0x0010;
		}
		if (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.dot < FAULT_REC_LIMIT)
			{
				g_FaultRecord.dot++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_DOT, (uint8_t*)&g_FaultRecord.dot, 2);
				Nvm_UpdateRecentFaultRec(0xE8);
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
				Nvm_UpdateRecentFaultRec(0xE1);
			}
			state |= 0x0020;
		}
		if (g_SystemWarning.DOC == WARNING_SECOND_LEVEL)
		{
			if (g_FaultRecord.doc < FAULT_REC_LIMIT)
			{
				g_FaultRecord.doc++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_DOC, (uint8_t*)&g_FaultRecord.doc, 2);
				Nvm_UpdateRecentFaultRec(0xE1);
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
				Nvm_UpdateRecentFaultRec(0xE0);	
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
		if (g_SystemError.ltc_com)
		{
			if (g_FaultRecord.ltc_com < FAULT_REC_LIMIT)
			{
				g_FaultRecord.ltc_com++;
				EEPROM_WriteBlock(EEPROM_ADDR_FAULT_LTCCOM, (uint8_t*)&g_FaultRecord.ltc_com, 2);
				Nvm_UpdateRecentFaultRec(0xE0);
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
#if (0)
   static uint8_t BalFlg = 0;
   static uint8_t n = 0;
   static uint16_t TimeStamp = 0;
   uint8_t i, j;

   if (BatteryMode == CHARGE)
   {
      if (getSysTickCounter() - TimeStamp > 5000)
      {
         BatteryUnit[0].CellBal = 0;
         BatteryUnit[1].CellBal = 0;

         if (((BatterySubMode == CHARGING)
          && (BatteryParameter.CellVoltMin 
                > CELL_BALANCE_OPEN_VOLT))
         || ((BatterySubMode == CHARGE_END)
          && (BatteryParameter.CellVoltMax 
                > CELL_BALANCE_PROTECT_VOLT)))
         {
            if (BatteryParameter.CellVoltMin 
                > CELL_BALANCE_OPEN_VOLT)
            {
               for (i=0; i<ModuleAmount; i++)
               {
                  for (j=0;j<(CellsAmount/ModuleAmount);j++)
                  {
                     if (BatteryUnit[i].CellVolt[j] - BatteryParameter.CellVoltMin 
                         > CELL_BALANCE_THRESHOLD)
                     {
                        BatteryUnit[i].CellBal |= 0x0001 << j;
                     }
                  }
               }
            }
         }

         Ltc6803_WriteCfgRegGroup(BatteryUnit);

         BalFlg = 1;
         TimeStamp = getSysTickCounter();
      }
   }
   else if (BalFlg == 1)
   {
      if (n++ < 3)
      {
         BatteryUnit[0].CellBal = 0;
         BatteryUnit[1].CellBal = 0;

         Ltc6803_WriteCfgRegGroup(BatteryUnit);
      }
      else
      {
         BalFlg = 0;
         n = 0;
      }
   }
#endif
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
	static uint32_t TimeStamp;
	static uint8_t ComErrCnt = 5;
	static AfeStateTypedef AfeState = AFE_CELL_VOLT_CNVT;
	uint8_t i;
	int16_t temp_dlt;
        
	switch (AfeState)
	{
	case AFE_CELL_VOLT_CNVT:
		Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_ALL);  //启动单体电压转换
		TimeStamp = g_SysTickMs;  //记录转换开始时间
		AfeState = AFE_READ_CELL_VOLT;  //状态切换
		break;

	case AFE_READ_CELL_VOLT:
		if (g_SysTickMs - TimeStamp >= 50)  // 转换完成需要20ms
		{
			if (Ltc6803_ReadAllCellVolt(g_ArrayLtc6803Unit))
			{
				AfeState = AFE_TEMP_CNVT;
				g_SystemError.ltc_com = 0;
				ComErrCnt = 5;
				
			}
			else
			{
				AfeState = AFE_CELL_VOLT_CNVT;

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

	case AFE_TEMP_CNVT:
		Ltc6803_TempCnvt(TEMP_ALL);  //启动温度转换
		TimeStamp = g_SysTickMs;  //记录转换开始时间
		AfeState = AFE_CAL_CELL_VOLT;  //状态切换
		break;

	case AFE_CAL_CELL_VOLT:
		DetectMaxMinAvgCellVolt();
		DetectCellsOverVolt();
		DetectCellsUnderVolt();
		DetectCellsVoltImba();
		DetectPackOv();
		DetectPackUv();    
		AfeState = AFE_READ_TEMP;//AFE_BALANCE;
		break;

	case AFE_BALANCE:
		TskBlncMgt();
		AfeState = AFE_READ_TEMP;
		break;

	case AFE_READ_TEMP:
		Ltc6803_ReadAllTemp(g_ArrayLtc6803Unit);
		AfeState = AFE_CELL_VOLT_CNVT;// AFE_CAL_TEMP;
		break;

	case AFE_CAL_TEMP:
		AfeState = AFE_CELL_VOLT_CNVT;
		break;

	default:
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
// Function    ：BatteryModeSwitch
// Description ：
// Parameters  ：none
// Returns     ：none
//============================================================================
void TskBatteryModeMgt(void)
{
	static uint32_t local_timer = 0;

	switch (g_BatteryMode)
	{
	case IDLE:  
		if ( (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)    // 过温保护
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)   // cell过压故障
			|| (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL) 
			|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.DOC == WARNING_SECOND_LEVEL)
			|| (g_SystemError.all & 0x07) )
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if ( GetChargeState() )
			{
				g_BatteryMode = CHARGE;
				if ( g_BatteryParameter.CellTempMin < 0)
				{
					g_BatteryMode = HEATING;
					g_BatterySubMode =  CHARGER_IN;
				}
			}
            else
			{
				g_BatteryMode = PRECHARGE;
				if ( g_BatteryParameter.CellTempMin < -18)
				{
					g_BatteryMode = HEATING;
					g_BatterySubMode =  RUNKEY_IN;
				}
				g_PrechargeTimer =0;                   
			}
		}
		break;

	case HEATING:
		if ( (g_SystemWarning.DOT == WARNING_SECOND_LEVEL)    // 过温保护
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)   // cell过压故障
			|| (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL) 
			|| (g_SystemError.all & 0x07) )
		{
			g_BatteryMode = PROTECTION;
		}
		else if (g_BatterySubMode ==  CHARGER_IN)
		{
			if (g_BatteryParameter.CellTempMin > 0)
			{
				g_BatteryMode = CHARGE;
			}
		}
		else if (g_BatterySubMode ==  RUNKEY_IN)
		{
			if (g_BatteryParameter.CellTempMin > -18)
			{
				g_BatteryMode = PRECHARGE;
			}                
		}    
		break;

	case PRECHARGE:  //预充电状态
		if (//(SystemWarning.COT == WARNING_SECOND_LEVEL)     // 过温保护
		//|| (SystemWarning.CUT == WARNING_SECOND_LEVEL)     // 低温保护
		//|| (SystemWarning.CUV == WARNING_SECOND_LEVEL)   // cell欠压故障
		(g_SystemWarning.COV == WARNING_SECOND_LEVEL)   // cell过压故障
		|| (g_SystemWarning.DOC == WARNING_SECOND_LEVEL)   // 过流保护
		|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
		|| (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
		|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)
		|| (g_SystemError.all & 0x07))
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if ( (++g_PrechargeTimer << 1) > PRE_CHARGE_TIME )
			{
				g_BatteryMode = DISCHARGE;
			}
		}
		break;

	case CHARGE:  //充电状态  
		if ((g_SystemWarning.COT == WARNING_SECOND_LEVEL)     // 过温保护
			|| (g_SystemWarning.CUT == WARNING_SECOND_LEVEL)     // 低温保护
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)    // cell过压故障
			|| (g_SystemWarning.COC == WARNING_SECOND_LEVEL)    // 过流保护
			|| (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL) 
			|| (g_SystemWarning.POV == WARNING_SECOND_LEVEL)
			|| (g_SystemError.all & 0x07))
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if ( !GetChargeState() )  // 检查充电插头是否拔掉
			{
				g_BatteryMode = IDLE;
				g_BatterySubMode = CHARGE_END;
			}
			else if ( DetectPackChargeFinish() )  //检测充电是否完成
			{
				g_BatterySubMode = CHARGE_END;
			}
			else
			{
				g_BatterySubMode = CHARGING;
			}
		}
		break;

	case DISCHARGE:  //放电状态
		if ((g_SystemWarning.DOT == WARNING_SECOND_LEVEL)     // 过温保护
			|| (g_SystemWarning.DUT == WARNING_SECOND_LEVEL)     // 低温保护
			//|| (g_SystemWarning.CUV == WARNING_SECOND_LEVEL)     // cell欠压故障
			|| (g_SystemWarning.COV == WARNING_SECOND_LEVEL)     // cell过压故障
			|| (g_SystemWarning.DOC == WARNING_SECOND_LEVEL)     // 过流保护    
			|| (g_SystemWarning.CIB == WARNING_SECOND_LEVEL)   
			|| (g_SystemWarning.TIB == WARNING_SECOND_LEVEL)    
			|| (g_SystemWarning.PUV == WARNING_SECOND_LEVEL)
			|| (g_SystemError.all & 0x07))
		{
			g_BatteryMode = PROTECTION;
		}
		else
		{
			if ( DetectPackDischargeFinish() )
			{
				g_BatteryMode = PROTECTION;
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_20S;
			}
			if (GetChargeState())
			{
				g_BatteryMode = PROTECTION;
				g_ProtectDelayCnt = RELAY_ACTION_DELAY_1S;                    
			}  
		}
		break;

	case PROTECTION:  //故障状态 
		if ( GetChargeState() )  // 检查充电插头是否拔掉
		{
			if (  (g_SystemWarning.COT != WARNING_SECOND_LEVEL) 
				&& (g_SystemWarning.CUT != WARNING_SECOND_LEVEL) 
				&& (g_SystemWarning.COC != WARNING_SECOND_LEVEL) 
				&& (g_SystemWarning.POV != WARNING_SECOND_LEVEL)
				&& (g_SystemWarning.COV != WARNING_SECOND_LEVEL)
				&& (g_SystemWarning.CIB != WARNING_SECOND_LEVEL)  
				&& (g_SystemWarning.TIB != WARNING_SECOND_LEVEL) )
			{
				g_BatteryMode = CHARGE;
			}
		}
		else
		{
			if ( (g_SystemWarning.DOT != WARNING_SECOND_LEVEL) 
				&& (g_SystemWarning.DUT != WARNING_SECOND_LEVEL) 
				&& (g_SystemWarning.DOC != WARNING_SECOND_LEVEL)
				&& (g_SystemWarning.PUV != WARNING_SECOND_LEVEL)
				&& (g_SystemWarning.COV != WARNING_SECOND_LEVEL) 
				&& (g_SystemWarning.CIB != WARNING_SECOND_LEVEL)  
				&& (g_SystemWarning.TIB != WARNING_SECOND_LEVEL) )
			{
				if ( g_BatteryParameter.SOC)
					g_BatteryMode = PRECHARGE;  
			}
		}
		break;
	default:  
		break;
    }
}










