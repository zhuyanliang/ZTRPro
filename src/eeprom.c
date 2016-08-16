//============================================================================
//
//              Copyright (c) 2013, Globetools. All rights reserved.
//
//
//  File name   ： eeprom.c
//  Description ： 片上EEPROM的读写驱动
//  Versions    ： V1.0 
//  Author      ： 
//  Date        ： 
//  History     ：
//  Comment     ：
//============================================================================
#include "include.h"

//----------------------------------------------------------------------------
// 名称: EEPROM_IsReady
// 功能: Returns true if EEPROM is ready to read/write
// 参数: none
// 返回:
//----------------------------------------------------------------------------
uint8_t EEPROM_IsReady(void)
{
    return EECON1bits.WR == 0 ? 1 : 0;
}

//----------------------------------------------------------------------------
// 名称: EEPROM_ReadByte
// 功能: 在EEPROM中从地址addr中读取一个字节数据
// 参数: none
// 返回: none
//----------------------------------------------------------------------------
uint8_t EEPROM_ReadByte(uint16_t addr)
{
   EEADRH = addr >> 8;                      // 设置读地址的高位
   EEADR  = addr;                           // 设置读地址低位

   EECON1bits.EEPGD = 0;                    // 访问数据 EEPROM 存储器
   EECON1bits.CFGS  = 0;                    // 访问闪存程序存储器或数据 EEPROM 存储器
   EECON1bits.RD = 1;                       // 启动 EEPROM 读操作
   Nop();
   Nop();// 空操作

   return (uint8_t)EEDATA;                           
}

//----------------------------------------------------------------------------
// 名称: EEPROM_WriteByte
// 功能: 向地址addr写入数据data
// 参数: none
// 返回: none
//----------------------------------------------------------------------------
void EEPROM_WriteByte(uint16_t addr, uint8_t data)
{
   EEADRH = addr >> 8;                      // 设置写地址高位
   EEADR  = addr;							// 设置写地址低位

   EECON1bits.WREN  = 1;                    // 允许对闪存程序存储器 / 数据 EEPROM 存储器的写周期
   EECON1bits.EEPGD = 0;                    // 访问数据 EEPROM 存储器
   EECON1bits.CFGS  = 0;					// 访问闪存程序存储器或数据 EEPROM 存储器

   INTCONbits.GIE   = 0;                    // 禁止中断
   EEDATA           = data;                 // Setup data value
   EECON2           = 0x55;                 // Interlock pattern 1
   EECON2           = 0xAA;                 // Interlock pattern 2
   EECON1bits.WR    = 1;                    // 启动写操作
   
   while(EECON1bits.WR);					// 写完成后，WR位由硬件清0
   INTCONbits.GIE   = 1;                    // 恢复中断

   EECON1bits.WREN  = 0;
}

//----------------------------------------------------------------------------
// 名称: EEPROM_WriteBlock
// 功能: 向地址addr写入长度为len字节的数据pData
// 参数: none
// 返回: none
//----------------------------------------------------------------------------
void EEPROM_WriteBlock(uint16_t addr, uint8_t *pData, uint8_t len)
{
   // wait for EEPROM ready
   while (!EEPROM_IsReady());

   while (len--)
      EEPROM_WriteByte(addr++,*pData++);
}

//----------------------------------------------------------------------------
// 名称: EEPROM_ReadBlock
// 功能: 从地址addr读取长度为len字节的数据放入pData
// 参数: none
// 返回: none
//----------------------------------------------------------------------------
void EEPROM_ReadBlock(uint16_t addr, uint8_t *pData, uint8_t len)
{
	// wait for EEPROM ready
	while (!EEPROM_IsReady());

	while (len--)
		*pData++ = EEPROM_ReadByte(addr++);
}











