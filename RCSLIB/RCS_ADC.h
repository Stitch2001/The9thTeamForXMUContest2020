//@filename: ADC.h
//@date: 2019-07-22
//@author: ����
//@brief: ģ��ת��

#ifndef _RCS_ADC_H_
#define _RCS_ADC_H_

#include "rcs.h"

#define VOLTAGE_REFERANCE	3.3//��������ѹ
#define	MAX_RANGE			4096//ת������

void RCS_ADC_Init(ADC_TypeDef* ADCx, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t ADC_Channel);
uint16_t RCS_Get_ADC(ADC_TypeDef* ADCx);
float RCS_Get_Voltage(ADC_TypeDef* ADCx);

#endif
