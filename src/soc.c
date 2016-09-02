
#include "include.h"

/* ��̬״̬��(����30�������ϣ�30��)SOC�͵����ѹ���ұ� */
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
   g_BatteryParameter.SOC = 0;  // �����ϵ� SOC ��Ϊ 0%
   g_BatteryParameter.Ah = 0; 
   g_BatteryParameter.Accumulator = 0;
}



//============================================================================
// Function    : Soc_PowerOnAdjust
// Description : SOC�ϵ�У׼���ú�����ϵͳ�ϵ��ʱ����ã����ݵ����ѹֵͨ����
//               ��ȷ��pack�ĵ�ǰSOC��ϵͳ���������й������Դ�Ϊ�������е�����
//               �ֻ��ʵʱ��SOCֵ��
// Parameters  : none
// Return      : none
//============================================================================
void Soc_PowerOnAdjust(void)
{
	uint8_t i;
	uint8_t soc_frmVol;
	uint8_t soc_frmEerom;

	/* ���������ѹת�� */
	Ltc6803_CellVoltCnvt(STCVAD_CMD, CELL_ALL);  
	DelayMs(15);

	// ��ȡ���е�о��ѹ
	if (Ltc6803_ReadAllCellVolt((Ltc6803_Parameter *)g_ArrayLtc6803Unit))
	{
		DetectMaxMinAvgCellVolt();
	}

	/* �ϵ���ݵ����ѹ�������ϵͳ�ĵ�ǰsocֵ */
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
		// SOC �� 0~100 ֮��
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

	/* ��eeprom�ж�ȡ�ϵ�֮ǰ�洢��Ahֵ��ת����SOC*/
	g_BatteryParameter.Ah = Soc_ReadAh();
	soc_frmEerom = (uint32_t)g_BatteryParameter.Ah * 100 / BATTERY_CAPACITY_TOTAL; 

	/* �Ƚϴ洢soc���ѹУ׼soc,�����ֵ�������Ե�ѹУ׼socΪ׼ */
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
// Description : ���µ�ذ�SOC��ֵ
// Parameters  : None
// Return      : None
//============================================================================
void Soc_Update (void)
{
	static uint32_t timeStamp=0;

	/* ��ŵ����½�ֹ��SOCУ׼, ע���У׼һ��Ҫ��ϵͳ״̬ */
	/* �л�ǰ��ɣ�����������佫�޷��õ�ִ�С�            */
	if((g_BatteryMode == CHARGE) && (DetectPackChargeFinish()))  // �����ֹ��У׼
	{
		g_BatteryParameter.Ah = BATTERY_CAPACITY_TOTAL;
		g_BatteryParameter.SOC = 100;
	}
	else if((g_BatteryParameter.CellVoltMin <= CELL_DISCHARGE_END_VOLT)  // �ŵ���ֹ��У׼
				&& (g_BatteryMode == DISCHARGE))
	{
		g_BatteryParameter.Ah = 0;
		g_BatteryParameter.SOC= 0;
	}

	/* ÿ��SOC_UPDATE_PERIOD_MS���ڸ���һ��SOCֵ Ŀǰ�ݶ�4s = 2Ms * SOC_UPDATE_PERIOD_MS */
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

		g_BatteryParameter.SOC = (uint32_t)g_BatteryParameter.Ah * 100 / BATTERY_CAPACITY_TOTAL;   // ���Ϊ�ٷֱ�

		timeStamp = g_SysTickMs;
	}

}


//============================================================================
// Function    : Soc_AhAcc
// Description : ��ʱ�����ۼӣ��ú�����2.0ms��ʱ���е���
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
// Description : �ۼ�������0.1Ah���1��g_BatteryParameter.Ah��
// Parameters  : none
// Return      : none
//============================================================================
void Soc_CalculateAh(void)
{
	// 2Ms�ۼ�һ��Ah  1Ah = 60*60*1000Ms*I ��������˼��
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
// Description : �洢ϵͳ��ǰSOC��ز�����EEPROM��,
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
// Description : ��EEPROM�ж�ȡSOCֵ
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
// Description : ��EEPROM�ж�ȡSoc.Accumulatorֵ
// Parameters  : none
// Return      : 
//============================================================================
int32_t Soc_ReadAcc(void)
{
	int32_t tmp;

	EEPROM_ReadBlock(EEPROM_ADDR_ACC, (uint8_t*)&tmp, 4);

	if (ABS(tmp) > 1800000)	// 1800000 ��Ϊÿ��2ms�ɼ�һ��
	{
		tmp = 0;
	}

	return tmp;
}
















