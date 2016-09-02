
#include "include.h"

/* 静态状态下(静置30分钟以上，30℃)SOC和单体电压查找表 */
const SocLookupTypeDef Soc_Volt_1C_Table[] = 
{
	{4080, 100}, {4040, 95}, {4005, 90}, {3978, 85},
	{3931,  80}, {3886, 75}, {3843, 70}, {3795, 65},
	{3730,  60}, {3696, 55}, {3666, 50}, {3648, 45},
	{3630,  40}, {3612, 35}, {3592, 30}, {3552, 25},
	{3502,  20}, {3474, 15}, {3418, 10}, {3232,  5},
	{3170,   0}
};

#define SOC_TABLE_NUM       ((uint8_t)21)


//============================================================================
// Function    : Soc_Init
// Description : 
// Parameters  : None
// Return      : None
//============================================================================
void Soc_Init(void)
{
   g_BatteryParameter.socCyclesPerSec = SOC_SAMPLE_CYCLES_PER_SECOND;
   g_BatteryParameter.current = 0;
   g_BatteryParameter.SOC = 0;  // 初次上电 SOC 设为 0%
   g_BatteryParameter.Ah = 0; 
   g_BatteryParameter.Accumulator = 0;
}



//============================================================================
// Function    : Soc_PowerOnAdjust
// Description : SOC上电校准，该函数在系统上电的时候调用，根据单体电压值通过查
//               表确定pack的当前SOC，系统在整个运行过程中以此为基础进行电流积
//               分获得实时的SOC值。
// Parameters  : none
// Return      : none
//============================================================================
void Soc_PowerOnAdjust(void)
{
	uint8_t i;
	uint8_t soc_frmVol;
	uint8_t soc_frmEerom;

	/* 启动单体电压转换 */
	Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_ALL);  
	DelayMs(15);

	// 获取所有电芯电压
	if (Ltc6803_ReadAllCellVolt((Ltc6803_Parameter *)g_ArrayLtc6803Unit))
	{
		DetectMaxMinAvgCellVolt();
	}

	/* 上电根据单体电压查表计算出系统的当前soc值 */
	if (g_BatteryParameter.CellVoltMax >= Soc_Volt_1C_Table[0].cell_volt)
	{
		soc_frmVol = 100;
	}
	else if (g_BatteryParameter.CellVoltMin <= Soc_Volt_1C_Table[20].cell_volt)
	{
		soc_frmVol = 0;
	}
	else
	{
		// SOC 在 0~100 之间
		for (i=0; i<SOC_TABLE_NUM; i++)
		{
			if (g_BatteryParameter.CellVoltMin >= Soc_Volt_1C_Table[i].cell_volt)
			{
				soc_frmVol = Soc_Volt_1C_Table[i].soc + (g_BatteryParameter.CellVoltMin 
				                    - Soc_Volt_1C_Table[i].cell_volt) * 5
				                   / (Soc_Volt_1C_Table[i-1].cell_volt
				                    - Soc_Volt_1C_Table[i].cell_volt);            
				break;
			}
		}
	}

	/* 从eeprom中读取断电之前存储的Ah值，转换成SOC*/
	g_BatteryParameter.Ah = Soc_ReadAh();
	soc_frmEerom = (uint32_t)g_BatteryParameter.Ah * 100 / BATTERY_CAPACITY_TOTAL; 

	/* 比较存储soc与电压校准soc,如果差值过大，则以电压校准soc为准 */
	if (ABS_DIFF(soc_frmVol, soc_frmEerom) > 10)
	{
		g_BatteryParameter.Ah = (uint32_t)soc_frmVol * BATTERY_CAPACITY_TOTAL / 100;
		g_BatteryParameter.Accumulator = 0;
		g_BatteryParameter.SOC = soc_frmVol ;
	}
	else 
	{
		g_BatteryParameter.SOC = soc_frmEerom;
		if (g_BatteryParameter.SOC)
		{
			g_BatteryParameter.Accumulator = Soc_ReadAcc();
		}
		else
		{
			g_BatteryParameter.Accumulator = 0;
		} 
	}
}


//============================================================================
// Function    : Soc_Update
// Description : 更新电池包SOC数值
// Parameters  : None
// Return      : None
//============================================================================
void Soc_Update (void)
{
	static uint32_t timeStamp=0;

	/* 充放电上下截止点SOC校准, 注意该校准一定要在系统状态 */
	/* 切换前完成，否则以下语句将无法得到执行。            */
	if((g_BatteryMode == CHARGE) && (DetectPackChargeFinish()))  // 充电上止点校准
	{
		g_BatteryParameter.Ah = BATTERY_CAPACITY_TOTAL;
		g_BatteryParameter.SOC = 100;
	}
	else if((g_BatteryParameter.CellVoltMin <= CELL_DISCHARGE_END_VOLT)  // 放电下止点校准
				&& (g_BatteryMode == DISCHARGE))
	{
		g_BatteryParameter.Ah = 0;
		g_BatteryParameter.SOC= 0;
	}

	/* 每个SOC_UPDATE_PERIOD_MS周期更新一次SOC值 目前暂定4s = 2Ms * SOC_UPDATE_PERIOD_MS */
	if(g_SysTickMs - timeStamp >= SOC_UPDATE_PERIOD_MS)
	{
		if (g_BatteryParameter.Ah > BATTERY_CAPACITY_TOTAL)
		{
			g_BatteryParameter.Ah = BATTERY_CAPACITY_TOTAL;
		}
		else if (g_BatteryParameter.Ah < 0)
		{
			g_BatteryParameter.Ah = 0;
		}

		g_BatteryParameter.SOC = (uint32_t)g_BatteryParameter.Ah * 100 / BATTERY_CAPACITY_TOTAL;   // 结果为百分比

		timeStamp = g_SysTickMs;
	}

}


