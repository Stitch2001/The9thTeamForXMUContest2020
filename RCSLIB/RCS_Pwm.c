//@filename: RCS_pwm.C
//@date:2012-08-18
//@author:黄润东
//@brief: 方波输出，PWM模块
//计算公式
//  TimClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency / 2);TimClk 频率等于PCLK2的频率
//  PrescalerValue = (uuint32_t16_t) (TimClk / _CLKHZ ) - 1; 计数器频率由分频控制
//  ARR = (uint32_t)(_CLKHZ / _PWMHZ); ARR 控制输出频率
//  CCRValue = (uint32_t)(_percent * ARR / 100); CCR 控制占空比
//
#include "RCS_Pwm.h"

//@name: PWMInit,周期必须小于65535
//@brief: PWM初始化
//@param:TIM_TypeDef * _TIM : 定时器号
//@param:uint8_t _ch  :定时器 pwm输出 通道号
//@param:GPIO_TypeDef* _port :pwm输出的GPIO组
//@param:uint32_t _pin : pwm输出管脚
//@param:uint32_t _CLKHZ : 定时器计数器频率,必须能被计数器时钟频率(84MHZ)整除（一般取1MHZ(1000000)）
//@param:uint32_t _PWMHZ : pwm输出频率
//PWM初始化成功就返回TRUE
bool PWMInit(TIM_TypeDef *_TIM, uint8_t _ch,
                GPIO_TypeDef   *_port, uint32_t _pin,
                uint32_t _CLKHZ, uint32_t _PWMHZ)
{
		uint32_t PrescalerValue;
		uint32_t TimClk;
		uint32_t ARR;
		uint32_t CCRValue;	
	
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
	
    assert_param(IS_TIM_ALL_PERIPH(_TIM));
    assert_param(IS_GPIO_ALL_PERIPH(_port));
    assert_param(IS_GPIO_PIN(_pin));
    if (_TIM == TIM6 || _TIM == TIM7)
    {
        return false;
    }
    RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_port), ENABLE);

    if (_TIM == TIM1 ||  _TIM == TIM8 || _TIM == TIM9 || _TIM == TIM10 || _TIM == TIM11 )
    {
        RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_TIM(_TIM), ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(GetRCS_RCC_APB1Periph_TIM(_TIM), ENABLE);
    }
    GPIO_PinAFConfig(_port, GetRCS_GPIO_PinSource(_pin), GetRCS_GPIO_AF_TIM(_TIM));
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = _pin ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(_port, &GPIO_InitStructure);

    GPIO_PinAFConfig(_port, GetRCS_GPIO_PinSource(_pin) , GetRCS_GPIO_AF_TIM(_TIM));

    TimClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency / 2);
    if (_TIM == TIM1 ||  _TIM == TIM8 || _TIM == TIM9 || _TIM == TIM10 || _TIM == TIM11 )
    {
        TimClk = (uint32_t ) (rcc_clocks.SYSCLK_Frequency);
    }
    PrescalerValue = (uint16_t) (TimClk / _CLKHZ) - 1;
    ARR = (uint32_t)(_CLKHZ / _PWMHZ);
    assert_param( ARR <= 65536 );
    CCRValue = (uint32_t)( 0.5 * ARR );

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ARR - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = CCRValue;
    if (_TIM == TIM1 || _TIM == TIM8)
    {
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
        TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
        TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    }
    //选择输出通道，不同通道对应不同管脚
    if (_ch == 1)
    {
        TIM_OC1Init(_TIM, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(_TIM, TIM_OCPreload_Enable);
    }
    else if (_ch == 2)
    {
        TIM_OC2Init(_TIM, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(_TIM, TIM_OCPreload_Enable);
    }
    else if (_ch == 3)
    {
        TIM_OC3Init(_TIM, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(_TIM, TIM_OCPreload_Enable);
    }
    else if (_ch == 4)
    {
        TIM_OC4Init(_TIM, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(_TIM, TIM_OCPreload_Enable);
    }

    //预装载使能
    TIM_ARRPreloadConfig(_TIM, ENABLE);
    /* TIMx 计数器使能 */
    TIM_Cmd(_TIM, ENABLE);
    if (_TIM == TIM1 ||  _TIM == TIM8)
    {
        TIM_CtrlPWMOutputs(_TIM, ENABLE);
    }

    return true;
}


uint8_t Measure_ch;
double frequency, DutyCycle;


//TIM3测量PWM输入中断函数(使用时将改函数名映射成对应的TIM中断函数名)
void Measure_IRQHandler(void)
{
    uint32_t IC2Value = 0;
    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
    {
        IC2Value = TIM_GetCapture2(TIM3);              //读取IC2补获寄存器的值,即为PWM周期的计数值
        if (IC2Value != 0)
        {
            DutyCycle = (TIM_GetCapture1(TIM3) * 100) / IC2Value; //读取IC1捕获寄存器的值,并计算占空比
            frequency = 72000000 / IC2Value;      //计算PWM频率
        }
        else
        {
            DutyCycle = 0;
            frequency = 0;
        }
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);       //清除TIM中断标志位
}
//@name: PWMGetIn_Period_Duty
//@brief: 测量输入pwm的频率和占空比
//@param:TIM_TypeDef * _TIM : 定时器号
//@param:uint8_t _ch  :定时器 pwm输出通道号
//@param:GPIO_TypeDef* _port :pwm输入GPIO号
//@param:uint32_t _pin : 输入的管脚号
//@retval: 初始化成功则返回TRUE
bool PWMGetIn_Period_Duty(TIM_TypeDef *_TIM, uint8_t _ch,
                             GPIO_TypeDef   *_port, uint32_t _pin, uint8_t _pri)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef nvic_initStruct;	

    Measure_ch = _ch;
    assert_param(IS_TIM_ALL_PERIPH(_TIM));
    if (_TIM == TIM6 || _TIM == TIM7 || _TIM == TIM9 || _TIM == TIM10 || _TIM == TIM11 || _TIM == TIM12 || _TIM == TIM13 || _TIM == TIM14)
    {
        return false;
    }
    RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_port), ENABLE);

    if (_TIM == TIM1 ||  _TIM == TIM8 || _TIM == TIM9 || _TIM == TIM10 || _TIM == TIM11 )
    {
        RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_TIM(_TIM), ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(GetRCS_RCC_APB1Periph_TIM(_TIM), ENABLE);
    }


    GPIO_InitStructure.GPIO_Pin = _pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(_port, &GPIO_InitStructure);


    nvic_initStruct.NVIC_IRQChannel = GetRCS_TIM_IRQn(_TIM);
    nvic_initStruct.NVIC_IRQChannelPreemptionPriority = (_pri >> 4 ) & 0x0f;
    nvic_initStruct.NVIC_IRQChannelSubPriority = _pri & 0x0f;
    nvic_initStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initStruct);


    TIM_ICInitStructure.TIM_Channel = _ch;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;       //滤波设置,经历几个周期跳变认定波形稳定0x0~0xF

    TIM_PWMIConfig(_TIM, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(_TIM, TIM_TS_TI2FP2);                //选择IC2为时钟触发源
    TIM_SelectSlaveMode(_TIM, TIM_SlaveMode_Reset);
    //TIM从模式:触发信号的上升沿重新初始化计数器和触发寄存器的更新事件
    TIM_SelectMasterSlaveMode(_TIM, TIM_MasterSlaveMode_Enable); //启动定时器的被动触发
    TIM_Cmd(_TIM, ENABLE);
    TIM_ITConfig(_TIM, TIM_IT_CC2, ENABLE);     //打开中断
    return true;
}

