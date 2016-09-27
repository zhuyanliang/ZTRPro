/* 
 * File:   ecan.h
 * Author: Administrator
 *
 * Created on 2016年6月23日, 下午2:59
 */

#ifndef ECAN_H
#define	ECAN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"   
    
#define CAN_ID_STD                  ((uint8_t)0x00)           
#define CAN_ID_EXT                  ((uint8_t)0x08)
#define CAN_RTR_DATA                ((uint8_t)0x00)         
#define CAN_RTR_REMOTE              ((uint8_t)0x40)

#define CAN_BAUDRATE    125         //设置CAN波特率为125kbps
    
//CAN message structure definition  
typedef struct
{
   uint32_t COB_ID;
   uint8_t IDE;
   uint8_t RTR; 
   uint8_t DLC;
   uint8_t Data[8];
} CanMsgTypedef;

typedef enum {
   NORMAL_MODE = 0,
   SLEEP_MODE, 
   LOOP_MODE,
   MONITOR_MODE,                
   CONFIG_MODE
}EcanModeSet;

void ECAN_Init(void);
uint8_t ECAN_TransmitMsg(CanMsgTypedef* TxMessage);
uint8_t ECAN_ReceiveMsg(CanMsgTypedef* RxMessage);


#ifdef	__cplusplus
}
#endif

#endif	/* ECAN_H */

