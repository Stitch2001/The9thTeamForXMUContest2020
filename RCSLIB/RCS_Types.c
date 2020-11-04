//@filename: RCS_Types.C
//@date:2012-08-17
//@author:柯国霖
//@brief: 一些类型定义
#include "RCS_Types.h"

uint32_t GetRCS_RCC_APB2Periph_ADC(ADC_TypeDef* ADCx)
{
	if (ADCx == ADC1)    return RCC_APB2Periph_ADC1;
	else if (ADCx == ADC2)    return RCC_APB2Periph_ADC2;
	else if (ADCx == ADC3)    return RCC_APB2Periph_ADC3;
	return NULL;
}

uint32_t GetRCS_RCC_AHB1Periph_GPIO(GPIO_TypeDef *_g)
{

	if (_g == GPIOA)    return RCC_AHB1Periph_GPIOA;
	else if (_g == GPIOB)    return RCC_AHB1Periph_GPIOB;
	else if (_g == GPIOC)    return RCC_AHB1Periph_GPIOC;
	else if (_g == GPIOD)    return RCC_AHB1Periph_GPIOD;
	else if (_g == GPIOE)    return RCC_AHB1Periph_GPIOE;
	else if (_g == GPIOF)    return RCC_AHB1Periph_GPIOF;
	else if (_g == GPIOG)    return RCC_AHB1Periph_GPIOG;
	else if (_g == GPIOH)    return RCC_AHB1Periph_GPIOH;
	else if (_g == GPIOI)    return RCC_AHB1Periph_GPIOI;
	return NULL;
}
uint8_t GetRCS_EXTI_PortSourceGPIO(GPIO_TypeDef *_g)
{

	if (_g == GPIOA)    return EXTI_PortSourceGPIOA;
	else if (_g == GPIOB)    return EXTI_PortSourceGPIOB;
	else if (_g == GPIOC)    return EXTI_PortSourceGPIOC;
	else if (_g == GPIOD)    return EXTI_PortSourceGPIOD;
	else if (_g == GPIOE)    return EXTI_PortSourceGPIOE;
	else if (_g == GPIOF)    return EXTI_PortSourceGPIOF;
	else if (_g == GPIOG)    return EXTI_PortSourceGPIOG;
	else if (_g == GPIOH)    return EXTI_PortSourceGPIOH;
	else if (_g == GPIOI)    return EXTI_PortSourceGPIOI;
	return NULL;
}

uint16_t  GetRCS_GPIO_PinSource(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return GPIO_PinSource0;
	case GPIO_Pin_1: return GPIO_PinSource1;
	case GPIO_Pin_2: return GPIO_PinSource2;
	case GPIO_Pin_3: return GPIO_PinSource3;
	case GPIO_Pin_4: return GPIO_PinSource4;
	case GPIO_Pin_5: return GPIO_PinSource5;
	case GPIO_Pin_6: return GPIO_PinSource6;
	case GPIO_Pin_7: return GPIO_PinSource7;
	case GPIO_Pin_8: return GPIO_PinSource8;
	case GPIO_Pin_9: return GPIO_PinSource9;
	case GPIO_Pin_10: return GPIO_PinSource10;
	case GPIO_Pin_11: return GPIO_PinSource11;
	case GPIO_Pin_12: return GPIO_PinSource12;
	case GPIO_Pin_13: return GPIO_PinSource13;
	case GPIO_Pin_14: return GPIO_PinSource14;
	case GPIO_Pin_15: return GPIO_PinSource15;
	}
	return NULL;
}

