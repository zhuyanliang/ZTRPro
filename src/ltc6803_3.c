//=======================================================================================
//
//              Copyright (c) 2013, Globetools. All rights reserved.
//
//
// * File Name   : ltc6803_3.c
//
// * Author      : 
//
// * Date        : 
//
// * Version     ：v1.0
//
// * Copyright   : 
//
// * Description : 电池组监测芯片ltc6803-1、ltc6803-3底层驱动程序，该驱动程序为多片6803
//                 菊花链形式级联下的驱动。
//
//=======================================================================================

#include "include.h"

//================  CRC8 查找表，用于通信中CRC计算 ================ //
static uint8_t const Crc8Table[256] = 
{
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
    0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
    0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
    0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
    0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
    0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
    0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
    0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
    0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
    0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
    0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
    0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
    0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
    0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
    0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
    0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};

volatile Ltc6803_Parameter g_ArrayLtc6803Unit[ModuleAmount];

//===========  定义ltc6803寄存器结构体，主要用于通信缓存 =========== //
static struct
{
	uint8_t cfgr[6];
	uint8_t cvr[18];
	uint8_t flgr[3];
	uint8_t tmpr[5];
	uint8_t dgnr[2];
	uint8_t pec;
}RegStr;

//============================================================================
// Function    : Ltc6803_ByteCrc8Cal
// Description : this function calculate the packet error code(PEC)
// Parameters  : CrcByte, Byte to be calculated Crc8 value
// Returns     : temp, Current Crc8Value
//============================================================================
uint8_t Ltc6803_ByteCrc8Cal(uint8_t crcByte)
{
	uint8_t temp;

	temp = Crc8Table[0x41 ^ crcByte];
	return temp;
}

//Ltc6803的初始化，设置管理芯片参数
void Ltc6803_Init(void)
{
	uint8_t i;
	uint32_t delay = g_SysTickMs;

	TRISCbits.TRISC2 = 0;  //设置ltc6802片选管脚(C2)为输出

	for(i=0; i<2; i++)
	{
		g_ArrayLtc6803Unit[i].GenrCfg.WDT = 0;
		g_ArrayLtc6803Unit[i].GenrCfg.GPIO2 = 1;
		g_ArrayLtc6803Unit[i].GenrCfg.GPIO1 = 1;
		g_ArrayLtc6803Unit[i].GenrCfg.LVLPL = 1;  // 电平轮询
		g_ArrayLtc6803Unit[i].GenrCfg.CELL10 = 0; // 12节电池模式
		g_ArrayLtc6803Unit[i].GenrCfg.CDC = 1;
		g_ArrayLtc6803Unit[i].CellBal = 0;        
		g_ArrayLtc6803Unit[i].CellMask = 0x0FFF;
		g_ArrayLtc6803Unit[i].UndVoltThre = 0;
		g_ArrayLtc6803Unit[i].OveVoltThre = 0;
	}

	while(g_SysTickMs - delay < 300);  //延时300ms等待ltc6803电源稳定

	Ltc6803_WriteCfgRegGroup(g_ArrayLtc6803Unit);
}


//============================================================================
// Function    : Ltc6803_BlockCrc8Cal
// Description : this function calculate the packet error code(PEC)
// Parameters  : CrcByte: point of bytes to be calculated Crc8 value
//               len: length of block be calculated
// Returns     : temp: Current Crc8Value
//============================================================================
uint8_t Ltc6803_BlockCrc8Cal(uint8_t *CrcByte, uint8_t len)
{
	uint8_t temp = 0x41;

	while(len--)
	{
		temp = Crc8Table[temp ^ *CrcByte++];
	}

	return(temp);
}

//============================================================================
// Function    : Ltc6803_WriteCfgRegGroup
// Description : this function write the config registers group
// Parameters  : 
// Returns     : none
//============================================================================
void Ltc6803_WriteCfgRegGroup(Ltc6803_Parameter *Dev)
{
	uint8_t i, pec;
	Ltc6803_Parameter *ptr;

	ptr = Dev + ModuleAmount - 1;

	Ltc6803_ChipSelect();	// CSBI拉低

	SPI_SendByte(WRCFG_CMD);	// 发送WRCFG命令及PEC字节
	SPI_SendByte(0xC7);

	for (i=0; i<ModuleAmount; i++)	
	{
		RegStr.cfgr[0] = *(uint8_t*)&(ptr->GenrCfg);
		RegStr.cfgr[1] = ptr->CellBal;
		RegStr.cfgr[2] = (ptr->CellMask << 4) | (ptr->CellBal >> 8);
		RegStr.cfgr[3] = ptr->CellMask >> 4;
		RegStr.cfgr[4] = ptr->UndVoltThre;
		RegStr.cfgr[5] = ptr->OveVoltThre;

		pec = Ltc6803_BlockCrc8Cal((uint8_t*)&RegStr.cfgr[0], 6);
		SPI_SendBlock(RegStr.cfgr, 6);	// 发送器件的CFGR0-CFGR5 的字节
		SPI_SendByte(pec);	// 发送PEC
		ptr--;
	}

	Ltc6803_ChipUnselect();	// CSBI拉高
}

