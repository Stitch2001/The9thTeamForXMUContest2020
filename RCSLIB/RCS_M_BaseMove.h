/*@filename: RCS_M_BaseMove.h
 *@author     ��־ΰ       
 *@brief:     X�����ֵ����ٶȽ���
 *@date: 2020-7-17
*/
#ifndef _RCS_M_BASEMOVE_H_
#define _RCS_M_BASEMOVE_H_

#include "rcs.h"		
/***********���̹��(mm)************/
#define BASE_LENGTH     300.0f  //���̳�
#define BASE_WIDTH			385.0f	//��
#define RAD_WHEEL				76.2f	//���Ӱ뾶
#define SLOWDOWN_RATE	  19.0f	  //������ٱ�
#define MM_S2R_MIN			0.1772276212907689f	// ������ٶ�(mm/s)ת��������ת��(r/min)�����㷽����60/(2*RAD_WHEEL*pi*cos(45��))

void M_BaseMove_Init(void);			//���̳�ʼ��


void BaseMove(float move_speed,float move_direction,float rotate_speed,
												float target_angle,int feedback_flag,float target_pos_x,
												float target_pos_y);			//�������㷨
												
#endif
