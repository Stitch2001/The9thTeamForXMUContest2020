/*@filename: RCS_PIDctrl.c
 *@author     ��־ΰ       
 *@brief:     ���PID�㷨
 *@date: 2020-7-17
*/

#include "RCS_PIDctrl.h"

//@name: PID_Degree_Ctrl
//@brief: PIDƫ���Ƿ������� �����ٶ�ֵmm/s
//@param:float target_degree Ŀ��ƫ����
//@param:float current_degree ��ǰƫ����
int32_t PID_Degree_Ctrl(float target_degree,float current_degree,PID_Struct _pid)  
{   
    static int32_t err = 0;                //��ǰ�Ƕ����
    static int32_t last_err = 0;           //�ϴνǶ����
    static int32_t totle_err = 0;          //�������
    static int32_t different_err;          //΢�����
    int32_t out_speed;                     //����ٶ�
    
    last_err = err;
    err =target_degree - current_degree;
    
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

//@name: PID_FeedbackX
//@brief: ���ֵ���X���������� �����ٶ�ֵmm/s
//@param:int target_distance Ŀ��λ��
//@param:int current_distance ��ǰλ��
//@param:PID_Struct _pid   ���귴�����ڵ�PID��
int32_t PID_FeedbackX(float target_distance,float current_distance,PID_Struct _pid)
{
    static int32_t err = 0;                //��ǰλ�����
    static int32_t last_err = 0;           //�ϴ�λ�����
    static int32_t totle_err = 0;          //�������
    static int32_t different_err;          //΢�����
    int32_t out_speed;                     //����ٶ�
    
    last_err = err;
    err = target_distance - current_distance;
    
    totle_err += err;
    different_err = err-last_err;
    
    if(totle_err >= LIMIT_ERR)                  //�����޷�
        totle_err = LIMIT_ERR;
    if(totle_err <= -LIMIT_ERR)
        totle_err = -LIMIT_ERR;
    
    out_speed = _pid.P * err + _pid.I * totle_err + _pid.D * different_err;
    
    if(out_speed >= LIMIT_FEEDBACK_SPEED)                //����޷�
        out_speed = LIMIT_FEEDBACK_SPEED;
    if(out_speed <= -LIMIT_FEEDBACK_SPEED)
        out_speed = -LIMIT_FEEDBACK_SPEED;
 
    return out_speed;
}

//@name: PID_FeedbackY
//@brief: ���ֵ���Y���������� �����ٶ�ֵmm/s
//@param:int target_distance Ŀ��λ��
//@param:int current_distance ��ǰλ��
//@param:PID_Struct _pid   ���귴�����ڵ�PID��
int32_t PID_FeedbackY(float target_distance,float current_distance,PID_Struct _pid)
{
    static int32_t err = 0;                //��ǰλ�����
    static int32_t last_err = 0;           //�ϴ�λ�����
    static int32_t totle_err = 0;          //�������
    static int32_t different_err;          //΢�����
    int32_t out_speed;                     //����ٶ�
    
    last_err = err;
    err = target_distance - current_distance;
    
    totle_err += err;
    different_err = err-last_err;
    
    if(totle_err >= LIMIT_ERR)                  //�����޷�
        totle_err = LIMIT_ERR;
    if(totle_err <= -LIMIT_ERR)
        totle_err = -LIMIT_ERR;
    
    out_speed = _pid.P * err + _pid.I * totle_err + _pid.D * different_err;
    
    if(out_speed >= LIMIT_FEEDBACK_SPEED)                //����޷�
        out_speed = LIMIT_FEEDBACK_SPEED;
    if(out_speed <= -LIMIT_FEEDBACK_SPEED)
        out_speed = -LIMIT_FEEDBACK_SPEED;
 
    return out_speed;
}

//@name: PID_Mwheel_Speed_Ctrl
//@brief: ����PID�ٶȻ����� ���ص���ֵmA
//@param:int target_speed Ŀ��ת��
//@param:int current_speed ��ǰת��
//@param:int num   ���ID
//@param:PID_Struct _pid   ÿ�����Ӷ�Ӧ��PID��
int32_t PID_Mwheel_Speed_Ctrl(float target_speed,float current_speed,int num,PID_Struct _pid)
{
    static int32_t err[4] = {0,0,0,0};                //��ǰ�ٶ����
    static int32_t last_err[4] = {0,0,0,0};           //�ϴ��ٶ����
    static int32_t totle_err[4] = {0,0,0,0};          //�������
    int32_t different_err;               //΢�����
    int32_t out_current;                 //�������
    
    last_err[num-1] = err[num-1];
    err[num-1] =target_speed - current_speed;
    
    totle_err[num-1] += err[num-1];
    different_err = err[num-1]-last_err[num-1];
    
    if(totle_err[num-1] >= LIMIT_ERR)                  //��������޷�
        totle_err[num-1] = LIMIT_ERR;
    if(totle_err[num-1] <=-LIMIT_ERR)
        totle_err[num-1] = LIMIT_ERR;
    
    out_current = _pid.P * err[num-1] + _pid.I * totle_err[num-1] + _pid.D * different_err;
    
    if(out_current >= LIMIT_SPEED)                //��������޷�
        out_current = LIMIT_SPEED;
    if(out_current <=-LIMIT_SPEED)
        out_current =-LIMIT_SPEED;

    return out_current;
}