//============================================================================
// Function    : Ltc6803_ReadCfgRegGroup
// Description : this function reads the parameter from the config registers 
//               group
// Parameters  : 
// Returns     : 
//============================================================================
uint8_t Ltc6803_ReadCfgRegGroup(uint8_t *CfgData)
{
   uint8_t i;
   uint8_t pec;
   uint8_t dataBuf[6];

   dataBuf[0] = RDCFG_CMD;
   dataBuf[1] = 0xCE;  

   Ltc6803_ChipSelect();
   SPI_SendBlock(dataBuf, 2);
   SPI_ReceiveBlock(dataBuf, 6);
   pec = SPI_ReceiveByte();
   Ltc6803_ChipUnselect();

   if (pec == Ltc6803_BlockCrc8Cal(dataBuf, 6))
   {
      for (i=0; i<6; i++)
      {
         *CfgData++ = dataBuf[i];
      }

      return 1;
   }

   return 0;
}

//============================================================================
// Function    : Ltc6803_CellVoltCnvt
// Description : start cell voltage convert, broadcast mode
// Parameters  : CvtMode, STCVAD、STCVDC、STOWAD or STOWDC
//               channel, 
// Returns     : none
//============================================================================
void Ltc6803_CellVoltCnvt(uint8_t convertMode, uint8_t channel)
{
	uint8_t dataBuf[2];

	if (convertMode != STCVDC_CMD)
	{
		dataBuf[0] = STCVAD_CMD | (channel&0x0F);
	}
	else
	{
		dataBuf[0] = STCVDC_CMD | channel;
	}

	dataBuf[1] = Ltc6803_ByteCrc8Cal(dataBuf[0]);  

	Ltc6803_ChipSelect();
	SPI_SendBlock(dataBuf, 2);
	Ltc6803_ChipUnselect();
}

//============================================================================
// Function    : Ltc6803_ReadAllCellVolt
// Description : read the convert value of all cells voltage, address mode
// Parameters  : CellVolt, the raw convert value of the cell voltage
// Returns     : 1, success   0, fail
//============================================================================
uint8_t Ltc6803_ReadAllCellVolt(Ltc6803_Parameter *dev)
{
	uint8_t pec;
	uint8_t i;
	uint8_t j;
	uint16_t temp;

	Ltc6803_ChipSelect();

	SPI_SendByte(RDCV_CMD);	// 发送RDCV命令和PEC
	SPI_SendByte(0xDC);

	for (i=0; i<ModuleAmount; i++)
	{
		SPI_ReceiveBlock(RegStr.cvr, 18);
		pec = SPI_ReceiveByte();

		// PEC校验，判断数据是否正确接收
		if (pec == Ltc6803_BlockCrc8Cal(RegStr.cvr, 18))
		{
			for (j=0; j<6; j++)
			{
				temp = (uint16_t)RegStr.cvr[3*j] | ((uint16_t)(RegStr.cvr[3*j + 1] & 0x0F) << 8);
				dev->CellVolt[2*j] = Ltc6803_CellVoltCal((int16_t)temp);
				temp = (uint16_t)(RegStr.cvr[3*j +1] >> 4) | ((uint16_t)RegStr.cvr[3*j + 2] << 4);
				dev->CellVolt[2*j+1] = Ltc6803_CellVoltCal((int16_t)temp);
			}
		}
		else
		{
			Ltc6803_ChipUnselect();
			return 0;
		}

		dev++;
	}

	Ltc6803_ChipUnselect();

	return(1);
}

//============================================================================
// Function    : Ltc6803_CellVoltCal
// Description : 
// Parameters  : none
// Returns     : none
//============================================================================
int16_t Ltc6803_CellVoltCal(int16_t VoltRaw)
{
   int16_t temp;

   temp = ((VoltRaw - 512)* 3) / 2;  // 读取转换值的时候偏移量512没有减掉

   return temp;
}


//============================================================================
// Function    : Ltc6803_TempCnvt
// Description : start all tempereture convert with broadcast mode
// Parameters  : none
// Returns     : none
//============================================================================
void Ltc6803_TempCnvt(uint8_t channel)
{
	uint8_t dataBuf[2];

	dataBuf[0] = STTMPAD_CMD + (channel&0x0F);
	dataBuf[1] = Ltc6803_ByteCrc8Cal(dataBuf[0]);   

	Ltc6803_ChipSelect();
	SPI_SendBlock(dataBuf, 0x02);
	Ltc6803_ChipUnselect();
}

//============================================================================
// Function    : Ltc6803_ReadTempRegGroup
// Description : 读温度寄存器组
// Parameters  : 
// Returns     : 1, success   0, fail
//============================================================================
uint8_t Ltc6803_ReadTempRegGroup(uint8_t *data)
{
   uint8_t cmdBuf[2], regBuf[6];
   uint8_t pec, i;

   cmdBuf[0] = RDTMP_CMD;
   cmdBuf[1] = 0xEA;

   Ltc6803_ChipSelect();
   SPI_SendBlock(cmdBuf, 2);
   SPI_ReceiveBlock(regBuf, 0x06);
   Ltc6803_ChipUnselect();

   pec = Ltc6803_BlockCrc8Cal(regBuf, 0x05);

   if (pec == regBuf[5])
   {
      for (i=0; i<5; i++)
      {
         *data++ = regBuf[i];
      }

      return 1;
   }

   return 0;
}

