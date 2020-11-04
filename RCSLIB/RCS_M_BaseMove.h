/*@filename: RCS_M_BaseMove.h
 *@author     陈志伟       
 *@brief:     X型麦轮底盘速度解算
 *@date: 2020-7-17
*/
#ifndef _RCS_M_BASEMOVE_H_
#define _RCS_M_BASEMOVE_H_

#include "rcs.h"		
/***********底盘规格(mm)************/
#define BASE_LENGTH     300.0f  //底盘长
#define BASE_WIDTH			385.0f	//宽
#define RAD_WHEEL				76.2f	//轮子半径
#define SLOWDOWN_RATE	  19.0f	  //电机减速比
#define MM_S2R_MIN			0.1772276212907689f	// 轮毂线速度(mm/s)转化成轮子转速(r/min)，计算方法：60/(2*RAD_WHEEL*pi*cos(45°))

void M_BaseMove_Init(void);			//底盘初始化


void BaseMove(float move_speed,float move_direction,float rotate_speed,
												float target_angle,int feedback_flag,float target_pos_x,
												float target_pos_y);			//底盘总算法
												
#endif
