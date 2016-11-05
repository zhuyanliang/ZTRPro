//============================================================================
//
//              Copyright (c) 2013, Globetools. All rights reserved.
//
//
//  File name   ：ecan.c
//  Description ：ecan模块配置及相关驱动函数.
//  Versions    ：V1.0
//  Author      ：
//  Date        ：
//  History     ：
//  Comment     ：
//============================================================================

#include "include.h"

//----------------------------------------------------------------------------
// Function    ：ECAN_Init
// Description ：ecan初始化配置函数
// Parameters  ：none
// Returns     ：none
//----------------------------------------------------------------------------
void ECAN_Init(void)
{
	//set cantx as input
	TRISBbits.TRISB3 = 1;

	//Enter CAN module into config mode
	CANCON = 0x80;
	while(!(CANSTATbits.OPMODE ==0x04));
	//Enter CAN module into Mode 0
	ECANCON = 0x00;

	//Initialize CAN baud rate generator
#if (Fosc == 16000000)      
#if (CAN_BAUDRATE == 100)       
	BRGCON1 = 0x07; //0000 0111  //SJW=1TQ, BRP 7
	BRGCON2 = 0x98; //1001 1000  //SEG2PHTS 1, sampled once  PS1=4TQ, PropagationT 1TQ  
	BRGCON3 = 0x03; //0000 0011  //PS2 3TQ
#elif (CAN_BAUDRATE == 125)     
	BRGCON1 = 0x07; //0000 0111  //SJW=1TQ, BRP 7
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1, sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ
#elif (CAN_BAUDRATE == 250)      
	BRGCON1 = 0x03; //0000 0011  //SJW=1TQ, BRP 3
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1, sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ  
#elif (CAN_BAUDRATE == 500)      
	BRGCON1 = 0x01; //0000 0001  //SJW=1TQ, BRP 1
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1 ,sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ
#elif (CAN_BAUDRATE == 1000)      
	BRGCON1 = 0x00; //0000 0000  //SJW=1TQ, BRP 0
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1, sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ
#else
#endif
#elif Fosc == 64000000  //config can baudrate while Fosc==64MHz   
#if (CAN_BAUDRATE == 100)       
	BRGCON1 = 0x5F; //0101 1111  //SJW=2TQ, BRP 31
	BRGCON2 = 0x91; //1001 0001  //SEG2PHTS 1, sampled once  PS1=3TQ, PropagationT 2TQ
	BRGCON3 = 0x03; //0000 0011  //PS2 3TQ
#elif (CAN_BAUDRATE == 125)
#if 0
	BRGCON1 = 0x1F; //0001 1111  //SJW=1TQ, BRP 31
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1, sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ
#endif
	BRGCON1 = 0x8F; //0000 0111     //SJW=3TQ     BRP  15
	BRGCON2 = 0xB8; //1011 1000     //SEG2PHTS 1    sampled once  PS1=8TQ  PropagationT 1TQ  
	BRGCON3 = 0x05; //0000 0101     //PS2  6TQ 
	//  125 Kbps @ 40MHz 
	//BRGCON1 = 0x49; //0000 0111     //SJW=2TQ     BRP  9
	//BRGCON2 = 0xF1; //1011 1000     //SEG2PHTS 1    sampled once  PS1=8TQ  PropagationT 1TQ  
	//BRGCON3 = 0x05; //0000 0101     //PS2  6TQ 
#elif (CAN_BAUDRATE == 250)      
	BRGCON1 = 0x0F; //0000 1111  //SJW=1TQ, BRP 15
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1, sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ  
#elif (CAN_BAUDRATE == 500)      
	BRGCON1 = 0x07; //0000 0111  //SJW=1TQ, BRP 7
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1 ,sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ
#elif (CAN_BAUDRATE == 1000)      
	BRGCON1 = 0x03; //0000 0011  //SJW=1TQ, BRP 3
	BRGCON2 = 0x90; //1001 0000  //SEG2PHTS 1, sampled once  PS1=3TQ, PropagationT 1TQ  
	BRGCON3 = 0x02; //0000 0010  //PS2 3TQ
#else
#endif
#endif


	//Set Receive Mode for buffers
	RXB0CON = 0x00;
	RXB1CON = 0x00;

	// 配置过滤器屏蔽器
	RXM0SIDL = 0x00;
	RXM0SIDH = 0x00;
	RXM0EIDL = 0x00;
	RXM0EIDH = 0x00;

	RXM1SIDL = 0x00;
	RXM1SIDH = 0x00;
	RXM1EIDL = 0x00;
	RXM1EIDH = 0xFF;

	RXF0SIDL = 0;
	RXF0SIDH = 0;
	RXF0EIDL = 0;
	RXF0EIDH = 0;

	RXF1SIDL = 0;
	RXF1SIDH = 0;
	RXF1EIDL = 0;
	RXF1EIDH = BMS;//0xF4

	RXF0SIDLbits.EXIDEN = 0;  // 过滤器0只接收标准报文
	RXF1SIDLbits.EXIDEN = 1;  // 过滤器1只接收扩展报文

	CIOCON = 0x20;

	// 接收中断设置
	PIR5bits.RXB0IF = 0;
	IPR5bits.RXB0IP = 0;
	PIE5bits.RXB0IE = 0b0;
	PIR5bits.RXB1IF = 0;
	IPR5bits.RXB1IP = 0;
	PIE5bits.RXB1IE = 0b0;

	//Enter CAN module into normal mode
	CANCON = 0x00;
	while(!(CANSTATbits.OPMODE==0x00));
}

