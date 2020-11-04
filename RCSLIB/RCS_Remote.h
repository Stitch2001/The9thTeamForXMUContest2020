/*@filename: RCS_Remoute.h
 *@author     陈志伟       
 *@brief:     RM手柄接受端配置
 *@date: 2020-7-17
*/
#ifndef _RCS_REMOTE_H_
#define _RCS_REMOTE_H_

#include "rcs.h"	
/* ----------------------- 遥控示意图---------------------------- 
					!S1															      					!S2
								 ↑ch3										    	↑ch1
								 |		ch2								  			|		ch0
						 ←--+--→										  ←--+--→
								 |															|
                 ↓   													↓

-----------------------------------------------------------------*/

/* ----------------------- 遥控模拟摇杆值---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )				
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
/* ----------------------- 遥控拨动开关值----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- 电脑键盘鼠标值-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)
#define RC_FRAME_LENGTH 18u
/* ----------------------- 创建接受包结构体 ------------------------------------- */
typedef struct
 {
	uint16_t ch0;			//right Joystick left-right
	uint16_t ch1;			//right Joystick ahead-back
	uint16_t ch2;			//left Joystick left-right
	uint16_t ch3;			//left Joystick ahead-back
	uint8_t s1;				//left switch
	uint8_t s2;				//right switch
 }rc_ctrl;							//遥控
typedef struct
 {
	int16_t x;				//mouse left-right
	int16_t y;				//mouse ahead-back
	int16_t z;				//
	uint8_t press_l;	//mouse left button
	uint8_t press_r;	//mouse right button
 }mouse_ctrl;					//鼠标
typedef struct
 {
	uint16_t v;				//keyboard value
 }key_ctrl;						//键盘
 
 void Get_RC_Value(rc_ctrl *rc_value);				//获取遥控值
 void Get_Key_Value(key_ctrl *key_value);			//获取键盘值
 void Get_Mouse_Value(mouse_ctrl *mouse_value);//获取鼠标值
 void RC_Init(void);													//初始化遥控接受器
 
#endif
