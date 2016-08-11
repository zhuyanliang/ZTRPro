----本系统电池包设计
采集数据:
	1. 20节电芯电压（从模拟前端Ltc6803读取--SPI通信控制）
	2. 正极母线电流（电流传感器接入MUC的AD引脚）
	3. 四路温度数据（从模拟前端Ltc6803读取--SPI通信控制）
控制对象:
	1. 5路继电器（IO管脚输出控制:正极，负极，预充，风扇，降温）

部分数据通过CAN上报整机控制器

关键结构体与全局变量说明：
1.
volatile BatteryPackTypedef g_BatteryParameter;
// 定义电池包相关参数的结构体数据
typedef struct 
{
	uint16_t 			voltage;          		// 电池包总电压 0.1v/bit 
	volatile int16_t  	current;          		// 电池包母线电流 0.1A/bit 
	int16_t 			curr_pre;

	uint8_t  			SOC;          			// 电池包SOC (%)  
	uint16_t			socCyclesPerSec;		// 每秒计算SOC相关量的次数
	int16_t				Ah;						// 剩余安时数
	int32_t				Accumulator;			// 计算电量的累加器
	
	uint16_t 			SOH;					//充电循环次数
	int16_t 			sohCyclesPerSec;
	uint32_t			ChargedAh;
	uint16_t			sohCycleTimes;
	
	uint16_t 			CellVoltPre[CellsAmount];
	uint16_t 			CellRes[CellsAmount];
	uint16_t 			CellVoltMax;   			// 电池包最大单体电压
	uint16_t 			CellVoltMin;          	// 电池包最小单体电压
	uint16_t 			CellVoltAvg;          	// 电池包平均单体电压
	uint8_t  			MaxCellNum;				// 最大最小电压电芯编号
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
	int8_t   			AmbientTemp;          	// 板上环境温度
	uint16_t 			BalanceCellNum;       	// 指示哪几节cell在均衡
	uint8_t  			BalanceFlag;			// 是否处于均衡处理            
}BatteryPackTypedef;	