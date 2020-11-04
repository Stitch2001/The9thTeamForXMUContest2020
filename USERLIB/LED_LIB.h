#ifndef _LED_LIB_H_	//注意这个格式都要大写
#define _LED_LIB_H_

#include "rcs.h"		

//函数申明
void LED_Blink(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
