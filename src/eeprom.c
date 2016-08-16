//============================================================================
//
//              Copyright (c) 2013, Globetools. All rights reserved.
//
//
//  File name   �� eeprom.c
//  Description �� Ƭ��EEPROM�Ķ�д����
//  Versions    �� V1.0 
//  Author      �� 
//  Date        �� 
//  History     ��
//  Comment     ��
//============================================================================
#include "include.h"

//----------------------------------------------------------------------------
// ����: EEPROM_IsReady
// ����: Returns true if EEPROM is ready to read/write
// ����: none
// ����:
//----------------------------------------------------------------------------
uint8_t EEPROM_IsReady(void)
{
    return EECON1bits.WR == 0 ? 1 : 0;
}

//----------------------------------------------------------------------------
// ����: EEPROM_ReadByte
// ����: ��EEPROM�дӵ�ַaddr�ж�ȡһ���ֽ�����
// ����: none
// ����: none
//----------------------------------------------------------------------------
uint8_t EEPROM_ReadByte(uint16_t addr)
{
   EEADRH = addr >> 8;                      // ���ö���ַ�ĸ�λ
   EEADR  = addr;                           // ���ö���ַ��λ

   EECON1bits.EEPGD = 0;                    // �������� EEPROM �洢��
   EECON1bits.CFGS  = 0;                    // �����������洢�������� EEPROM �洢��
   EECON1bits.RD = 1;                       // ���� EEPROM ������
   Nop();
   Nop();// �ղ���

   return (uint8_t)EEDATA;                           
}

//----------------------------------------------------------------------------
// ����: EEPROM_WriteByte
// ����: ���ַaddrд������data
// ����: none
// ����: none
//----------------------------------------------------------------------------
void EEPROM_WriteByte(uint16_t addr, uint8_t data)
{
   EEADRH = addr >> 8;                      // ����д��ַ��λ
   EEADR  = addr;							// ����д��ַ��λ

   EECON1bits.WREN  = 1;                    // ������������洢�� / ���� EEPROM �洢����д����
   EECON1bits.EEPGD = 0;                    // �������� EEPROM �洢��
   EECON1bits.CFGS  = 0;					// �����������洢�������� EEPROM �洢��

   INTCONbits.GIE   = 0;                    // ��ֹ�ж�
   EEDATA           = data;                 // Setup data value
   EECON2           = 0x55;                 // Interlock pattern 1
   EECON2           = 0xAA;                 // Interlock pattern 2
   EECON1bits.WR    = 1;                    // ����д����
   
   while(EECON1bits.WR);					// д��ɺ�WRλ��Ӳ����0
   INTCONbits.GIE   = 1;                    // �ָ��ж�

   EECON1bits.WREN  = 0;
}

//----------------------------------------------------------------------------
// ����: EEPROM_WriteBlock
// ����: ���ַaddrд�볤��Ϊlen�ֽڵ�����pData
// ����: none
// ����: none
//----------------------------------------------------------------------------
void EEPROM_WriteBlock(uint16_t addr, uint8_t *pData, uint8_t len)
{
   // wait for EEPROM ready
   while (!EEPROM_IsReady());

   while (len--)
      EEPROM_WriteByte(addr++,*pData++);
}

//----------------------------------------------------------------------------
// ����: EEPROM_ReadBlock
// ����: �ӵ�ַaddr��ȡ����Ϊlen�ֽڵ����ݷ���pData
// ����: none
// ����: none
//----------------------------------------------------------------------------
void EEPROM_ReadBlock(uint16_t addr, uint8_t *pData, uint8_t len)
{
	// wait for EEPROM ready
	while (!EEPROM_IsReady());

	while (len--)
		*pData++ = EEPROM_ReadByte(addr++);
}











