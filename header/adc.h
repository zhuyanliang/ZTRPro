/* 
 * File:   adc.h
 * Author: Administrator
 *
 * Created on 2016��6��20��, ����3:50
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"

extern int16_t  g_CurrentOffset; //������У׼ƫ��

//--------------------------- define adc channel -----------------------------
#define AD_Channel_0             ((uint8_t)0x00)
#define AD_Channel_1             ((uint8_t)0x01)
#define AD_Channel_2             ((uint8_t)0x02)
#define AD_Channel_3             ((uint8_t)0x03)
#define AD_Channel_4             ((uint8_t)0x04)
#define AD_Channel_5             ((uint8_t)0x05)
#define AD_Channel_6             ((uint8_t)0x06)
#define AD_Channel_7             ((uint8_t)0x07)
#define AD_Channel_8             ((uint8_t)0x08)
#define AD_Channel_9             ((uint8_t)0x09)
#define AD_Channel_10            ((uint8_t)0x0A)


//��ֲ��ʱ�������Ҫ�޸����º궨��
//---------------------------- ��ģ��������ͨ�� ------------------------------
#define CAHNNEL_ISOVAL			AD_Channel_0 //	��Ե���ͨ��
#define CHANNEL_TBAVAL          AD_Channel_3 // PCB�����¶� 
#define CHANNEL_ILOW            AD_Channel_5 // ����������
#define CHANNEL_IHIGH           AD_Channel_6
#define CHANNEL_THXVAL          AD_Channel_7  // ����������ֵ 
#define CHANNEL_12VD			AD_Channel_8  // ������
#define CHANNEL_5VD				AD_Channel_9 
    
typedef struct {

	// ADCԭʼֵ����������ƽ��ֵ��
   uint16_t AmbTempRaw[8];  	// �洢PCB���ӵ��¶�
   uint16_t TheTempRaw[4][8];   // �洢4·�¶ȴ�������ֵ
   uint16_t CurLowRaw[8];		// �洢��������������
   uint16_t CurHighRaw[8];

	// ����������ָ��  
   uint8_t AmbTempIndex;
   uint8_t TheTempIndex[4];
   uint8_t CurLowIndex;
   uint8_t CurHighIndex;

	// ADC����ƽ��ֵ������Raw����
   uint16_t AmbTempAvg;		// PCB�����¶�
   uint16_t TheTempAvg[4];
   uint16_t CurLowAvg;
   uint16_t CurHighAvg;
}AdcRawTypedef;  

// ADC��ֵ���¶Ȳ��ұ����ݽṹ
typedef struct 
{
   uint16_t  AdcValue;         // Raw ADC, mv
   int8_t    TempValue;        // 1degC LSB  
} ThermLookupTypedef;


const TempWarnClsDef CellCOTThrDefault = {40,  45, 0};
const TempWarnClsDef CellCUTThrDefault = {5,   0,  0};
const TempWarnClsDef CellDOTThrDefault = {60,  65, 0};
const TempWarnClsDef CellDUTThrDefault = {-15,-20, 0};
const TempWarnClsDef CellDLTThrDefault = {3,   5,  0};

const CurrWarnClsDef BattCOCThrDefault = {-220, -250, 0}; // 0.1A
const CurrWarnClsDef BattDOCThrDefault = {2000, 2500, 0}; // 0.1A

extern AdcRawTypedef g_AdcConvertValue;   
    
void ADC_IOInit(void);
void ADC_Convert(uint8_t channel);
uint16_t ADC_GetCvtRaw(void);
uint16_t ADC_GetConvertVal(void);
void CurrentZeroOffsetAdjust(void);
uint16_t ADC_AverageCalculate(uint16_t *TableEntry);
int8_t ADCToTempVal(uint16_t dat);
void TskAdc_Init(void);



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

