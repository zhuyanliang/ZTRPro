----��ϵͳ��ذ����
�ɼ�����:
	1. 20�ڵ�о��ѹ����ģ��ǰ��Ltc6803��ȡ--SPIͨ�ſ��ƣ�
	2. ����ĸ�ߵ�������������������MUC��AD���ţ�
	3. ��·�¶����ݣ���ģ��ǰ��Ltc6803��ȡ--SPIͨ�ſ��ƣ�
���ƶ���:
	1. 5·�̵�����IO�ܽ��������:������������Ԥ�䣬���ȣ����£�

��������ͨ��CAN�ϱ�����������

�ؼ��ṹ����ȫ�ֱ���˵����
1.
volatile BatteryPackTypedef g_BatteryParameter;
// �����ذ���ز����Ľṹ������
typedef struct 
{
	uint16_t 			voltage;          		// ��ذ��ܵ�ѹ 0.1v/bit 
	volatile int16_t  	current;          		// ��ذ�ĸ�ߵ��� 0.1A/bit 
	int16_t 			curr_pre;

	uint8_t  			SOC;          			// ��ذ�SOC (%)  
	uint16_t			socCyclesPerSec;		// ÿ�����SOC������Ĵ���
	int16_t				Ah;						// ʣ�లʱ��
	int32_t				Accumulator;			// ����������ۼ���
	
	uint16_t 			SOH;					//���ѭ������
	int16_t 			sohCyclesPerSec;
	uint32_t			ChargedAh;
	uint16_t			sohCycleTimes;
	
	uint16_t 			CellVoltPre[CellsAmount];
	uint16_t 			CellRes[CellsAmount];
	uint16_t 			CellVoltMax;   			// ��ذ�������ѹ
	uint16_t 			CellVoltMin;          	// ��ذ���С�����ѹ
	uint16_t 			CellVoltAvg;          	// ��ذ�ƽ�������ѹ
	uint8_t  			MaxCellNum;				// �����С��ѹ��о���
	uint8_t  			MinCellNum;
	int8_t   			CellTemp[8];          	// cell temperature   
	int8_t   			CellTempMax;          
	int8_t   			CellTempMin;   
	int8_t  			CellTempAvg; 
	uint8_t 			MaxTempNum;
	uint8_t 			MinTempNum; 
	uint8_t 			MaxTempModuIdx;
	uint8_t 			MinTempModuIdx;
	uint8_t 			MaxTempChnIdx;
	uint8_t 			MinTempChnIdx;     
	int8_t   			AmbientTemp;          	// ���ϻ����¶�
	uint16_t 			BalanceCellNum;       	// ָʾ�ļ���cell�ھ���
	uint8_t  			BalanceFlag;			// �Ƿ��ھ��⴦��            
}BatteryPackTypedef;	