/*@filename: RCS_MOTOR_3508.h
 *@author     陈志伟       
 *@brief:     3508电机的控制
 *@date: 2020-7-17
*/
#ifndef _RCS_MOTOR_H_
#define _RCS_MOTOR_H_

#include "rcs.h"		

#define Get_Motor_Data(void) 		CAN1_RX0_IRQHandler(void)			//CAN中断函数执行接受数据
#define Get_Motor_Data2(void)		CAN2_RX0_IRQHandler(void)	

void Motor_Init(void);		//电机初始化
void Motor_Init2(void);			
void Motor_Send2(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current);	//给3508发送数据
void Motor_Send(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current);

float 	Get_Motor_Float_Angle(uint8_t Motor_ID);//CAN1电机浮点机械角度,单位deg
int32_t Get_Motor_Integer_Angle(uint8_t Motor_ID);//整形,单位deg
int16_t Get_Motor_Speed(uint8_t Motor_ID);//CAN1电机转速,单位RPM
float 	Get_Motor_Float_Angle2(uint8_t Motor_ID);//CAN2浮点机械角度，单位deg
int32_t Get_Motor_Integer_Angle2(uint8_t Motor_ID);
int16_t Get_Motor_Speed2(uint8_t Motor_ID);	//CAN2电机转速,单位RPM

void Current_Data_Takepart(int16_t Current_Data, uint8_t *High_Data, uint8_t *Low_Data);
//数据拆解为高八位和低八位
__inline void Current_Data_Takepart(int16_t Current_Data, uint8_t *High_Data, uint8_t *Low_Data)
{
	*High_Data = (Current_Data & 0xff00) >> 8;
	*Low_Data = (Current_Data & 0x00ff);
}

#endif
