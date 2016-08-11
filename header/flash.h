/* 
 * File:   flash.h
 * Author: Administrator
 *
 * Created on 2016年6月20日, 下午3:38
 */

#ifndef FLASH_H
#define	FLASH_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"    
    
void FLASH_Erase64Bytes(uint24_t FlashAddr);
void FlashWr64Bytes(uint24_t FlashAddr, uint8_t *BufPtr);


#ifdef	__cplusplus
}
#endif

#endif	/* FLASH_H */

