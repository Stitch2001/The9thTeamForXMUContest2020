/*@filename: RCS_M_BaseMove.c
 *@author     ��־ΰ       
 *@brief:     X�����ֵ����ٶȽ���
 *@date: 2020-7-17
*/
#include "RCS_M_BaseMove.h"

/*******************
X�ͳ��������ֵ��̣�Ĭ��CAN1ͨѶ
�����š��������ϵ�������תʱ���ӵ���Ч��������
        ��Y
        |
 1�J----+---�K2
   |    |   |
   |    +---+----��X
   |        |��
 4�I--------�L3
       ��

��������ϵ��������
       ��Y
       |
       |
       |
     Z +---------�� X      
********************/

/********ȫ�ֱ���********/

static float wheel_dir[4];		   //�����תʱ�����˶���������
static float wheel_distance;		 //���Ӻ͵������ĵþ���
static float wheel_w_dir[4];		 //������תʱ��ת���ٶ��ڸ������ϵķ���

PID_Struct speed_pid;	//�ٶȻ�PID
PID_Struct angle_pid;	//��̬��PID
PID_Struct coord_pid; //���귴��PID

/********��̬����********/
static void Wheel_Init(void);
static void M_PID_Init(void);

/************************/

//���ֵ��̳�ʼ��
void M_BaseMove_Init(void)
{
		Motor_Init();		//�����ʼ��
		M_PID_Init();		//PID��ʼ��
		Wheel_Init();		//���Ӳ�����ʼ��
}

//@name: BaseMove
//@brief: ���ֵ��̺����㷨
//@param:float move_speed               ƽ���ٶ�(mm/s)
//@param:float move_direction           ƽ���ķ���(degĿ�귽����y��н�,����ʱ��Ϊ����
//																								�Զ�����ʱ�Ǿ�������ϵ�µķ���
//																								���ֶ�����������Գ�������ϵ�ķ���)
//@param:float rotate_speed             ��תģʽ(rad/s��ʱ��Ϊ��,�ֶ�����ʱ����)
//@param:float target_angle 				    ƫ����ģʽ(deg��ʱ��Ϊ��,�Զ�����ʱ����)
//@param:int feedback_flag              ���귴�����Ʊ�־ 0 �ֶ�ģʽ,1 ��x��������,2 ��y��������,3 ��(x,y)��������
//@param:float target_pos_x             Ŀ��x����
//@param:float target_pos_y             Ŀ��y����
//feedback_flagΪ0ʱΪ�ֶ�����ģʽ������Ϊ�Զ�����ģʽ��
void BaseMove(float move_speed,	 float move_direction,float rotate_speed,
							float target_angle,int feedback_flag,		float target_pos_x,
							float target_pos_y)
{
    float target_speed[4] = {0,0,0,0};			//Ŀ��ת��
		float feedback_speed[2] = {0,0};				//X��Y���������ڵ��ٶ�
		float w_size =0;												//��ת���ٶȴ�С
		float z_angle =0,now_pos_x=0,now_pos_y=0;				//��ǰƫ���� ��λrad
		int16_t now_speed[4]={0,0,0,0}; 				//ÿ�������������ǰת��
		int32_t out[4]={0,0,0,0};
		int i=1;
		
		now_pos_x = RCS_Get_X();
		now_pos_y = RCS_Get_Y();
		
		if(feedback_flag)		//�ж��Զ�ģʽ�����ֶ�ģʽ
		{
			z_angle = RCS_Get_Z() * DEG2RAD;	//��ȡƫ����
			w_size = PID_Degree_Ctrl(target_angle,z_angle,angle_pid);
			//��ȡ���귴������X�����ٶȺ�Y�����ٶ�(���껷PID)
			for(i=1;(i<=4)&&(feedback_flag==1||feedback_flag==3);i++)
					feedback_speed[0] = -PID_FeedbackX(target_pos_x,now_pos_x,coord_pid);
			for(i=1;(i<=4)&&(feedback_flag >= 2);i++)
					feedback_speed[1] = -PID_FeedbackY(target_pos_y,now_pos_y,coord_pid);
		}
		else
		{
			w_size = rotate_speed * wheel_distance;
			z_angle = 0;	
		}
		
    for(i=0;i<4;i++)
		{
				target_speed[i] += move_speed * cos(move_direction * DEG2RAD - z_angle - wheel_dir[i]); //ƽ���ٶ������������ͶӰ
				target_speed[i] += w_size * cos(wheel_w_dir[i] - wheel_dir[i]);									//ת�����ٶ������������ͶӰ
				target_speed[i] += feedback_speed[0] * cos(-PI/2.0f- z_angle - wheel_dir[i]);		//��������ϵ��X�������ٶ������������ͶӰ
				target_speed[i] += feedback_speed[1] * cos(   0    - z_angle - wheel_dir[i]);		//Y�����ٶ������������ͶӰ
				target_speed[i] *= MM_S2R_MIN;				//��������������ٶ�ת��Ϊ����ת��
				target_speed[i] *= SLOWDOWN_RATE;		  //������ת��ת��Ϊ������ת���Ա���PID����
				now_speed[i] = Get_Motor_Speed(i+1);	//��ȡ�����������ת��
				out[i] = PID_Mwheel_Speed_Ctrl(target_speed[i],now_speed[i],i+1,speed_pid); //�ٶȻ�
		}
		Motor_Send(out[0], out[1], out[2], out[3]);
}

//PID��ʼ��
static void M_PID_Init(void)
{
		//�ٶȻ�PID��ʼ��
		speed_pid.P = 8.3;
		speed_pid.I = 0.25;
	  speed_pid.D = 0.38;
		
		//ƫ���Ƿ�������PID��ʼ��
		angle_pid.P = 70.0;
	  angle_pid.I = 0.03;
	  angle_pid.D = 4.0;
	
		//���귴������PID��ʼ��
		coord_pid.P = 2.0;
		coord_pid.I = 0.03;
		coord_pid.D = 0;
}

//���Ӳ�����ʼ��
static void Wheel_Init(void)
{
	  //������������򣨼����Ϸ�ͼ����
		wheel_dir[0] = -PI/4.0f;			
		wheel_dir[1] = 3 * wheel_dir[0];
		wheel_dir[2] = -wheel_dir[1];
		wheel_dir[3] = -wheel_dir[0];
		//������תʱ��ת���ٶ��ڸ������ϵķ���
		wheel_w_dir[0] = atan2( BASE_WIDTH, BASE_LENGTH) + PI/2.0f;
		wheel_w_dir[1] = atan2(-BASE_WIDTH, BASE_LENGTH) + PI/2.0f;
		wheel_w_dir[2] = atan2(-BASE_WIDTH,-BASE_LENGTH) + PI/2.0f;
		wheel_w_dir[3] = atan2( BASE_WIDTH,-BASE_LENGTH) + PI/2.0f;
		//����������������ľ���
		wheel_distance = sqrt(BASE_LENGTH * BASE_LENGTH + BASE_WIDTH * BASE_WIDTH) / 2.0f;
}
