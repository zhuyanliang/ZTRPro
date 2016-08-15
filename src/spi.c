
//============================================================================
//  File name    ：spi.c
//  Description  ：spi模块配置及相关驱动函数,
//  Versions     ：V1.0
//  Copyright    ：
//  Author       ：
//  Date         ：
//  Modification ：
//  Comment      ：
//============================================================================

#include "include.h"

//----------------------------------------------------------------------------
// Function    : SPI_Init
// Description : This function config the spi port as master mode,
// Parameters  : none
// Returns     : none
//----------------------------------------------------------------------------
void SPI_Init(void)
{
	SpiDisable();

	SSPSTATbits.SMP = 0b1;  //Setup Master SPI mode
	SSPSTATbits.CKE = 0b1;

	SSPCON1bits.SSPM = 3;  //spi clock: TMR2 output/2,
	SSPCON1bits.CKP = 0;  //The clock signal is low when idle

	TRISCbits.TRISC5 = 0;  //config the SDO output
	TRISCbits.TRISC4 = 1;
	TRISCbits.TRISC3 = 0;  //config the SCK output
	
	SpiEnable();  //open spi
}

//----------------------------------------------------------------------------
// Function    : SPI_SendByte
// Description : This function writes a byte to SPI port
// Parameters  : uint8_t WrByte
// Returns     : None
//----------------------------------------------------------------------------
void SPI_SendByte(uint8_t WrByte)
{
	uint8_t TmpByt;

	PIR1bits.SSPIF = 0;  //Clear interrupt flag
	SSPCON1bits.WCOL = 0;  //Clear any previous write collision

	if (SSPSTATbits.BF)
	{
		TmpByt = SSPBUF;  //Clear buffer
	}

	SSPBUF = WrByte;  //Initiate bus cycle
	while(!SSPSTATbits.BF);  //Wait until cycle complete
//   while(!PIR1bits.SSPIF);  //Do not use this！！！
}   

//----------------------------------------------------------------------------
// Function    : SPI_SendBlock
// Description : This function writes a byte to SPI port
// Parameters  : BlkPtr,data block pointer
//               BlkLen,data block length
// Returns     : None
//----------------------------------------------------------------------------
void SPI_SendBlock(uint8_t *BlockPtr, uint8_t BlockLen)
{
	uint8_t tmpByt;

	if (SSPSTATbits.BF)
	{
		tmpByt = SSPBUF;  // Clear buffer, prevents overrun
	}

	while (BlockLen--)  // While there are bytes left
	{                          
		SSPBUF = *BlockPtr++;  // initiate SPI bus cycle
		while(!SSPSTATbits.BF);  // Wait until cycle complete
		tmpByt = SSPBUF;  // Clear buffer
	}
}   

//----------------------------------------------------------------------------
// Function    : SPI_ReceiveByte
// Description : This function reads a byte from SPI port
// Parameters  : none
// Returns     : the data in receive buf
//----------------------------------------------------------------------------
uint8_t SPI_ReceiveByte(void)
{
	uint8_t TmpByt;

	PIR1bits.SSPIF = 0;

	if (SSPSTATbits.BF)
	{
		TmpByt = SSPBUF;  // Clear buffer, prevents overrun
	}

	SSPBUF = 0x00;  // Initiate bus cycle
	while(!SSPSTATbits.BF);  // Wait until cycle complete
	return(SSPBUF);  //return with byte read 
}  

//----------------------------------------------------------------------------
// Function    : SPI_ReceiveBlock
// Description : This function reads a byte from SPI port
// Parameters  : 
// Returns     : none
//----------------------------------------------------------------------------
void SPI_ReceiveBlock(uint8_t *BlockPtr, uint8_t BlockLen)
{
	uint8_t tmpByt;

	PIR1bits.SSPIF = 0;

	if (SSPSTATbits.BF)
	{
		tmpByt = SSPBUF;  // Clear buffer, prevents overrun
	}

	while (BlockLen--)  // While there are bytes left
	{ 
		SSPBUF = 0x00;  // Initiate bus cycle
		while(!SSPSTATbits.BF);  // Wait until cycle complete
		*BlockPtr++ = SSPBUF;  //return with byte read 
	}
} 