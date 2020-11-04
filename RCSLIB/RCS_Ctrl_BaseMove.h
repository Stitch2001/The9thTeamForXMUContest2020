/*@filename: RCS_Ctrl_BaseMove.h
 *@author     ��־ΰ       
 *@brief:    �ۺϿ��Ƶ���
 *@date: 2020-7-17
*/
#ifndef _RCS_CTRL_BASEMOVE_H_
#define _RCS_CTRL_BASEMOVE_H_

#include "rcs.h"		
//�ֶ�����
#define SPEED_EQUIVALENT  	1.515151515151f			//�ٶȵ���1000/��RC_CH_VALUE_MAX(1684) -RC_CH_VALUE_OFFSET(1024)����ÿ��+1000mm/s��
#define ROTATE_EQUIVALENT		2.379994435e-3f			//�Ƕȵ���PI/2/��RC_CH_VALUE_MAX(1684) -RC_CH_VALUE_OFFSET(1024)��������֮��rad/s��
//�Զ�����
#define AUTO_SPEED					8000								//�Զ����Ƶ��ƶ�����ٶ� ������ת��r/min
#define SLOW_DOWN_DISTANCE  2000								//�쵽Ŀ��λ�õļ��پ���mm

void Ctrl_BaseMove_Init(void);    //���Ƶ��̳�ʼ��
void Remote_Ctrl(float init_angle);						//�ֶ�����ģʽ
float Speed_Cal(float now_distance,float max_speed);   //�Զ����ƹ������ٶȼ���
float Angle_Cal(float target_z,float begin_z,float now_distance,float target_distance);//�Զ����ƹ�����ƫ���Ǽ���
void Point2Point_Move(float target_x,float target_y,float begin_x,float begin_y,float target_z,float begin_z);//��Ե��ƶ��Զ�����
#endif


