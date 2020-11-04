/*@filename: RCS_MOTOR_3508.c
 *@author     ��־ΰ       
 *@brief:     3508����Ŀ���
 *@date: 2020-7-17
*/
#include "RCS_MOTOR_3508.h"

#define ENCODER2ANGLE	0.0439453125		//360/8192

/****************ȫ�ֱ���****************/
int32_t max_position = 0;
static float g_encoder_start_angle[4];
static uint16_t g_encoder_oringin_angle[4];
static uint16_t g_encoder_last_angle[4];
static int16_t g_encoder_cycles[4];
volatile float g_encoder_float_angle[4];
volatile int32_t g_encoder_int_angle[4];
static int16_t g_encoder_oringin_speed[4];
volatile int16_t g_encoder_speed[4];

static float g_encoder_start_angle2[4];
static uint16_t g_encoder_oringin_angle2[4];
static uint16_t g_encoder_last_angle2[4];
static int16_t g_encoder_cycles2[4];
volatile float g_encoder_float_angle2[4];
volatile int32_t g_encoder_int_angle2[4];
static int16_t g_encoder_oringin_speed2[4];
volatile int16_t g_encoder_speed2[4];


/****************��̬����****************/

/******************����******************/
//CAN1��ʼ��
void Motor_Init(void)
{
	int i;

	for (i = 0; i < 4; i++)
	{
              
		g_encoder_start_angle[i] = 0;
		g_encoder_oringin_angle[i] = 0;
		g_encoder_cycles[i] = 0;
		g_encoder_float_angle[i] = 0;
		g_encoder_int_angle[i] = 0;
		g_encoder_oringin_speed[i] = 0;
		g_encoder_speed[i] = 0;
	}
	RCS_CAN1_Config(CAN1_GPIO, CAN1_TXD, CAN1_RXD,CAN1_BAUD);
	delay_ms(10);
	for ( i = 0; i < 4; i++)
		g_encoder_start_angle[i] = g_encoder_float_angle[i];
}

//CAN2��ʼ��
void Motor_Init2(void)
{
	int i;

	for (i = 0; i < 4; i++)
	{
              
		g_encoder_start_angle2[i] = 0;
		g_encoder_oringin_angle2[i] = 0;
		g_encoder_cycles2[i] = 0;
		g_encoder_float_angle2[i] = 0;
		g_encoder_int_angle2[i] = 0;
		g_encoder_oringin_speed2[i] = 0;
		g_encoder_speed2[i] = 0;
	}
	RCS_CAN2_Config(CAN2_GPIO, CAN2_TXD, CAN2_RXD ,CAN2_BAUD);
	delay_ms(10);
	for (i = 0; i < 4; i++)
		g_encoder_start_angle2[i] = g_encoder_float_angle2[i];
}

//@name: Motor_Send
//@brief: ��3508�ĵ�����Ϳ�������
//@param:ID1_Current ���IDΪ1�ĵ�������
//@param:ID2_Current ���IDΪ2�ĵ�������
//@param:ID3_Current ���IDΪ3�ĵ�������
//@param:ID4_Current ���IDΪ4�ĵ�������
void Motor_Send(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current)
{
	uint8_t current_data[8];

	Current_Data_Takepart(ID1_Current, &current_data[0], &current_data[1]);
	Current_Data_Takepart(ID2_Current, &current_data[2], &current_data[3]);
	Current_Data_Takepart(ID3_Current, &current_data[4], &current_data[5]);
	Current_Data_Takepart(ID4_Current, &current_data[6], &current_data[7]);        
	RCS_CANx_Send(CAN1,0x200, 0x08, current_data);
}

void Motor_Send2(int16_t ID1_Current, int16_t ID2_Current, int16_t ID3_Current, int16_t ID4_Current)
{
	uint8_t current_data[8];

	Current_Data_Takepart(ID1_Current, &current_data[0], &current_data[1]);
	Current_Data_Takepart(ID2_Current, &current_data[2], &current_data[3]);
	Current_Data_Takepart(ID3_Current, &current_data[4], &current_data[5]);
	Current_Data_Takepart(ID4_Current, &current_data[6], &current_data[7]);
	RCS_CANx_Send(CAN2,0x200, 0x08, current_data);
}

