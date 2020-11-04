//@filename: Timer.c
//@date: 2012-08-16
//@author: ����ѩ
//@brief: ��ʱ�ж�
#include "RCS_Timer.h"
//#include "stm32f4xx_conf.h"

//@name: InitTimerInt
//@brief: ��ʱ�ж�
//@param:TIM_TypeDef * _TIM ��ʱ����
//@param:uint32_t _TIM_period ��������
//@param:uint32_t  _div ��1s�ָ�_div����λ,�ָ����������3000,�����Ƶ���
//@param:uint8_t _priority :ǰ4��ռ���ȼ�,��4��Ӧ���ȼ�
//@notes:����жϱ�־λ �жϺ������������
//TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
//���ڼ���Ϊ _TIM_period/_div ��
void InitTimerInt(TIM_TypeDef *_TIM, uint32_t _TIM_period, uint32_t  _div,
				   uint8_t _priority)
{
	uint32_t SysClk;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);

	//TIM_X clock enable
	if (_TIM == TIM1 ||  _TIM == TIM8 || _TIM == TIM9 || _TIM == TIM10 || _TIM == TIM11 )
	{
		RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_TIM(_TIM), ENABLE);
		SysClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency );
	}
	else
	{
		RCC_APB1PeriphClockCmd(GetRCS_RCC_APB1Periph_TIM(_TIM), ENABLE);
		SysClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency / 2);
	}
	assert_param( (uint32_t)(SysClk / (float)_div) <= 65536 );
	//enable the TIM_X global interrupt
	NVIC_InitStructure.NVIC_IRQChannel = GetRCS_TIM_IRQn(_TIM);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (_priority >> 4) & 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _priority & 0x0f;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//set interrupt time _TIM_period
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = _TIM_period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SysClk / (float)_div) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(_TIM, &TIM_TimeBaseStructure);
	TIM_ITConfig(_TIM, TIM_IT_Update, ENABLE);

	//TIM enable counter
	TIM_Cmd(_TIM, ENABLE);
}
//@name: StopTimer
//@brief: ֹͣ��ʱ�ж�
//@param:TIM_TypeDef * _TIM ��ʱ����
void StopTimer(TIM_TypeDef *_TIM )
{
	TIM_Cmd(_TIM, DISABLE);
}


//��ʱ���ж�2
void TIM2_IRQHandler()
{ 
	static int flag = 0;
	if(flag ==0)
	flag = 1;
	else
		flag =0;
	if(flag)
		RCS_GPIO_Reset(LED1_GPIO, LED1_PIN);  			//����һյLED 
	else
		RCS_GPIO_Set(LED1_GPIO, LED1_PIN);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	//����жϱ�־λ
}

//��ʱ���ж�3
void TIM3_IRQHandler()
{
	
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	//����жϱ�־λ
}

//��ʱ���ж�4
void TIM4_IRQHandler()
{
	
	
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	//����жϱ�־λ
}
