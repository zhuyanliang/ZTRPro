----本系统电池包设计
采集数据:
	1. 20节电芯电压（从模拟前端Ltc6803读取--SPI通信控制）
	2. 正极母线电流（电流传感器接入MUC的AD引脚）
	3. 四路温度数据（从模拟前端Ltc6803读取--SPI通信控制）
控制对象:
	1. 5路继电器（IO管脚输出控制:正极，负极，预充，风扇，降温）

部分数据通过CAN上报整机控制器


引脚功能说明：

A0 AN0:绝缘性检测
A1:保留
A2:Relay NEG+ 负极继电器控制机端口
A3 AN3:TBAVAL PCB板子的温度采集，模拟量
A5:ISO_PWM 绝缘检测产生PWM波形
A6:OUTPUT3						？？
A7:Heater Control 控制加温装置启停端口

B0:PreCHA+ 预充继电器控制
B1 AN8:V12VD  检测电压是否为12V
B2:CANTX	CAN通信端口
B3:CANRX
B4 AN9:检测电压是否为5V    	
B5:FAN Control 风扇控制引脚
B6:ICD PGC
B7:ICD PGD

C0:Relay POS+ 正极继电器控制端口
C1:OUTPUT4				？？
C2:BSSB 与LTC6803之间SPI通信的片选信号
C3:SCK SPI时钟输出
C4:SDI SPI数据输入
C5:SDO SPI数据输出
C6:LDGRNB 红灯控制引脚 0-点亮 1-熄灭
C7:LDREDB 绿灯控制引脚 0-点亮 1-熄灭

D0:TH1EN   热敏电阻1使能端口
D1:TH2EN   热敏电阻2使能端口
D2:MCUcoderOUT     暂时不用
D3:MCUcoderIN      暂时不用
D4:KEYRUN 钥匙开关
D5:BusRelease     暂时不用
D6:INPUT1         ？？
D7:ChargeIN  充电继电器控制端口

E0 AN5:ILOW  电流传感器的输入模拟量
E1 AN6:IHIGH 电流传感器的输入模拟量
E2 AN7:THXVAL 外接热敏电阻（模拟量输入）配合TH1EN TH2EN使用
E3:ICD VPP

// 2016/9/6 记得添加存储数据前的掉电检测
        
// 后续工作：添加绝缘检测

2016/10/12
VDIF TDIF POV PUV //出现的次数没有添加 ，以0代替

2016/10/27 需要要进行的测试:
1.保护延时时间整体测试
2.放电过程中接入充电器
3.包、电芯低压上电测试
4.充放电过程中、6803突然损坏测试
5.CAN整车、充电器中断测试

更能增加:
10min无操作,BMS休眠，停止放电









