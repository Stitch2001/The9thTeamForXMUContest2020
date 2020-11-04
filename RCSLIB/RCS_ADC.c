//@filename: ADC.c
//@date: 2019-07-22
//@author: 闫锐
//@brief: 模数转换

#include "RCS_ADC.h"

//@name: RCS_ADC_Init
//@brief: ADC初始化
//@param:ADCx : ADC1 、ADC2 or ADC3
//@param:GPIO_Pin ADC管脚
//@param:ADC_Channel ADC通道
void RCS_ADC_Init(ADC_TypeDef* ADCx, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t ADC_Channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(GPIOx), ENABLE); //GPIO时钟使能
	RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_ADC(ADCx), ENABLE); 		//ADC时钟使能

	/*端口设置为模拟输入*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; /*模拟输入*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; /*浮空输入*/
	GPIO_Init(GPIOx, &GPIO_InitStructure);/*初始化*/

	/*设置通用控制寄存器*/

	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;/*DMA失能*/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; /*独立模式*/
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;/*设置ADC的频率为APB2/4 即84/4=21M*/
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;/*采样时间间隔*/
	ADC_CommonInit(&ADC_CommonInitStructure);/*初始化*/

	/*ADC的初始化*/

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;/*12位模式*/
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;/*非扫描模式*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;/*关闭连续转换*/
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*禁止触发检测 使用软件触发*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;/*右对齐*/
	ADC_InitStructure.ADC_NbrOfConversion = 1;/*只使用1个通道采样*/
	ADC_Init(ADCx, &ADC_InitStructure); /*初始化*/
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_480Cycles); /*设置规则通道3 一个序列 采样时间 */
	ADC_Cmd(ADCx, ENABLE); /*开启ADC*/
}

//ADC获取值
uint16_t RCS_Get_ADC(ADC_TypeDef* ADCx )
{
	ADC_SoftwareStartConv(ADCx);/*启动软件转换*/
	while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC)); /*等待转换结束*/
	return ADC_GetConversionValue(ADCx);/*读取转换结果*/
}

//ADC值转换
float RCS_Get_Voltage(ADC_TypeDef* ADCx)
{
	return (float)(RCS_Get_ADC(ADCx) * VOLTAGE_REFERANCE / MAX_RANGE);
}
