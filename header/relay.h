/* 
 * File:   relay.h
 * Author: Administrator
 *
 * Created on 2016��7��1��, ����10:49
 */

#ifndef RELAY_H
#define	RELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"
//---------------------------- Macro definition ------------------------------
//��ֲ��ʱ�������Ҫ�޸����º궨��
//------------------------- Control Pin definition ---------------------------
#define  RELAY_POSI_CTRL                LATCbits.LATC0     // �����������̵������ƹܽ�
#define  RELAY_NEGA_CTRL                LATAbits.LATA2     // �������ظ��̵������ƹܽ�
#define  RELAY_PREC_CTRL                LATBbits.LATB0     // ����Ԥ���̵������ƹܽ�
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