uint8_t GetRCS_EXTI_PinSource(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return EXTI_PinSource0;
	case GPIO_Pin_1: return EXTI_PinSource1;
	case GPIO_Pin_2: return EXTI_PinSource2;
	case GPIO_Pin_3: return EXTI_PinSource3;
	case GPIO_Pin_4: return EXTI_PinSource4;
	case GPIO_Pin_5: return EXTI_PinSource5;
	case GPIO_Pin_6: return EXTI_PinSource6;
	case GPIO_Pin_7: return EXTI_PinSource7;
	case GPIO_Pin_8: return EXTI_PinSource8;
	case GPIO_Pin_9: return EXTI_PinSource9;
	case GPIO_Pin_10: return EXTI_PinSource10;
	case GPIO_Pin_11: return EXTI_PinSource11;
	case GPIO_Pin_12: return EXTI_PinSource12;
	case GPIO_Pin_13: return EXTI_PinSource13;
	case GPIO_Pin_14: return EXTI_PinSource14;
	case GPIO_Pin_15: return EXTI_PinSource15;
	}
	return NULL;
}
uint32_t GetRCS_EXTI_Line(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return EXTI_Line0;
	case GPIO_Pin_1: return EXTI_Line1;
	case GPIO_Pin_2: return EXTI_Line2;
	case GPIO_Pin_3: return EXTI_Line3;
	case GPIO_Pin_4: return EXTI_Line4;
	case GPIO_Pin_5: return EXTI_Line5;
	case GPIO_Pin_6: return EXTI_Line6;
	case GPIO_Pin_7: return EXTI_Line7;
	case GPIO_Pin_8: return EXTI_Line8;
	case GPIO_Pin_9: return EXTI_Line9;
	case GPIO_Pin_10: return EXTI_Line10;
	case GPIO_Pin_11: return EXTI_Line11;
	case GPIO_Pin_12: return EXTI_Line12;
	case GPIO_Pin_13: return EXTI_Line13;
	case GPIO_Pin_14: return EXTI_Line14;
	case GPIO_Pin_15: return EXTI_Line15;
	}
	return NULL;
}
uint16_t GetRCS_EXTI_IRQn(uint32_t _p)
{
	switch (_p)
	{
	case GPIO_Pin_0: return EXTI0_IRQn;
	case GPIO_Pin_1: return EXTI1_IRQn;
	case GPIO_Pin_2: return EXTI2_IRQn;
	case GPIO_Pin_3: return EXTI3_IRQn;
	case GPIO_Pin_4: return EXTI4_IRQn;
	case GPIO_Pin_5: return EXTI9_5_IRQn;
	case GPIO_Pin_6: return EXTI9_5_IRQn;
	case GPIO_Pin_7: return EXTI9_5_IRQn;
	case GPIO_Pin_8: return EXTI9_5_IRQn;
	case GPIO_Pin_9: return EXTI9_5_IRQn;
	case GPIO_Pin_10: return EXTI15_10_IRQn;
	case GPIO_Pin_11: return EXTI15_10_IRQn;
	case GPIO_Pin_12: return EXTI15_10_IRQn;
	case GPIO_Pin_13: return EXTI15_10_IRQn;
	case GPIO_Pin_14: return EXTI15_10_IRQn;
	case GPIO_Pin_15: return EXTI15_10_IRQn;
	}
	return NULL;
}


uint32_t  GetRCS_RCC_APB1Periph_USART(USART_TypeDef *_u)
{
	if ( USART2 == _u) return  RCC_APB1Periph_USART2;
	else if ( USART3 == _u) return  RCC_APB1Periph_USART3;
	else if ( UART4  == _u) return  RCC_APB1Periph_UART4;
	else if ( UART5  == _u) return  RCC_APB1Periph_UART5;
	return NULL;
}
uint32_t  GetRCS_RCC_APB2Periph_USART(USART_TypeDef *_u)
{
	if ( USART1 == _u) return  RCC_APB2Periph_USART1;
	else if ( USART6 == _u) return  RCC_APB2Periph_USART6;
	return NULL;
}


uint8_t  GetRCS_GPIO_CAN_AF(CAN_TypeDef *_u)
{
	if ( CAN1 == _u) return  GPIO_AF_CAN1;
	else if ( CAN2 == _u) return  GPIO_AF_CAN2;
	return NULL;
}

