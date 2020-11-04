/*@filename: RCS_Ctrl_BaseMove.c
 *@author     ��־ΰ       
 *@brief:    �ۺϿ��Ƶ���
 *@date: 2020-7-17
*/

#include "RCS_Ctrl_BaseMove.h"

rc_ctrl rc_value;

//���̿��Ƴ�ʼ��
void Ctrl_BaseMove_Init(void)
{
	M_BaseMove_Init();		//���ֵ��̳�ʼ��
	RC_Init();						//ң���ֱ���ʼ��
}

//ң���ֶ�����
void Remote_Ctrl(void)
{
		float speed[3];				//0��1Ϊ��Գ��� X Y�����ƽ���ٶȣ�2Ϊ��ƽ���ٶ�
		float speed_dir;			//ƽ������
		float rotate_w;				//��ת���ٶ�
	  int gear = 1;							//�ٶȵ�λ
	
		Get_RC_Value(&rc_value);												//��ȡ�ֱ�ֵ
		//gear = rc_value.s2 * SPEED_EQUIVALENT;				//��ȡ��λ
		speed[0] = (rc_value.ch2 - RC_CH_VALUE_OFFSET) * gear;	//��Ϊ��ch2����ˮƽ����
		speed[1] = (rc_value.ch3 - RC_CH_VALUE_OFFSET) * gear;	//��Ϊ��ch3���ƴ�ֱ����
		speed[2] = sqrt(speed[0]*speed[0]+speed[1]*speed[1]);		
		speed_dir = atan2(-speed[0],speed[1]) * RAD2DEG;
	
		rotate_w =-(rc_value.ch0 - RC_CH_VALUE_OFFSET) * ROTATE_EQUIVALENT * gear; //��ʱ��Ϊ��ch0������ת����
		if(rc_value.s1 == RC_SW_MID)			//�󿪹�S1���м�ʱΪ�ֶ�ģʽ
			BaseMove(speed[2],speed_dir,rotate_w,0,0,0,0);   //�ֶ����Ƶ���
		else if (rc_value.s1 == RC_SW_UP)
			Motor_Send2(rc_value.ch1,0,0,0);		//�󿪹�S1����ʱΪ��е�ۿ���ģʽ��ͳһͨ���Ҹ˿��ƻ�е�ۣ���е�۵ĵ������Ϊ1
		else if (rc_value.s1 == RC_SW_DOWN);		//�󿪹�S1����ʱΪ���׿���ģʽ����˿���ץ�����ף��Ҹ˿�����������
}

//@name: Point2Point_Move
//@brief: ��Ե��ƶ��Զ�����
//@param: float target_x		Ŀ��X����
//@param: float target_y		Ŀ��Y����
//@param: float begin_x			��ʼX����
//@param: float begin_y			��ʼY����
//@param: float target_z		Ŀ��ƫ����
//@param: float begin_z			��ʼƫ����
void Point2Point_Move(float target_x,float target_y,float begin_x,float begin_y,float target_z,float begin_z)
{
		float speed = 0;
		float speed_dir = 0;
		float angle = 0;
		float x=0,y=0;
		float now_distance =0;
		float target_distance =0;
	
		x = RCS_Get_X();						
		y = RCS_Get_Y();
		
		now_distance = sqrt((x - target_x)*(x - target_x)+(y - target_y)*(y - target_y));
		target_distance = sqrt((begin_x - target_x)*(begin_x - target_x)+(begin_y - target_y)*(begin_y - target_y));
	
		speed = Speed_Cal(now_distance,AUTO_SPEED);
		speed_dir = atan2(-target_x,target_y) * RAD2DEG;
		angle = Angle_Cal(target_z,begin_z,now_distance,target_distance);
	
		BaseMove(speed,speed_dir,0,angle,3,target_x,target_y);
}

//@name: Speed_Cal
//@brief: �Զ������ٶȼ���
//@param: float now_distance	��ǰλ����Ŀ���ľ��Ծ���
//@param: float max_speed			���������е�����ٶ�
float Speed_Cal(float now_distance,float max_speed)
{
		float current_speed = 0;    
		if(now_distance <= SLOW_DOWN_DISTANCE)
			current_speed = max_speed * (1 - pow((SLOW_DOWN_DISTANCE - now_distance)/SLOW_DOWN_DISTANCE,3));
		else
			current_speed = max_speed;
    return current_speed;
}

//@name: Angle_Cal
//@brief: �Զ�����ƫ���Ǽ���
//@param: float target_z				Ŀ��ƫ����
//@param: float begin_z					��ʼƫ����
//@param: float now_distance	  ��ǰλ����Ŀ���ľ��Ծ���
//@param: float target_distance	��ʼ����Ŀ���ľ��Ծ���
float Angle_Cal(float target_z,float begin_z,float now_distance,float target_distance)
{
	return ((target_z - begin_z)*(1.0f - now_distance/target_distance) + begin_z);
}
