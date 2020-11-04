#include "LED_LIB.h"

//@name: LED_Blink
//@brief: LED��˸������Ϊ2s
//@param:GPIO_TypeDef *GPIOx GPIO��
//@param:uint32_t   GPIO_Pin ����ܽź�
void LED_Blink(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
		RCS_GPIO_Reset(GPIOx, GPIO_Pin);
		delay_ms(1000);
		RCS_GPIO_Set(GPIOx, GPIO_Pin);
		delay_ms(1000);
}