//============================================================================
// Function    : Soc_AhAcc
// Description : 安时积分累加，该函数在2.0ms定时器中调用
// Parameters  : none
// Return      : none
//============================================================================
void Soc_AhAcc(void)
{
   if((g_BatteryMode == CHARGE) || (g_BatteryMode == DISCHARGE))
   {
	   if ( (g_BatteryParameter.current > 3) || (g_BatteryParameter.current < -3) )
	   {
	      g_BatteryParameter.Accumulator += g_BatteryParameter.current;
	   }
	}
}


//============================================================================
// Function    : Soc_CalculateWh
// Description : 累加器中满0.1Ah则进1到g_BatteryParameter.Ah中
// Parameters  : none
// Return      : none
//============================================================================
void Soc_CalculateAh(void)
{
	// 2Ms累加一次Ah  1Ah = 60*60*1000Ms*I 电流积分思想
	// 1Ah = 1A*1h = I1+I2+I3+...+I1800000 
	if (g_BatteryParameter.Accumulator > 1800000)
	{
		g_BatteryParameter.Accumulator -= 1800000;

		if (g_BatteryParameter.SOC >= 1)
		{
			g_BatteryParameter.Ah -= 1;
		}
	}
	else if (g_BatteryParameter.Accumulator < -1800000)
	{
		g_BatteryParameter.Accumulator += 1800000;

		if (g_BatteryParameter.SOC < 99) 
		{     
			g_BatteryParameter.Ah += 1;
		}
	}
}


//============================================================================
// Function    : Soc_StoreSoc
// Description : 存储系统当前SOC相关参数到EEPROM中,
// Parameters  : none
// Return      : none
//============================================================================
void Soc_StoreSoc(void)
{
	static uint8_t cnt = 0;
	static uint8_t socSt = 0;
	uint8_t temp; 
	if (++cnt < 200)
	{
		return;
	}

	switch(socSt++)
	{
	case 0:
		temp = (uint8_t)g_BatteryParameter.Ah;
        EEPROM_WriteByte(EEPROM_ADDR_SOC, temp);
        break;
    case 1:
        temp = (uint8_t)(g_BatteryParameter.Ah >> 8);    
        EEPROM_WriteByte(EEPROM_ADDR_SOC+1, temp);
        break;
    case 2:
        temp = (uint8_t)(g_BatteryParameter.Accumulator);
        EEPROM_WriteByte(EEPROM_ADDR_ACC, temp);
        break;
    case 3:
        temp = (uint8_t)(g_BatteryParameter.Accumulator >> 8);
        EEPROM_WriteByte(EEPROM_ADDR_ACC+1, temp);
        break;
    case 4:
        temp = (uint8_t)(g_BatteryParameter.Accumulator >> 16);
        EEPROM_WriteByte(EEPROM_ADDR_ACC+2, temp);
        break;
    case 5:
        temp = (uint8_t)(g_BatteryParameter.Accumulator >> 24);
        EEPROM_WriteByte(EEPROM_ADDR_ACC+3, temp);
        socSt = 0;
        cnt = 0;
        break;
    default:
        cnt = 0;
        socSt = 0;
        break;
	}
}


//============================================================================
// Function    : Soc_ReadAh
// Description : 从EEPROM中读取SOC值
// Parameters  : none
// Return      : soc
//============================================================================
int16_t Soc_ReadAh(void)
{
	int16_t tmp;

	EEPROM_ReadBlock(EEPROM_ADDR_SOC, (uint8_t*)&tmp, 2);

	if (ABS(tmp) > BATTERY_CAPACITY_TOTAL)
	{
		tmp = 0;
	}

	return tmp;
}



//============================================================================
// Function    : Soc_ReadAcc
// Description : 从EEPROM中读取Soc.Accumulator值
// Parameters  : none
// Return      : 
//============================================================================
int32_t Soc_ReadAcc(void)
{
	int32_t tmp;

	EEPROM_ReadBlock(EEPROM_ADDR_ACC, (uint8_t*)&tmp, 4);

	if (ABS(tmp) > 1800000)	// 1800000 因为每隔2ms采集一次
	{
		tmp = 0;
	}

	return tmp;
}
















