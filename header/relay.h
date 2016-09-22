/* 
 * File:   relay.h
 * Author: Administrator
 *
 * Created on 2016年7月1日, 上午10:49
 */

#ifndef RELAY_H
#define	RELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"
//---------------------------- Macro definition ------------------------------
//移植的时候根据需要修改以下宏定义
//------------------------- Control Pin definition ---------------------------
#define  RELAY_POSI_CTRL                LATCbits.LATC0     // 定义主控正继电器控制管脚
#define  RELAY_NEGA_CTRL                LATAbits.LATA2     // 定义主控负继电器控制管脚
#define  RELAY_PREC_CTRL                LATBbits.LATB0     // 定义预充电继电器控制管脚
#define  RELAY_FAN_CTRL                 LATBbits.LATB5    
//-------------------------- variable definition -----------------------------
typedef union{
   uint8_t all;

   struct {
      unsigned positive :1;
      unsigned negative :1;
      unsigned precharge:1;
      unsigned heating  :1;
      unsigned cooling  :1;
      unsigned          :3;
   };
}RelayFlagTepedef;

#define FanOn()                            LATBbits.LATB5 = 0b1      
#define FanOff()                           LATBbits.LATB5 = 0b0 

extern RelayFlagTepedef g_RelayActFlg;

void Relay_Init(void);
void RelayAction(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RELAY_H */

