//@filename: RCS_EXTI.h
//@date: 2012-08-17
//@author: ֣�Ŀ�&&κ��
//@brief: �ⲿ�ж�ģ��
//�޸���ʷ
//2012-08-17    23:30   �¹���   �޸ĺ����� ������ �ļ���
//2012-08-18    01:42   ������   ��΢�޸��˴����ʽ
//2012-08-18    09:30   �¹���   ͬ��ע�ͺʹ���,�޸�ʱ���ʽ,�����Ϊ yyyy-mm-dd
//2012-08-19    21:48   ������   �޸����ļ���
//2012-08-25    22:00   �¹���   �����޸�,ʹ�ú�����װ
//2012-10-14    14:00   �¹���   ����ж����ȼ��Ķ���
//2012-12-08    17:00   �¹���   ������ڲ������,�淶ע��
#ifndef _RCS_EXTI_H_
#define _RCS_EXTI_H_

#include "rcs.h"

void RCS_InitEXTI(GPIO_TypeDef *_port, uint32_t _pin, EXTITrigger_TypeDef _trigger, uint8_t _priority);

#endif  
