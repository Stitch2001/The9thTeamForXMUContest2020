#include "MOTOR_CTRL_LIB.h"

//@name: MOTOR_CTRL
//@brief:���ƻ�е�۵��
//@param:float init_angle ��ʼ���Ƕȱ���
//@param:float raw_angle �Ƕ�����
//@param:float motor_angle ��������ǰ�Ƕ�
float M3508_Angle_Init()                        //M3508�Ƕȳ�ʼ��
{
				float init_angle;
				Motor_Init2();			    //CAN2��ʼ��
        init_angle = Get_Motor_Float_Angle2(1);    //M3508 IDΪ1�����ʹ�õ���CAN1������ĳ�Angle1(��ͬ)
				return init_angle;
}

void M3508_angle_Ctrl(float target_angle,float init_angle)                         //���Ƶ����ĽǶȣ��������ΪĿ��Ƕ�
{
			float raw_angle, motor_angle;
			int32_t output;
			PID_Struct2 motor_pid;
	
      raw_angle = Get_Motor_Float_Angle2(1) - init_angle;	//��������ǰ�Ƕ�-��ʼ�Ƕ�=�Ƕ�����
      motor_angle = raw_angle / SLOWDOWN_RATE;            	 //ת��Ϊ�����Ƕ�
			motor_pid.P = 8.3;
			motor_pid.I = 0.25;
			motor_pid.D = 0.38;
      output = PID_MotorAngle_Ctrl(target_angle,motor_angle,motor_pid);      //PID�Ƕȿ���
      Motor_Send2(output,0,0,0);	//����CAN2 IDΪ1�ĵ���Ļ�е�Ƕ�
}

int32_t PID_MotorAngle_Ctrl(float target_angle,float motor_angle,PID_Struct2 _pid)
{
		static int32_t err = 0;                //��ǰ�Ƕ����
    static int32_t last_err = 0;           //�ϴνǶ����
    static int32_t totle_err = 0;          //�������
    static int32_t different_err;          //΢�����
    int32_t out_speed;                     //����ٶ�
    
    last_err = err;
    err =target_angle - motor_angle;
    
    totle_err += err;
    different_err = err-last_err;
    
    if(totle_err >= LIMIT_ERR)        //�����޷�
        totle_err = LIMIT_ERR;
    if(totle_err <= -LIMIT_ERR)
        totle_err =-LIMIT_ERR;
    
    out_speed = _pid.P * err + _pid.I * totle_err + _pid.D * different_err;
    
    if(out_speed >= LIMIT_ROTATE_SPEED)          //����ٶ��޷�
        out_speed = LIMIT_ROTATE_SPEED;
    if(out_speed <= -LIMIT_ROTATE_SPEED)
        out_speed = -LIMIT_ROTATE_SPEED;
    
    return out_speed;
}
