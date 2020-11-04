//@filename: RCS_pwm.h
//@date:2012-08-18
//@author:黄润东
//@brief: 方波输出，PWM模块
//计算公式
//  TimClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency / 2);TimClk 频率等于PCLK2的频率
//  PrescalerValue = (uuint32_t16_t) (TimClk / _CLKHZ ) - 1; 计数器频率由分频控制
//  ARR = (uint32_t)(_CLKHZ / _PWMHZ); ARR 控制输出频率
//  CCRValue = (uint32_t)(_percent * ARR / 100); CCR 控制占空比

#ifndef __RCS_PWM__
#define __RCS_PWM__

#include "rcs.h"

//@name: PWMInit,周期必须小于65535
//@brief: PWM初始化
//@param:TIM_TypeDef * _TIM : 定时器号
//@param:uint8_t _ch  :定时器 pwm输出 通道号
//@param:GPIO_TypeDef* _port :pwm输出的GPIO组
//@param:uint32_t _pin : pwm输出管脚
//@param:uint32_t _CLKHZ : 定时器计数器频率,必须能被计数器时钟频率(84MHZ)整除
//@param:uint32_t _PWMHZ : pwm输出频率
//PWM初始化成功就返回TRUE
bool PWMInit(TIM_TypeDef *_TIM, uint8_t _ch,
				GPIO_TypeDef   *_port, uint32_t _pin,
				uint32_t _CLKHZ, uint32_t _PWMHZ);


bool PWMGetIn_Period_Duty(TIM_TypeDef *_TIM, uint8_t _ch,
                             GPIO_TypeDef   *_port, uint32_t _pin, uint8_t _pri);

//@brief:获取定时器周期
//@param:   TIM_TypeDef * _TIM 使用定时器号
__inline uint32_t PWMGetPeriod(TIM_TypeDef *_TIM)
{
	uint32_t ARR = _TIM->ARR + 1;
	return ARR;
}


//@name: PWMOutput
//@brief: 在输出频率不变的情况下改变pwm的占空比
//@param:TIM_TypeDef * _TIM 定时器号
//@param:uint8_t _ch,定时器的PWM的输出通道号
//@param:double _percent 占空比0~1
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
//@brief:获取pwm的占空比
//@param:TIM_TypeDef * _TIM 定时器号
//@param:uint8_t _ch,PWM输入的定时器通道号
//@retval: pwm占空比,返回范围0-1
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