//CAN1�����ж�
void Get_Motor_Data(void)
{
	CanRxMsg rx_message;

	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);

		if ((rx_message.IDE == CAN_Id_Standard) && (rx_message.RTR == CAN_RTR_Data) && (rx_message.DLC == 0x08)) //��׼֡������֡����Ϊ8
		{
			//����Ž���
			uint8_t motor_number = (rx_message.StdId & 0x000f);

			//�ٶ����ݽ���
			g_encoder_oringin_speed[motor_number - 1] = (rx_message.Data[2] << 8) | (rx_message.Data[3]);
			if (g_encoder_oringin_speed[motor_number - 1] <= 3 && g_encoder_oringin_speed[motor_number - 1] >= -3)
				g_encoder_speed[motor_number - 1] = 0;
			else
				g_encoder_speed[motor_number - 1] = g_encoder_oringin_speed[motor_number - 1];

			//λ�����ݽ���
			g_encoder_oringin_angle[motor_number - 1] = (rx_message.Data[0] << 8) | (rx_message.Data[1]);
			//��0�����ж�
			if (abs(g_encoder_oringin_angle[motor_number - 1] - g_encoder_last_angle[motor_number - 1]) > 6000)
			{
				if (g_encoder_last_angle[motor_number - 1] > 4000)g_encoder_cycles[motor_number - 1]++;
				else g_encoder_cycles[motor_number - 1]--;
			}
			g_encoder_last_angle[motor_number - 1] = g_encoder_oringin_angle[motor_number - 1];
			g_encoder_float_angle[motor_number - 1] = 360.0 * g_encoder_cycles[motor_number - 1] + g_encoder_oringin_angle[motor_number - 1] * ENCODER2ANGLE - g_encoder_start_angle[motor_number - 1];
			g_encoder_int_angle[motor_number - 1] = (int32_t)g_encoder_float_angle[motor_number - 1];
		}
		CAN_FIFORelease(CAN1, CAN_FIFO0);
	}
}

//CAN2�����ж�
void Get_Motor_Data2(void)
{
	CanRxMsg rx_message;

	if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN2, CAN_FIFO0, &rx_message);

		if ((rx_message.IDE == CAN_Id_Standard) && (rx_message.RTR == CAN_RTR_Data) && (rx_message.DLC == 0x08)) //��׼֡������֡����Ϊ8
		{
			//����Ž���
			uint8_t motor_number = (rx_message.StdId & 0x000f);

			//�ٶ����ݽ���
			g_encoder_oringin_speed2[motor_number - 1] = (rx_message.Data[2] << 8) | (rx_message.Data[3]);
			if (g_encoder_oringin_speed2[motor_number - 1] <= 3 && g_encoder_oringin_speed2[motor_number - 1] >= -3)
				g_encoder_speed2[motor_number - 1] = 0;
			else
				g_encoder_speed2[motor_number - 1] = g_encoder_oringin_speed2[motor_number - 1];

			//λ�����ݽ���
			g_encoder_oringin_angle2[motor_number - 1] = (rx_message.Data[0] << 8) | (rx_message.Data[1]);
			//��0�����ж�
			if (abs(g_encoder_oringin_angle2[motor_number - 1] - g_encoder_last_angle2[motor_number - 1]) > 6000)
			{
				if (g_encoder_last_angle2[motor_number - 1] > 4000)g_encoder_cycles2[motor_number - 1]++;
				else g_encoder_cycles2[motor_number - 1]--;
			}
			g_encoder_last_angle2[motor_number - 1] = g_encoder_oringin_angle2[motor_number - 1];
			g_encoder_float_angle2[motor_number - 1] = 360.0 * g_encoder_cycles2[motor_number - 1] + g_encoder_oringin_angle2[motor_number - 1] * ENCODER2ANGLE - g_encoder_start_angle2[motor_number - 1];
			g_encoder_int_angle2[motor_number - 1] = (int32_t)g_encoder_float_angle2[motor_number - 1];
		}
		CAN_FIFORelease(CAN2, CAN_FIFO0);
	}
}

//��ȡCAN1��������е�Ƕ�,��λdeg
float Get_Motor_Float_Angle(uint8_t Motor_ID)
{
	return g_encoder_float_angle[Motor_ID - 1];
}

//��ȡ���ε����е�Ƕ�,��λdeg
int32_t Get_Motor_Integer_Angle(uint8_t Motor_ID)
{
	return g_encoder_int_angle[Motor_ID - 1];
}

//��ȡ���ת��,��λRPM
int16_t Get_Motor_Speed(uint8_t Motor_ID)
{
	return g_encoder_speed[Motor_ID - 1];
}

//��ȡCAN2����Ƕ�,��λdeg
float Get_Motor_Float_Angle2(uint8_t Motor_ID)
{
	return g_encoder_float_angle2[Motor_ID - 1];
}

//����,��λdeg
int32_t Get_Motor_Integer_Angle2(uint8_t Motor_ID)
{
	return g_encoder_int_angle2[Motor_ID - 1];
}

//CAN2���ת��,��λRPM
int16_t Get_Motor_Speed2(uint8_t Motor_ID)
{
	return g_encoder_speed2[Motor_ID - 1];
}
