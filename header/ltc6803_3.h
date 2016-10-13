/* 
 * File:   ltc6803.h
 * Author: Administrator
 *
 * Created on 2016年6月30日, 下午1:22
 */

#ifndef LTC6803_3_H
#define	LTC6803_3_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "include.h"  
//================================ Macro Definition =================================//


// 相关接口的宏定义，根据具体应用做相应修改
#define  Ltc6803_ChipSelect()         {LATCbits.LATC2 = 0;Nop();Nop();Nop();Nop();}
#define  Ltc6803_ChipUnselect()       {Nop();Nop();Nop();Nop();LATCbits.LATC2 = 1;}
//#define  SPI_ReceiveByte()            Spi1ReadByte()
//#define  SPI_SendBlock(x,y)           Spi1WriteBlock(x,y)
//#define  SPI_ReceiveBlock(x,y)        Spi1ReadBlock(x,y)



//================================== Command Codes ==================================//
#define  WRCFG_CMD             ((uint8_t)0x01)               // Write config registers
#define  RDCFG_CMD             ((uint8_t)0x02)               // Read config registers
#define  RDCV_CMD              ((uint8_t)0x04)               // Read cell voltage registers
#define  RDCVA_CMD             ((uint8_t)0x06)               // Read cell voltage 1-4
#define  RDCVB_CMD             ((uint8_t)0x08)               // Read cell voltage 5-8
#define  RDCVC_CMD             ((uint8_t)0x0A)               // Read cell voltage 9-12
#define  RDFLG_CMD             ((uint8_t)0x0C)               // Read flag registers
#define  RDTMP_CMD             ((uint8_t)0x0E)               // Read temp. registers
#define  STCVAD_CMD            ((uint8_t)0x10)               // Start CV ADC / Poll
#define  STOWAD_CMD            ((uint8_t)0x20)               // Start OpenWire CV ADC /Poll
#define  STTMPAD_CMD           ((uint8_t)0x30)               // Start Temp. ADC / Poll
#define  PLADC_CMD             ((uint8_t)0x40)               // Poll ADC status
#define  PLINT_CMD             ((uint8_t)0x50)               // Poll Interrupt status
#define  DAGN_CMD              ((uint8_t)0x52)               // Start Diagnose and Poll Status
#define  RDDGNR_CMD            ((uint8_t)0x54)               // Read Diagnostic Register
#define  STCVDC_CMD            ((uint8_t)0x60)               // Start CV ADC /Poll with Discharge Permitted
#define  STOWDC_CMD            ((uint8_t)0x70)               // Start OpenWire CV ADC /Poll with Discharge Permitted


//================================ Cell definitions  ================================//
#define  CELL_ALL              ((uint8_t)0x00)               // All cells
#define  CELL_01               ((uint8_t)0x01)               // Cell 1 only
#define  CELL_02               ((uint8_t)0x02)               // Cell 2 only
#define  CELL_03               ((uint8_t)0x03)               // Cell 3 only
#define  CELL_04               ((uint8_t)0x04)               // Cell 4 only
#define  CELL_05               ((uint8_t)0x05)               // Cell 5 only
#define  CELL_06               ((uint8_t)0x06)               // Cell 6 only
#define  CELL_07               ((uint8_t)0x07)               // Cell 7 only
#define  CELL_08               ((uint8_t)0x08)               // Cell 8 only
#define  CELL_09               ((uint8_t)0x09)               // Cell 9 only
#define  CELL_10               ((uint8_t)0x0A)               // Cell 10 only
#define  CELL_11               ((uint8_t)0x0B)               // Cell 11 only (Cell10 bit=0)
#define  CELL_12               ((uint8_t)0x0C)               // Cell 12 only (Cell10 bit=0)
#define  CLEAR                 ((uint8_t)0x0D)               // clear command
#define  CELL_TEST1            ((uint8_t)0x0E)               // Test-pattern 1
#define  CELL_TEST2            ((uint8_t)0x0F)               // Test-pattern 2 


//============================= Temperature definitions  ============================//
#define  TEMP_ALL              ((uint8_t)0x00) 
#define  TEMP_01               ((uint8_t)0x01)
#define  TEMP_02               ((uint8_t)0x02)
#define  TEMP_IN               ((uint8_t)0x03)
#define  TEMP_TEST1            ((uint8_t)0x0E)
#define  TEMP_TEST2            ((uint8_t)0x0F)


//===================================================================================//
#define  _10_CELL_MODE          ((uint8_t)0x04)
#define  _12_CELL_MODE          ((uint8_t)0x00)

//===================================================================================//

typedef struct 
{
   struct 
   {
      unsigned CDC   :3;
      unsigned CELL10:1;
      unsigned LVLPL :1;
      unsigned GPIO1 :1;
      unsigned GPIO2 :1;
      unsigned WDT   :1;
   }GenrCfg;

   uint16_t CellBal;
   uint16_t CellMask;
   uint8_t  UndVoltThre;
   uint8_t  OveVoltThre;
   int16_t  CellVolt[12]; 
   uint16_t Temp1;
   uint16_t Temp2;
   uint16_t Itemp;
   uint8_t  UndVoltFlg;
   uint8_t  OveVoltFlg;
   uint16_t Vref;
}Ltc6803_Parameter;

extern Ltc6803_Parameter g_ArrayLtc6803Unit[ModuleAmount]; //Ltc6803管理芯片参数

//=============================== Function Prototype  ===============================//
void Ltc6803_Init(void);

uint8_t Ltc6803_BlockCrc8Cal(uint8_t *CrcByte, uint8_t len);
//uint8_t Ltc6803_ReadCfgRegGroup(uint8_t *CfgData);
void Ltc6803_WriteCfgRegGroup(Ltc6803_Parameter *Dev);
void Ltc6803_CellVoltCnvt(uint8_t ConvertMode, uint8_t channel);
uint8_t Ltc6803_ReadAllCellVolt(Ltc6803_Parameter *Dev);
int16_t Ltc6803_CellVoltCal(int16_t VoltRaw);
void Ltc6803_TempCnvt(uint8_t channel);
//uint8_t Ltc6803_ReadTempRegGroup(uint8_t *data);
uint8_t Ltc6803_ReadAllTemp(Ltc6803_Parameter *Dev);
//uint8_t Ltc6803_ReadFlagRegister(Ltc6803_Parameter *Dev);
uint8_t Ltc6803_SetBalanceSwitch(uint16_t cells);



#ifdef	__cplusplus
}
#endif

#endif	/* LTC6803_H */

