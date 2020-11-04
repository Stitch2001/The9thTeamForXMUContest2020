/*@filename: RCS_Ctrl_BaseMove.h
 *@author     陈志伟       
 *@brief:    综合控制底盘
 *@date: 2020-7-17
*/
#ifndef _RCS_CTRL_BASEMOVE_H_
#define _RCS_CTRL_BASEMOVE_H_

#include "rcs.h"		
//手动控制
#define SPEED_EQUIVALENT  	1.515151515151f			//速度当量1000/（RC_CH_VALUE_MAX(1684) -RC_CH_VALUE_OFFSET(1024)）（每档+1000mm/s）
#define ROTATE_EQUIVALENT		2.379994435e-3f			//角度当量PI/2/（RC_CH_VALUE_MAX(1684) -RC_CH_VALUE_OFFSET(1024)）（二分之派rad/s）
//自动控制
#define AUTO_SPEED					8000								//自动控制的移动最大速度 编码器转速r/min
#define SLOW_DOWN_DISTANCE  2000								//快到目标位置的减速距离mm

void Ctrl_BaseMove_Init(void);    //控制底盘初始化
void Remote_Ctrl(void);						//手动控制模式
float Speed_Cal(float now_distance,float max_speed);   //自动控制过程中速度计算
float Angle_Cal(float target_z,float begin_z,float now_distance,float target_distance);//自动控制过程中偏航角计算
void Point2Point_Move(float target_x,float target_y,float begin_x,float begin_y,float target_z,float begin_z);//点对点移动自动控制
#endif