//----------------------------------------------------------------------------
// Function    ：ECAN_TransmitMsg
// Description ：
// Parameters  ：
// Returns     ：
//----------------------------------------------------------------------------
uint8_t ECAN_TransmitMsg(CanMsgTypedef* TxMessage)
{
   uint8_t i, *ptr;

   //Select one empty transmit mailbox
   if (!TXB0CONbits.TXREQ)
   {       
      if (TxMessage->IDE == CAN_ID_STD)  //Set up the standar Id
      {
         TXB0SIDH = (uint8_t)(TxMessage->COB_ID >> 3);
         TXB0SIDL = (uint8_t)(TxMessage->COB_ID << 5);
      }
      else  //Set up the extend Id
      {
         TXB0SIDH = (uint8_t)(TxMessage->COB_ID >> 21);
         TXB0SIDL = ((uint8_t)(TxMessage->COB_ID >> 13) & 0xE0) | 0x08 | 
                    ((uint8_t)(TxMessage->COB_ID >> 16) & 0x03);
         TXB0EIDH = (uint8_t)(TxMessage->COB_ID >> 8);
         TXB0EIDL = (uint8_t)TxMessage->COB_ID;         
      }
       
      if (TxMessage->RTR == CAN_RTR_DATA)
      { 
         TXB0DLCbits.TXRTR = 0;
         TXB0DLCbits.DLC = TxMessage->DLC;  //Set up the DLC

         ptr = (uint8_t*)0x0F26;  //address of TXB0D0

         for (i=0;i<TxMessage->DLC;i++)
         {
            *ptr++ = TxMessage->Data[i];
         }
      }
      else
      {
         TXB0DLCbits.TXRTR = 1;
      }

      TXB0CONbits.TXREQ = 1;  // Request transmission
   }
   else if (!TXB1CONbits.TXREQ)
   {
      if (TxMessage->IDE == CAN_ID_STD)  //Set up the standar Id
      {
         TXB1SIDH = (uint8_t)(TxMessage->COB_ID >> 3);
         TXB1SIDL = (uint8_t)(TxMessage->COB_ID << 5);
      }
      else  //Set up the extend Id
      {
         TXB1SIDH = (uint8_t)(TxMessage->COB_ID >> 21);
         TXB1SIDL = ((uint8_t)(TxMessage->COB_ID >> 13) & 0xE0) | 0x08 | 
                    ((uint8_t)(TxMessage->COB_ID >> 16) & 0x03);
         TXB1EIDH = (uint8_t)(TxMessage->COB_ID >> 8);
         TXB1EIDL = (uint8_t)TxMessage->COB_ID;
      }
       
      if (TxMessage->RTR == CAN_RTR_DATA)
      {
         TXB1DLCbits.TXRTR = 0; 
         TXB1DLCbits.DLC = TxMessage->DLC;  //Set up the DLC

         ptr = (uint8_t*)0x0F16;  //address of TXB1D0

         for (i=0;i<TxMessage->DLC;i++)
         {
            *ptr++ = TxMessage->Data[i];
         }
      }
      else
      {
         TXB1DLCbits.TXRTR = 1;
      }

      TXB1CONbits.TXREQ = 1;  //Request transmission
   }
   else if (!TXB2CONbits.TXREQ)
   {
      if (TxMessage->IDE == CAN_ID_STD)  //Set up the standar Id
      {
         TXB2SIDH = (uint8_t)(TxMessage->COB_ID >> 3);
         TXB2SIDL = (uint8_t)(TxMessage->COB_ID << 5);
      }
      else  //Set up the extend Id
      {
         TXB2SIDH = (uint8_t)(TxMessage->COB_ID >> 21);
         TXB2SIDL = ((uint8_t)(TxMessage->COB_ID >> 13) & 0xE0) | 0x08 | 
                    ((uint8_t)(TxMessage->COB_ID >> 16) & 0x03);
         TXB2EIDH = (uint8_t)(TxMessage->COB_ID >> 8);
         TXB2EIDL = (uint8_t)TxMessage->COB_ID;
      }
       
      if (TxMessage->RTR == CAN_RTR_DATA)
      {
         TXB2DLCbits.TXRTR = 0; 
         TXB2DLCbits.DLC = TxMessage->DLC;  //Set up the DLC

         ptr = (uint8_t*)0x0F06;  //address of TXB2D0

         for (i=0;i<TxMessage->DLC;i++)
         {
            *ptr++ = TxMessage->Data[i];
         }
      }
      else
      {
         TXB2DLCbits.TXRTR = 1;
      }

      TXB2CONbits.TXREQ = 1;  //Request transmission 
   }
   else
   {
      return 0;
   }

   return 1;
}

