/*@filename: RCS_GROBAL_GPS.c
 *@author     ��־ΰ       
 *@brief:     ��ȡȫ����λ��Ϣ
 *@date: 2020-7-17
*/

#include "RCS_GROBAL_GPS.h"

static float rcs_pos_x = 0;
static float rcs_pos_y = 0;
static float rcs_zangle = 0;
static float rcs_xangle = 0;
static float rcs_yangle = 0;
static float rcs_w_z = 0;

//RCS��λ��ʼ��
void RCS_GPS_Init(void)
{
    RCS_USART_Config(RCS_GPS_USART, RCS_GPS_GPIO,RCS_GPS_TX,
                      RCS_GPS_RX, RCS_GPS_BAUD, RCS_GPS_PRI);
}

//��ȡ��λ����
void Get_GPS_Msg(float *GPS_values)
{
    GPS_values[0] = rcs_pos_x;
    GPS_values[1] = rcs_pos_y;
    GPS_values[2] = rcs_zangle;
    GPS_values[3] = rcs_xangle;
    GPS_values[4] = rcs_yangle;
    GPS_values[5] = rcs_w_z;
}
//�жϽ��ܺ���(ʹ��ʱӳ��ɶ�ӦUSART���жϺ���)
void RCS_GPS_Interrupt(void)
{
    static int flag = 0;
    uint8_t receive_char;
    static uint8_t receive_str_data[13];
    static int j = 0;
    
    if (USART_GetITStatus(RCS_GPS_USART, USART_IT_RXNE) == SET)
    {
      receive_char = RCS_USART_Accept_Char(RCS_GPS_USART);
      if(flag == 1)
      {
          if(receive_char == 0xEC)//������־
          {
            if(j == 12)
            {
                  rcs_pos_x = (int16_t)(receive_str_data[0]<<8)|(int16_t)(receive_str_data[1]); 
                  rcs_pos_y = (int16_t)(receive_str_data[2]<<8)|(int16_t)(receive_str_data[3]);  
									rcs_w_z   = (int16_t)(receive_str_data[4]<<8)|(int16_t)(receive_str_data[5]); 
									rcs_zangle = (int16_t)(receive_str_data[6]<<8)|(int16_t)(receive_str_data[7]); 
									rcs_xangle = (int16_t)(receive_str_data[8]<<8)|(int16_t)(receive_str_data[9]); 
									rcs_yangle = (int16_t)(receive_str_data[10]<<8)|(int16_t)(receive_str_data[11]); 
            }
            flag = 0;
            j = 0;
          }
          else
          {
            receive_str_data[j] = receive_char;
						j++;
          }
      }
      if(receive_char == 0xCE)            //��ʼ��־
        flag = 1;  
    }
    //�����־λ
    USART_ClearITPendingBit(RCS_GPS_USART, USART_IT_RXNE);
}

//��ȡX����
float RCS_Get_X()
{
  return rcs_pos_x;
}
//��ȡY����
float RCS_Get_Y()
{
  return rcs_pos_y;
}
//��ȡƫ����
float RCS_Get_Z()
{
  return rcs_zangle;
}
