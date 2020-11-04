//@filename: RCS_EXTI.h
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
#ifndef _RCS_EXTI_H_
#define _RCS_EXTI_H_

#include "rcs.h"

void RCS_InitEXTI(GPIO_TypeDef *_port, uint32_t _pin, EXTITrigger_TypeDef _trigger, uint8_t _priority);

#endif  
