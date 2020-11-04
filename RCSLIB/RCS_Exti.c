//@filename: RCS_EXTI.c
//@date: 2012-08-17
//@author: 郑文凯&&魏闻
//@brief: 外部中断模块
//修改历史
//2012-08-17    23:30   柯国霖   修改函数名 参数名 文件名
//2012-08-18    01:42   高震宙   稍微修改了代码格式
//2012-08-18    09:30   柯国霖   同步注释和代码,修改时间格式,建议改为 yyyy-mm-dd
//2012-08-19    21:48   高震宙   修改了文件名
//2012-08-25    22:00   柯国霖   大量修改,使用函数封装
//2012-10-14    14:00   柯国霖   添加中断优先级的定义
//2012-12-08    17:00   柯国霖   增加入口参数检查,规范注释
#include "RCS_Exti.h"

//外部中断初始化
//@param:_port 中断port口
//@param:_pin 中断管脚
//@param:_trigger 中断触发模式：EXTI_Trigger_Rising、EXTI_Trigger_Falling or EXTI_Trigger_Rising_Falling
//@param:uint8_t _priority 中断优先级:前四位抢占优先级,后四位响应优先级

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
