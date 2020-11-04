/*@filename: RCS_MOTOR_3508.h
 *@author     ��־ΰ       
 *@brief:     3508����Ŀ���
 *@date: 2020-7-17
*/
#ifndef _RCS_MOTOR_H_
#define _RCS_MOTOR_H_

#include "rcs.h"		

#define Get_Motor_Data(void) 		CAN1_RX0_IRQHandler(void)			//CAN�жϺ���ִ�н�������
#define Get_Motor_Data2(void)		CAN2_RX0_IRQHandler(void)	

void Motor_Init(void);		//�����ʼ��
void Motor_Init2(void);			
void Motor_Send2(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current);	//��3508��������
void Motor_Send(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current);

float 	Get_Motor_Float_Angle(uint8_t Motor_ID);//CAN1��������е�Ƕ�,��λdeg
int32_t Get_Motor_Integer_Angle(uint8_t Motor_ID);//����,��λdeg
int16_t Get_Motor_Speed(uint8_t Motor_ID);//CAN1���ת��,��λRPM
float 	Get_Motor_Float_Angle2(uint8_t Motor_ID);//CAN2�����е�Ƕȣ���λdeg
int32_t Get_Motor_Integer_Angle2(uint8_t Motor_ID);
int16_t Get_Motor_Speed2(uint8_t Motor_ID);	//CAN2���ת��,��λRPM

void Current_Data_Takepart(int16_t Current_Data, uint8_t *High_Data, uint8_t *Low_Data);
//���ݲ��Ϊ�߰�λ�͵Ͱ�λ
__inline void Current_Data_Takepart(int16_t Current_Data, uint8_t *High_Data, uint8_t *Low_Data)
{
	*High_Data = (Current_Data & 0xff00) >> 8;
	*Low_Data = (Current_Data & 0x00ff);
}

#endif
