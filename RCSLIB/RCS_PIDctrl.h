/*@filename: RCS_PIDctrl.h
 *@author     陈志伟       
 *@brief:     存放PID算法
 *@date: 2020-7-17
*/
#ifndef _RCS_PIDCTRL_H_
#define _RCS_PIDCTRL_H_

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
}PID_Struct;

int32_t PID_Mwheel_Speed_Ctrl(float target_speed,float current_speed,int num,PID_Struct _pid);//麦轮电机转速PID控制
int32_t PID_FeedbackY(float target_distance,float current_distance,PID_Struct _pid);	//Y坐标反馈调节
int32_t PID_FeedbackX(float target_distance,float current_distance,PID_Struct _pid);	//X坐标反馈调节
int32_t PID_Degree_Ctrl(float target_degree,float current_degree,PID_Struct _pid);		//偏航角反馈调节

#endif
