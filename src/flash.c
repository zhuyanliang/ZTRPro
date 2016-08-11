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
   EECON1bits.EEPGD = 1;                // ��������������Գ���洢������  
   EECON1bits.CFGS  = 0;                // �� EEPGD ��ѡ����ʵĴ洢������
   EECON1bits.WREN  = 1;                // �������д����
   EECON1bits.FREE  = 1;                // ����Գ���洢�����в�������    
//   INTCONbits.GIE   = 0;              // Disable interrupts           
   EECON2           = 0x55;             // Interlock pattern 1          
   EECON2           = 0xAA;             // Interlock pattern 2          
   EECON1bits.WR    = 1;                // ����д����           
//   INTCONbits.GIE   = 1;              // Reenable interrupts 
               
   while (EECON1bits.WR);               //д�������ʱ����Ӳ����������                                
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
     * ��ִ�� TBLRD ʱ�� TBLPTR ������ 22 λ����������洢�����ĸ��ֽڶ��� TABLAT��
     * ��ִ�� TBLWT ʱ�� ��ָ��Ĵ����ĵ� 6 λ��TBLPTR<5:0>�� ����Ҫд��
     * ����洢�����ĸ����ּĴ��� ������ 64 ������
     * ������洢���Ķ�ʱд�� ��ͨ��WRλ�� ��ʼʱ�� 
     * TBLPTR�ĸ�16λ �� TBLPTR<21:6>��������Ҫд���ĸ�����洢���� ��ÿ�� 64 �ֽڣ���
     */
    TblAddr = FlashAddr  & 0x3FFFC0;    // ʼ�մӱ��ּĴ���0��ʼ
    NumByte = 64;                       // Number of bytes written
    TBLPTR = TblAddr;                   // 22 λ�ĵ�ַ

    while (NumByte--)                   // If still have byte
    {
        TABLAT = *BufPtr++;              // �Զ������� 64 �ֽ�д�뱣�ּĴ���

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

