/* 
 * File:   tempcontrol.h
 * Author: Administrator
 *
 * Created on 2016��7��13��, ����6:57
 */

#ifndef TEMPCONTROL_H
#define	TEMPCONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "pic18.h"


 //============================= Macro definition ==================================
//��ֲ��ʱ�������Ҫ�޸����º궨��
 

#define FanOn()                            LATBbits.LATB5 = 1      
#define FanOff()                           LATBbits.LATB5 = 0 


//============================ function prototypes ================================ 

void Thermal_Init(void);
void TskTherMgt(void);


//=================================================================================   
    
    

#ifdef	__cplusplus
}
#endif

#endif	/* TEMPCONTROL_H */

