//@filename: RCS_EXTI.c
//@date: 2012-08-17
//@author: ֣�Ŀ�&&κ��
//@brief: �ⲿ�ж�ģ��
//�޸���ʷ
//2012-08-17    23:30   �¹���   �޸ĺ����� ������ �ļ���
//2012-08-18    01:42   ������   ��΢�޸��˴����ʽ
//2012-08-18    09:30   �¹���   ͬ��ע�ͺʹ���,�޸�ʱ���ʽ,�����Ϊ yyyy-mm-dd
//2012-08-19    21:48   ������   �޸����ļ���
//2012-08-25    22:00   �¹���   �����޸�,ʹ�ú�����װ
//2012-10-14    14:00   �¹���   ����ж����ȼ��Ķ���
//2012-12-08    17:00   �¹���   ������ڲ������,�淶ע��
#include "RCS_Exti.h"

//�ⲿ�жϳ�ʼ��
//@param:_port �ж�port��
//@param:_pin �жϹܽ�
//@param:_trigger �жϴ���ģʽ��EXTI_Trigger_Rising��EXTI_Trigger_Falling or EXTI_Trigger_Rising_Falling
//@param:uint8_t _priority �ж����ȼ�:ǰ��λ��ռ���ȼ�,����λ��Ӧ���ȼ�

void RCS_InitEXTI(GPIO_TypeDef *_port, uint32_t _pin, EXTITrigger_TypeDef _trigger, uint8_t _priority)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	assert_param(IS_GPIO_ALL_PERIPH(_port));
	assert_param(IS_GPIO_PIN(_pin));
	assert_param(IS_EXTI_TRIGGER(_trigger));
	
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_port), ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure  pin as input floating */
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	if (_trigger == EXTI_Trigger_Rising)
	{
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	}
	else
	{
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	}
	GPIO_InitStructure.GPIO_Pin = _pin;
	GPIO_Init(_port, &GPIO_InitStructure);

	/* Connect EXTI Line topin */
	SYSCFG_EXTILineConfig(GetRCS_EXTI_PortSourceGPIO(_port), GetRCS_EXTI_PinSource(_pin));

	/* Configure EXTI Line */
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = GetRCS_EXTI_Line(_pin);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = _trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = GetRCS_EXTI_IRQn(_pin);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (_priority >> 4) & 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _priority & 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
