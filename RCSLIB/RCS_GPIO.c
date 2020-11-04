//@filename: RCS_GPIO.c
//@date: 2020-7-31
//@author: 陈志伟
//@brief: 常规GPIO配置
#include "RCS_GPIO.h"

//管脚输出初始化
//@name: RCS_GPIO_Output_Init
//@brief: 管脚输出初始化
//@param:GPIO_TypeDef *GPIOx GPIO组
//@param:uint32_t   GPIO_Pin 具体管脚号
void RCS_GPIO_Output_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	//开启GPIO时钟
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(GPIOx), ENABLE);
	//推挽输出模式，速度100MHZ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;			//输出模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;						//引脚
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			//推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHZ
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}


//@name: RCS_GPIO_Input_Init
//@brief: 管脚输入初始化
//@param:GPIO_TypeDef *GPIOx GPIO组
//@param:uint32_t   GPIO_Pin 具体管脚号
void RCS_GPIO_Input_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;				//创建初始化结构体
	
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(GPIOx), ENABLE);
	//上拉输入模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;				//输入模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;						//引脚
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	//100MHZ
	
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}


//@name: RCS_GPIO_Set
//@brief: 管脚置高
//@param:GPIO_TypeDef *GPIOx GPIO组
//@param:uint32_t   GPIO_Pin 具体管脚号
void RCS_GPIO_Set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
		GPIO_SetBits(GPIOx, GPIO_Pin);
}

//@name: RCS_GPIO_Reset
//@brief: 管脚置低
//@param:GPIO_TypeDef *GPIOx GPIO组
//@param:uint32_t   GPIO_Pin 具体管脚号
void RCS_GPIO_Reset(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_ResetBits(GPIOx, GPIO_Pin);
}


//@name: RCS_GPIO_Toggle
//@brief: 反置管脚
//@param:GPIO_TypeDef *GPIOx GPIO组
//@param:uint32_t   GPIO_Pin 具体管脚号
void RCS_GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if (GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin) == Bit_SET)
		RCS_GPIO_Reset(GPIOx, GPIO_Pin);
	else 
		RCS_GPIO_Set(GPIOx, GPIO_Pin);
}

//@name: RCS_GPIO_Read
//@brief: 读取管脚电平高低(返回值 高1、低0)
//@param:GPIO_TypeDef *GPIOx GPIO组
//@param:uint32_t   GPIO_Pin 具体管脚号
uint8_t RCS_GPIO_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}
