//@filename: ADC.c
//@date: 2019-07-22
//@author: ����
//@brief: ģ��ת��

#include "RCS_ADC.h"

//@name: RCS_ADC_Init
//@brief: ADC��ʼ��
//@param:ADCx : ADC1 ��ADC2 or ADC3
//@param:GPIO_Pin ADC�ܽ�
//@param:ADC_Channel ADCͨ��
void RCS_ADC_Init(ADC_TypeDef* ADCx, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t ADC_Channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(GPIOx), ENABLE); //GPIOʱ��ʹ��
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_ADC(ADCx), ENABLE); 		//ADCʱ��ʹ��

	/*�˿�����Ϊģ������*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; /*ģ������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; /*��������*/
	GPIO_Init(GPIOx, &GPIO_InitStructure);/*��ʼ��*/

	/*����ͨ�ÿ��ƼĴ���*/

	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;/*DMAʧ��*/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; /*����ģʽ*/
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;/*����ADC��Ƶ��ΪAPB2/4 ��84/4=21M*/
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;/*����ʱ����*/
	ADC_CommonInit(&ADC_CommonInitStructure);/*��ʼ��*/

	/*ADC�ĳ�ʼ��*/

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;/*12λģʽ*/
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;/*��ɨ��ģʽ*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;/*�ر�����ת��*/
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*��ֹ������� ʹ���������*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;/*�Ҷ���*/
	ADC_InitStructure.ADC_NbrOfConversion = 1;/*ֻʹ��1��ͨ������*/
	ADC_Init(ADCx, &ADC_InitStructure); /*��ʼ��*/
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_480Cycles); /*���ù���ͨ��3 һ������ ����ʱ�� */
	ADC_Cmd(ADCx, ENABLE); /*����ADC*/
}

//ADC��ȡֵ
uint16_t RCS_Get_ADC(ADC_TypeDef* ADCx )
{
	ADC_SoftwareStartConv(ADCx);/*�������ת��*/
	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC)); /*�ȴ�ת������*/
	return ADC_GetConversionValue(ADCx);/*��ȡת�����*/
}

//ADCֵת��
float RCS_Get_Voltage(ADC_TypeDef* ADCx)
{
	return (float)(RCS_Get_ADC(ADCx) * VOLTAGE_REFERANCE / MAX_RANGE);
}