//============================================================================
// Function    : Ltc6803_ReadAllTemp
// Description : 采样并计算得到VTEMP1、VTEMP2管脚电压，内部温度传感器电压。
// Parameters  : 
// Returns     : 1, success   0, fail
//============================================================================
uint8_t Ltc6803_ReadAllTemp(Ltc6803_Parameter *dev)
{
   uint8_t pec, i;
   uint16_t tmp;

   Ltc6803_ChipSelect();

   SPI_SendByte(RDTMP_CMD);
   SPI_SendByte(0xEA);

   for (i=0; i<ModuleAmount; i++)
   {
      SPI_ReceiveBlock(RegStr.tmpr, 5);
      pec = SPI_ReceiveByte();

      // PEC校验，判断数据是否正确接受
      if (pec == Ltc6803_BlockCrc8Cal(RegStr.tmpr, 5))
      {
         tmp = (uint16_t)RegStr.tmpr[0] | ((uint16_t)(RegStr.tmpr[1] & 0x0F) << 8);
         dev->Temp1 = (tmp - 512) * 3 / 2;

         tmp = (uint16_t)(RegStr.tmpr[1] >> 4) | ((uint16_t)RegStr.tmpr[2] << 4);
         dev->Temp2 = (tmp - 512) * 3 / 2;

         tmp = (uint16_t)RegStr.tmpr[3] | ((uint16_t)(RegStr.tmpr[4] & 0x0F) << 8);
         dev->Itemp = (tmp - 512) * 3 / 2;
      }
      else
      {
         Ltc6803_ChipUnselect();
         return(0);
      }
      dev++;
   }

   Ltc6803_ChipUnselect();

   return(1);
}

//============================================================================
// Function    : Ltc6803_ReadRef
// Description : 读取VREF值（mv）
// Parameters  : 
// Returns     : 1, success   0, fail
//============================================================================
uint8_t Ltc6803_ReadRef(Ltc6803_Parameter *dev)
{
   uint8_t pec, i;
   uint16_t tmp;

   Ltc6803_ChipSelect();

   SPI_SendByte(RDDGNR_CMD);
   SPI_SendByte(0x6B);

   for (i=0; i<ModuleAmount; i++)
   {
      SPI_ReceiveBlock(RegStr.dgnr, 2);
      pec = SPI_ReceiveByte();

      // PEC校验，判断数据是否正确接受
      if (pec == Ltc6803_BlockCrc8Cal(RegStr.dgnr, 2))
      {
         tmp = (uint16_t)RegStr.dgnr[0] | ((uint16_t)(RegStr.dgnr[1] & 0x0F) << 8);
         dev->Vref = (tmp - 512) * 3 / 2;
      }
      else
      {
         Ltc6803_ChipUnselect();
         return 0;
      }
   }

   Ltc6803_ChipUnselect();

   return 1;
}

//============================================================================
// Function    : Ltc6803_ReadFlagRegister
// Description : 
// Parameters  : *OverFlag, over voltage flags return
//               *UnderFlag, under voltage flags return 
// Returns     : 1, success   0, fail
//============================================================================
uint8_t Ltc6803_ReadFlagRegister(Ltc6803_Parameter *dev)
{
   uint8_t pec, i, j, k;

   Ltc6803_ChipSelect();

   SPI_SendByte(RDFLG_CMD);
   SPI_SendByte(0xE4);

   for (k=0; k<ModuleAmount; k++)
   {
      SPI_ReceiveBlock(RegStr.flgr, 3);
      pec = SPI_ReceiveByte();

      // PEC校验，判断数据是否正确接受
      if (pec == Ltc6803_BlockCrc8Cal(RegStr.flgr, 3))
      {
         dev->UndVoltFlg = 0;
         dev->OveVoltFlg = 0;

         for (i=0; i<3; i++)
         {
            for (j=0; j<4; j++)
            {
               if (RegStr.flgr[i] & (0x01 << (j*2)))
               {
                  dev->UndVoltFlg |= 0x0001 << (i*4 + j);
               }

               if (RegStr.flgr[i] & (0x01 << (j*2 + 1)))
               {
                  dev->OveVoltFlg |= 0x0001 << (i*4 + j); 
               }
            }
         }
      }
      else
      {
         Ltc6803_ChipUnselect();
         return(0);
      }
   }

   Ltc6803_ChipUnselect();
   return(1);
}

//============================================================================
// Function    : Ltc6803_SetBalanceSwitch
// Description : set the balance switch state
// Parameters  : cells, cells need to be balance
// Returns     : 1,success  0,fail
//============================================================================
uint8_t Ltc6803_SetBalanceSwitch(uint16_t cells)
{

}