//----------------------------------------------------------------------------
// Function    ：ECAN_ReceiveMsg
// Description ：check接收缓冲是否收到数据，如有数据则解析并放入缓冲区
// Parameters  ：  
// Returns     ：none
//----------------------------------------------------------------------------
uint8_t ECAN_ReceiveMsg(CanMsgTypedef* RxMessage)
{
   uint8_t i, *ptr;

   if(RXB0CONbits.RXFUL)
   {
      ptr = (uint8_t*)0x0F66;   //寄存器RXB0D0地址
      RxMessage->IDE = RXB0SIDL & 0x08;     //1-接收到的报文时扩展帧

      if (!RxMessage->IDE)
      {
         RxMessage->COB_ID = (((uint32_t)RXB0SIDH<<8)|((uint32_t)RXB0SIDL&0xE0))>>5;
      }
      else
      {
         RxMessage->COB_ID = ((uint32_t)RXB0SIDH<<21)|(((uint32_t)RXB0SIDL&0xE0)<<13)
                            |(((uint32_t)RXB0SIDL&0x03)<<16)
                            |((uint32_t)RXB0EIDH<<8) |((uint32_t)RXB0EIDL);   
      }
      
      RxMessage->RTR = RXB0DLC & 0x40;  //1-远程传输请求

      if (!RxMessage->RTR)
      { 
         RxMessage->DLC = RXB0DLCbits.DLC;

         for (i=0;i<RxMessage->DLC;i++)
         {
            RxMessage->Data[i] = *ptr++;
         }
      }
      
      RXB0CONbits.RXFUL = 0;    //接收缓冲区可用于接收新报文
   }
   
   else if (RXB1CONbits.RXFUL)
   {
      ptr = (uint8_t*)0x0F36;       //寄存器RXB1D0地址
      RxMessage->IDE = RXB1SIDL & 0x08;

      if (!RxMessage->IDE)
      {
         RxMessage->COB_ID = (((uint32_t)RXB1SIDH<<8)|((uint32_t)RXB1SIDL&0xE0))>>5;   
      }
      else
      {
         RxMessage->COB_ID = ((uint32_t)RXB1SIDH<<21)|(((uint32_t)RXB1SIDL&0xE0)<<13)
         					|(((uint32_t)RXB1SIDL&0x03)<<16)|
                            ((uint32_t)RXB1EIDH<<8) |((uint32_t)RXB1EIDL);           
      }
      
      RxMessage->RTR = RXB1DLC & 0x40;

      if (!RxMessage->RTR)
      { 
         RxMessage->DLC = RXB1DLCbits.DLC;

         for (i=0;i<RxMessage->DLC;i++)
         {
            RxMessage->Data[i] = *ptr++;
         }
      }

      RXB1CONbits.RXFUL = 0;
   }

   else
   {
      return 0;
   }

   return 1; 
}
