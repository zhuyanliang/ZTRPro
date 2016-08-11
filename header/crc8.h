/* 
 * File:   crc8.h
 * Author: Administrator
 *
 * Created on 2016年6月21日, 上午8:33
 */

#ifndef CRC8_H
#define	CRC8_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"
    
uint8_t calculate_crc8(uint8_t* ptr, uint8_t len);
uint8_t calculate_char_crc8(uint8_t crc, uint8_t nbyte);


#ifdef	__cplusplus
}
#endif

#endif	/* CRC8_H */

