/* 
 * File:   soc.h
 * Author: Administrator
 *
 * Created on 2016��7��13��, ����3:20
 */

#ifndef SOC_H
#define	SOC_H

#ifdef	__cplusplus
extern "C" {
#endif

// �����ѹ-SOC ���ұ����ݽṹ
typedef struct 
{
   uint16_t  cell_volt;       // Raw ADC, mv
   uint16_t  soc;             // 0.1degC LSB  
}SocLookupTypeDef;

void Soc_Init (void);
void Soc_PowerOnAdjust(void);
void Soc_Update (void);
void Soc_AhAcc (void);
void Soc_CalculateAh(void);
void Soc_StoreSoc(void);
int32_t Soc_ReadAcc(void);
int16_t Soc_ReadAh(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SOC_H */

