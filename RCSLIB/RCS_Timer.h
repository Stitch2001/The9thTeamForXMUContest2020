//@filename: Timer.c
//@date: 2012-08-16
//@author: ����ѩ
//@brief: ��ʱ�ж�
#ifndef RCS_TIMER_H_
#define RCS_TIMER_H_

#include "rcs.h"

//@name: InitTimerInt
//@brief: ��ʱ�ж�
//@param:TIM_TypeDef * _TIM ��ʱ����
//@param:uint32_t _TIM_period ��������
//@param:uint32_t  _div ��1s�ָ�_div����λ,�ָ����������3000,�����Ƶ���
//@param:uint8_t _priority :ǰ4��ռ���ȼ�,��4��Ӧ���ȼ�
//@notes:����жϱ�־λ
//TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
//���ڼ���Ϊ _TIM_period/_div ��
void InitTimerInt(TIM_TypeDef *_TIM, uint32_t _TIM_period, uint32_t  _div,
									uint8_t _priority);

//@name: StopTimer
//@brief: ֹͣ��ʱ�ж�
//@param:TIM_TypeDef * _TIM ��ʱ����
void StopTimer(TIM_TypeDef *_TIM );

#endif
