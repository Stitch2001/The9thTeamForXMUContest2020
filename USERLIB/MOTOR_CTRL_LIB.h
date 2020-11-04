#ifndef _MOTOR_LIB_H_
#define _MOTOR_LIB_H_

#include "rcs.h"		

#define LIMIT_ERR							2500
#define	LIMIT_SPEED						8000
#define LIMIT_FEEDBACK_SPEED 	8000
#define LIMIT_ROTATE_SPEED    8000

//定义PID结构体
typedef struct {
float P;
float I;
float D;
}PID_Struct2;

//函数申明
float M3508_Angle_Init();		//M3508角度初始化
void M3508_angle_Ctrl(float target_angle,float init_angle);		//控制电机轴的角度，输入参数为目标角度
int32_t PID_MotorAngle_Ctrl(float target_angle,float motor_angle,PID_Struct2 _pid);		//PID角度控制

#endif
