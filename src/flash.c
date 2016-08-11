#include "flash.h"
#include <pic18.h>
//============================================================================
// Function    : FLASH_Erase64Bytes
// Description : This function erases the 64 bytes of flash program memory 
//               specified by FlashAddr (ignoring the 6 LSBs )
// Parameters  : unsigned short long - Address of block to erase
// Returns     : none
//============================================================================
void FLASH_Erase64Bytes(uint24_t FlashAddr)
{
   TBLPTR = FlashAddr;                  // Setup flash program address
   EECON1bits.EEPGD = 1;                // 后续操作都将针对程序存储器进行  
   EECON1bits.CFGS  = 0;                // 由 EEPGD 来选择访问的存储器类型
   EECON1bits.WREN  = 1;                // 允许进行写操作
   EECON1bits.FREE  = 1;                // 允许对程序存储器进行擦除操作    
//   INTCONbits.GIE   = 0;              // Disable interrupts           
   EECON2           = 0x55;             // Interlock pattern 1          
   EECON2           = 0xAA;             // Interlock pattern 2          
   EECON1bits.WR    = 1;                // 启动写操作           
//   INTCONbits.GIE   = 1;              // Reenable interrupts 
               
   while (EECON1bits.WR);               //写操作完成时，由硬件将其清零                                
   EECON1bits.WREN = 0;                 // Disable flash program writes        
}

//============================================================================
// Function    : FlashWr64Bytes
// Description : This function writes 64 bytes to the flash program memory 
//               address specified by FlashAddr (ignoring the 6 LSBs)
// Parameters  : unsigned short long - Address of block write
//               unsigned char *BufPtr - Pointer to data written
// Returns     : none
//============================================================================
void FlashWr64Bytes(uint24_t FlashAddr, uint8_t *BufPtr)
{
    uint8_t NumByte;                    // Number of byte to write
    uint24_t TblAddr;                   // Two block start address

    FLASH_Erase64Bytes(FlashAddr);    // Erase 64 bytes by rom addr
   
    /*
     * 当执行 TBLRD 时， TBLPTR 的所有 22 位决定将程序存储器的哪个字节读入 TABLAT。
     * 当执行 TBLWT 时， 表指针寄存器的低 6 位（TBLPTR<5:0>） 决定要写入
     * 程序存储器的哪个保持寄存器 （共有 64 个）。
     * 当程序存储器的定时写入 （通过WR位） 开始时， 
     * TBLPTR的高16位 （ TBLPTR<21:6>）将决定要写入哪个程序存储器块 （每块 64 字节）。
     */
    TblAddr = FlashAddr  & 0x3FFFC0;    // 始终从保持寄存器0开始
    NumByte = 64;                       // Number of bytes written
    TBLPTR = TblAddr;                   // 22 位的地址

    while (NumByte--)                   // If still have byte
    {
        TABLAT = *BufPtr++;              // 自动递增将 64 字节写入保持寄存器

        #asm      
        TBLWTPOSTINC      // 32 holding registers
        #endasm                     
    }

    TBLPTR = TblAddr;                // Setup start written addr
    EECON1bits.EEPGD = 1;            // Point to program memory      
    EECON1bits.CFGS  = 0;            // Set write enable bit         
    EECON1bits.WREN  = 1;            // Flash row erase enable bit            
    EECON2           = 0x55;         // Interlock pattern 1          
    EECON2           = 0xAA;         // Interlock pattern 2          
    EECON1bits.WR    = 1;            // Start writing        
    while (EECON1bits.WR);           // Wait for write to terminate  
    EECON1bits.WREN = 0;             // Disable flash program writes                                                                             
}  

