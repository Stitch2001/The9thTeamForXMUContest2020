//@filename: Timer.c
//@date: 2012-08-16
//@author: 李万雪
//@brief: 定时中断
#ifndef RCS_TIMER_H_
#define RCS_TIMER_H_

#include "rcs.h"

//@name: InitTimerInt
//@brief: 定时中断
//@param:TIM_TypeDef * _TIM 定时器号
//@param:uint32_t _TIM_period 计数周期
//@param:uint32_t  _div 将1s分割_div个单位,分割数必须大于3000,否则分频溢出
//@param:uint8_t _priority :前4抢占优先级,后4响应优先级
//@notes:清除中断标志位
//TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
//周期计算为 _TIM_period/_div 秒
void InitTimerInt(TIM_TypeDef *_TIM, uint32_t _TIM_period, uint32_t  _div,
									uint8_t _priority);

//@name: StopTimer
//@brief: 停止定时中断
//@param:TIM_TypeDef * _TIM 定时器号
void StopTimer(TIM_TypeDef *_TIM );

#endif
