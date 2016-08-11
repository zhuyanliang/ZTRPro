/* 
 * File:   newfile.h
 * Author: Administrator
 *
 * Created on 2016年6月20日, 下午3:36
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"

//------------------------ function prototypes -------------------------------
uint8_t EEPROM_IsReady(void);
uint8_t EEPROM_ReadByte(uint16_t addr);
void EEPROM_WriteByte(uint16_t addr, uint8_t data);
void EEPROM_WriteBlock(uint16_t addr, uint8_t *pData, uint8_t len);
extern void EEPROM_ReadBlock(uint16_t addr, uint8_t *pData, uint8_t len);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

