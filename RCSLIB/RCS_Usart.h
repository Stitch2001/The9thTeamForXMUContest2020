//@filename:RCS_usart.h
//@author:  Morris Zhang
//@date:    18-August-2012
//@brief: USARTͨѶģ��
#ifndef _RCS_USART_H_
#define _RCS_USART_H_

#include "rcs.h"

//@name: USART_Config
//@brief: ����USART
//@param: _USARTx          USART��
//@param: _GPIOx           GPIO��
//@param: _GPIO_PinX_T     ���͹ܽź�
//@param:_GPIO_PinX_R      ���չ̺ܽ�
//@param:_baudRate         ������
//@param: _pri             ���ȼ���ǰ����ռ��������Ӧ
//@note:�жϱ�־λ�������
//USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
void RCS_USART_Config(USART_TypeDef *_USARTx, GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_T,
											uint32_t _GPIO_PinX_R, uint32_t _baudRate, uint8_t _pri);


//@name: RCS_USART_Send_Char
//@brief: ���ڷ���һ���ַ�
//@param:_USARTx     USART��
//@param:_character  �ַ�

__inline void RCS_USART_Send_Char( USART_TypeDef *_USARTx, uint8_t _character)
{
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	USART_SendData(_USARTx, (uint16_t)_character & 0x00ff);
}
//@name: RCS_USART_Send_Str
//@brief: ���ڷ����ַ���
//@param:_USARTx   USART��
//@param:_TxBuffer �ַ���
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
//@brief: ���ڷ�������
//@param:_USARTx USART��
//@param:address ���ݵ�ͷָ��
//@param:length  ���ݳ���
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
//@brief: ���ڽ����ַ�
//@param:_USARTx USART��
//@retval: uint16λ���ַ�
__inline uint16_t RCS_USART_Accept_Char(USART_TypeDef *_USARTx)
{
	return USART_ReceiveData(_USARTx);
}
#endif //_RCS_USART_H_
