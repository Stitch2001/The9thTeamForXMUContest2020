//@filename: RCS_pwm.h
//@date:2012-08-18
//@author:����
//@brief: ���������PWMģ��
//���㹫ʽ
//  TimClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency / 2);TimClk Ƶ�ʵ���PCLK2��Ƶ��
//  PrescalerValue = (uuint32_t16_t) (TimClk / _CLKHZ ) - 1; ������Ƶ���ɷ�Ƶ����
//  ARR = (uint32_t)(_CLKHZ / _PWMHZ); ARR �������Ƶ��
//  CCRValue = (uint32_t)(_percent * ARR / 100); CCR ����ռ�ձ�

#ifndef __RCS_PWM__
#define __RCS_PWM__

#include "rcs.h"

//@name: PWMInit,���ڱ���С��65535
//@brief: PWM��ʼ��
//@param:TIM_TypeDef * _TIM : ��ʱ����
//@param:uint8_t _ch  :��ʱ�� pwm��� ͨ����
//@param:GPIO_TypeDef* _port :pwm�����GPIO��
//@param:uint32_t _pin : pwm����ܽ�
//@param:uint32_t _CLKHZ : ��ʱ��������Ƶ��,�����ܱ�������ʱ��Ƶ��(84MHZ)����
//@param:uint32_t _PWMHZ : pwm���Ƶ��
//PWM��ʼ���ɹ��ͷ���TRUE
bool PWMInit(TIM_TypeDef *_TIM, uint8_t _ch,
				GPIO_TypeDef   *_port, uint32_t _pin,
				uint32_t _CLKHZ, uint32_t _PWMHZ);


bool PWMGetIn_Period_Duty(TIM_TypeDef *_TIM, uint8_t _ch,
                             GPIO_TypeDef   *_port, uint32_t _pin, uint8_t _pri);

//@brief:��ȡ��ʱ������
//@param:   TIM_TypeDef * _TIM ʹ�ö�ʱ����
__inline uint32_t PWMGetPeriod(TIM_TypeDef *_TIM)
{
	uint32_t ARR = _TIM->ARR + 1;
	return ARR;
}


//@name: PWMOutput
//@brief: �����Ƶ�ʲ��������¸ı�pwm��ռ�ձ�
//@param:TIM_TypeDef * _TIM ��ʱ����
//@param:uint8_t _ch,��ʱ����PWM�����ͨ����
//@param:double _percent ռ�ձ�0~1
__inline void PWMOutput(TIM_TypeDef *_TIM, uint8_t _ch, double _percent)
{
	uint32_t ARR;
	uint32_t CCRValue;
	assert_param(_percent < 1 && _percent >= 0);
	ARR = PWMGetPeriod(_TIM);
	CCRValue = (uint32_t)(_percent * ARR);
	if (_ch == 1)
	{
		_TIM -> CCR1 = CCRValue;
	}
	else if (_ch == 2)
	{
		_TIM -> CCR2 = CCRValue;
	}
	else if (_ch == 3)
	{
		_TIM -> CCR3 = CCRValue;
	}
	else if (_ch == 4)
	{
		_TIM -> CCR4 = CCRValue;
	}
}

//@name: PWMGetPercent
//@brief:��ȡpwm��ռ�ձ�
//@param:TIM_TypeDef * _TIM ��ʱ����
//@param:uint8_t _ch,PWM����Ķ�ʱ��ͨ����
//@retval: pwmռ�ձ�,���ط�Χ0-1
__inline double PWMGetPercent(TIM_TypeDef *_TIM, uint8_t _ch)
{
	uint32_t ARR = PWMGetPeriod(_TIM);
	if (_ch == 1)
	{
		return (double)(_TIM -> CCR1) / (double)ARR;
	}
	else if (_ch == 2)
	{
		return (double)(_TIM -> CCR2) / (double)ARR;
	}
	else if (_ch == 3)
	{
		return (double)(_TIM -> CCR3) / (double)ARR;
	}
	else if (_ch == 4)
	{
		return (double)(_TIM -> CCR4) / (double)ARR;
	}
	return NULL;
}

#endif