uint8_t  GetRCS_GPIO_AF(USART_TypeDef *_u)
{
	if ( USART1 == _u) return  GPIO_AF_USART1;
	else if ( USART2 == _u) return  GPIO_AF_USART2;
	else if ( USART3 == _u) return  GPIO_AF_USART3;
	else if ( UART4  == _u) return  GPIO_AF_UART4;
	else if ( UART5  == _u) return  GPIO_AF_UART5;
	else if ( USART6 == _u) return  GPIO_AF_USART6;
	return NULL;
}



uint8_t  GetRCS_USART_IRQn(USART_TypeDef *_u)
{
	if ( USART1 == _u) return  USART1_IRQn;
	else if ( USART2 == _u) return  USART2_IRQn;
	else if ( USART3 == _u) return  USART3_IRQn;
	else if ( UART4  == _u) return  UART4_IRQn;
	else if ( UART5  == _u) return  UART5_IRQn;
	else if ( USART6 == _u) return  USART6_IRQn;
	return NULL;
}

uint32_t GetRCS_RCC_APB1Periph_TIM(TIM_TypeDef *_t)
{
	//  if( TIM1  == _t) return RCC_APB1Periph_TIM1;
	if ( TIM2  == _t) return RCC_APB1Periph_TIM2;
	else if ( TIM3  == _t) return RCC_APB1Periph_TIM3;
	else if ( TIM4  == _t) return RCC_APB1Periph_TIM4;
	else if ( TIM5  == _t) return RCC_APB1Periph_TIM5;
	else if ( TIM6  == _t) return RCC_APB1Periph_TIM6;
	else if ( TIM7  == _t) return RCC_APB1Periph_TIM7;
	//  if( TIM8  == _t) return RCC_APB1Periph_TIM8;
	//  if( TIM9  == _t) return RCC_APB1Periph_TIM9;
	//  if( TIM10 == _t) return RCC_APB1Periph_TIM10;
	//  if( TIM11 == _t) return RCC_APB1Periph_TIM11;
	else if ( TIM12 == _t) return RCC_APB1Periph_TIM12;
	else if ( TIM13 == _t) return RCC_APB1Periph_TIM13;
	else if ( TIM14 == _t) return RCC_APB1Periph_TIM14;
	return NULL;
}
uint32_t GetRCS_RCC_APB2Periph_TIM(TIM_TypeDef *_t)
{
	if ( TIM1  == _t) return RCC_APB2Periph_TIM1;
	else if ( TIM8  == _t) return RCC_APB2Periph_TIM8;
	else if ( TIM9  == _t) return RCC_APB2Periph_TIM9;
	else if ( TIM10 == _t) return RCC_APB2Periph_TIM10;
	else if ( TIM11 == _t) return RCC_APB2Periph_TIM11;
	return NULL;
}
uint8_t GetRCS_TIM_IRQn(TIM_TypeDef *_t)
{
	if ( TIM1  == _t) return TIM1_UP_TIM10_IRQn;
	else if ( TIM2  == _t) return TIM2_IRQn;
	else if ( TIM3  == _t) return TIM3_IRQn;
	else if ( TIM4  == _t) return TIM4_IRQn;
	else if ( TIM5  == _t) return TIM5_IRQn;
	else if ( TIM6  == _t) return TIM6_DAC_IRQn;
	else if ( TIM7  == _t) return TIM7_IRQn;
	else if ( TIM8  == _t) return TIM8_UP_TIM13_IRQn;
	else if ( TIM9  == _t) return TIM1_BRK_TIM9_IRQn;
	else if ( TIM10 == _t) return TIM1_UP_TIM10_IRQn;
	else if ( TIM11 == _t) return TIM1_TRG_COM_TIM11_IRQn;
	else if ( TIM12 == _t) return TIM8_BRK_TIM12_IRQn;
	else if ( TIM13 == _t) return TIM8_UP_TIM13_IRQn;
	else if ( TIM14 == _t) return TIM8_TRG_COM_TIM14_IRQn;
	return NULL;
}
uint8_t GetRCS_GPIO_AF_TIM(TIM_TypeDef *_t)
{
	if ( TIM1  == _t) return GPIO_AF_TIM1;
	else if ( TIM2  == _t) return GPIO_AF_TIM2;
	else if ( TIM3  == _t) return GPIO_AF_TIM3;
	else if ( TIM4  == _t) return GPIO_AF_TIM4;
	else if ( TIM5  == _t) return GPIO_AF_TIM5;
	else if ( TIM8  == _t) return GPIO_AF_TIM8;
	else if ( TIM9  == _t) return GPIO_AF_TIM9;
	else if ( TIM10 == _t) return GPIO_AF_TIM10;
	else if ( TIM11 == _t) return GPIO_AF_TIM11;
	else if ( TIM12 == _t) return GPIO_AF_TIM12;
	else if ( TIM13 == _t) return GPIO_AF_TIM13;
	else if ( TIM14 == _t) return GPIO_AF_TIM14;
	return NULL;
}

