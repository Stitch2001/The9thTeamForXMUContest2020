//@filename:RCS_usart.h
//@author:  Morris Zhang
//@date:    18-August-2012
//@brief: USART通讯模块
#ifndef _RCS_USART_H_
#define _RCS_USART_H_

#include "rcs.h"

//@name: USART_Config
//@brief: 配置USART
//@param: _USARTx          USART号
//@param: _GPIOx           GPIO组
//@param: _GPIO_PinX_T     发送管脚号
//@param:_GPIO_PinX_R      接收管教号
//@param:_baudRate         波特率
//@param: _pri             优先级：前四抢占，后四响应
//@note:中断标志位清除函数
//USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
void RCS_USART_Config(USART_TypeDef *_USARTx, GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_T,
											uint32_t _GPIO_PinX_R, uint32_t _baudRate, uint8_t _pri);


//@name: RCS_USART_Send_Char
//@brief: 串口发送一个字符
//@param:_USARTx     USART号
//@param:_character  字符

__inline void RCS_USART_Send_Char( USART_TypeDef *_USARTx, uint8_t _character)
{
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	USART_SendData(_USARTx, (uint16_t)_character & 0x00ff);
}
//@name: RCS_USART_Send_Str
//@brief: 串口发送字符串
//@param:_USARTx   USART号
//@param:_TxBuffer 字符串
__inline void RCS_USART_Send_Str(USART_TypeDef *_USARTx, uint8_t _TxBuffer[])
{
	uint8_t i = 0;
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	for (; _TxBuffer[i] != '\0'; i++)
	{
		USART_SendData(_USARTx, (uint16_t)_TxBuffer[i]);
		while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	}
}
//@name: RCS_USART_Send_Data
//@brief: 串口发送数据
//@param:_USARTx USART号
//@param:address 数据的头指针
//@param:length  数据长度
__inline void RCS_USART_Send_Data(USART_TypeDef *_USARTx, uint8_t *address, uint8_t length)
{
	uint8_t i = 0;
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	for (; i < length ; i++)
	{
		USART_SendData(_USARTx, (uint16_t) * (address + i));
		while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	}
}
//@name: RCS_USART_Accept_Char
//@brief: 串口接受字符
//@param:_USARTx USART号
//@retval: uint16位的字符
__inline uint16_t RCS_USART_Accept_Char(USART_TypeDef *_USARTx)
{
	return USART_ReceiveData(_USARTx);
}
#endif //_RCS_USART_H_
