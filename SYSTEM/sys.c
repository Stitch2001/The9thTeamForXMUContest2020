#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ϵͳʱ�ӳ�ʼ��	
//����ʱ������/�жϹ���/GPIO���õ�
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
//////////////////////////////////////////////////////////////////////////////////  


//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  

#if defined ( __ICCARM__ )

void WFI_SET(void)
{
	asm("WFI");		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
void INTX_DISABLE(void)
{
	asm("CPSID   I");
	asm("BX      LR");	  
}
//���������ж�
void INTX_ENABLE(void)
{
	asm("CPSIE   I");
	asm("BX      LR");  
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr) 
{
	asm("MSR MSP, r0"); 			//set Main Stack value
	asm("BX r14");
}
#else

__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
#endif














