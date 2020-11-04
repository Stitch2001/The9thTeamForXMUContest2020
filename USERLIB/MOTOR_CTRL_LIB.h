#ifndef _MOTOR_LIB_H_
#define _MOTOR_LIB_H_

#include "rcs.h"		

#define LIMIT_ERR							2500
#define	LIMIT_SPEED						8000
#define LIMIT_FEEDBACK_SPEED 	8000
#define LIMIT_ROTATE_SPEED    8000

//����PID�ṹ��
typedef struct {
float P;
float I;
float D;
}PID_Struct2;

//��������
float M3508_Angle_Init();		//M3508�Ƕȳ�ʼ��
void M3508_angle_Ctrl(float target_angle,float init_angle);		//���Ƶ����ĽǶȣ��������ΪĿ��Ƕ�
int32_t PID_MotorAngle_Ctrl(float target_angle,float motor_angle,PID_Struct2 _pid);		//PID�Ƕȿ���

#endif
