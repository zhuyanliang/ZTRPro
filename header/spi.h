/* 
 * File:   newfile.h
 * Author: Administrator
 *
 * Created on 2016年6月21日, 上午11:39
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <pic18.h>
#include "common.h"

#define SpiEnable()         SSPCON1bits.SSPEN=1
#define SpiDisable()        SSPCON1bits.SSPEN=0

//------------------------ external prototypes -------------------------------
void SPI_Init(void);
void SPI_SendByte(uint8_t WrByte);
void SPI_SendBlock(uint8_t *BlockPtr, uint8_t BlockLen);
uint8_t SPI_ReceiveByte(void);
void SPI_ReceiveBlock(uint8_t *BlockPtr, uint8_t BlockLen);

//------------------------ SSPSTAT definitions -------------------------------
//          Signal   Description
//----------------------------------------------------------------------------
// 1....... SMP      SampleBit (Slave=0) (Master 1=End of data out)
//                                       (       0=Mid of data out)
// .1...... CKE      Clock select bit (1= Tx occurs active=>idle)
//                                    (0= Tx occurs idle=>active)
// ..0..... D/A      Data/Address bit (I2C mode only)
// ...0.... P        Stop  bit (I2C mode only)
// ....0... S        Start bit (I2C mode only)
// .....0.. R/W      Read/Write info bit (I2C mode only)
// ......0. UA       Update address bit (I2C mode only)
// .......0 BF       Buffer full status bit (receive mode only)
//                   (1= Rx Complete, SSPxBUF is full)
//                   (0= Rx not complete, SSPxBUF is empty)
#define SSPSTAT_INIT      0xC0  // Master SPI mode

#ifdef	__cplusplus
}
#endif

#endif	/* NEWFILE_H */