uint8_t GetRCS_GPIO_AF_SPI(SPI_TypeDef * _s)
{
	if(_s == SPI1) return GPIO_AF_SPI1;
	else if(_s == SPI2) return GPIO_AF_SPI2;
	else if(_s == SPI3) return GPIO_AF_SPI3;
	return NULL;
}

uint32_t GetRCS_RCC_APB1Periph_SPI(SPI_TypeDef * _s)
{
	if(_s == SPI2) return RCC_APB1Periph_SPI2;
	else if(_s == SPI3) return RCC_APB1Periph_SPI3;
	return NULL;
}
uint32_t GetRCS_RCC_APB2Periph_SPI(SPI_TypeDef * _s)
{
	if(_s == SPI1) return RCC_APB2Periph_SPI1;
	return NULL;
}



uint8_t GetRCS_SPI_IRQn(SPI_TypeDef * _s)
{
	if(_s == SPI1) return SPI1_IRQn;
	else if(_s == SPI2) return SPI2_IRQn;
	else if(_s == SPI3) return SPI3_IRQn;
	return NULL;
}

uint32_t GetRCS_RCC_APB1Periph_CAN(CAN_TypeDef* _u)
{
	if( CAN1 == _u) return RCC_APB1Periph_CAN1;
	else if( CAN2 == _u) return RCC_APB1Periph_CAN2;
	return NULL;
}

uint32_t GetRCS_RCC_APB2Periph_I2C(I2C_TypeDef* _u)
{
	if( I2C1 == _u) return RCC_APB1Periph_I2C1;
	else if( I2C2 == _u) return RCC_APB1Periph_I2C2;
	else if( I2C3 == _u) return RCC_APB1Periph_I2C3;
	return NULL;
}

uint8_t  GetRCS_I2C_EV_IRQn(I2C_TypeDef *_u)
{
	if ( I2C1 == _u) return  I2C1_EV_IRQn;
	else if ( I2C2 == _u) return  I2C2_EV_IRQn;
	else if ( I2C3 == _u) return  I2C3_EV_IRQn;
	return NULL;
}

uint8_t GetRCS_I2C_ER_IRQn(I2C_TypeDef *_u)
{
	if( I2C1 == _u) return I2C1_ER_IRQn;
	else if ( I2C2 == _u) return I2C2_ER_IRQn;
	else if ( I2C3 == _u) return I2C3_ER_IRQn;
	return NULL;
}



uint8_t  GetRCS_I2C_AF(I2C_TypeDef *_u)
{
	if ( I2C1 == _u) return  GPIO_AF_I2C1;
	else if ( I2C2 == _u) return  GPIO_AF_I2C2;
	else if ( I2C3 == _u) return  GPIO_AF_I2C3;
	return NULL;
}
