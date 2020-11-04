/*@filename: RCS_PIDctrl.c
 *@author     陈志伟       
 *@brief:     存放PID算法
 *@date: 2020-7-17
*/

#include "RCS_PIDctrl.h"

//@name: PID_Degree_Ctrl
//@brief: PID偏航角反馈控制 返回速度值mm/s
//@param:float target_degree 目标偏航角
//@param:float current_degree 当前偏航角
int32_t PID_Degree_Ctrl(float target_degree,float current_degree,PID_Struct _pid)  
{   
    static int32_t err = 0;                //当前角度误差
    static int32_t last_err = 0;           //上次角度误差
    static int32_t totle_err = 0;          //积分误差
    static int32_t different_err;          //微分误差
    int32_t out_speed;                     //输出速度
    
    last_err = err;
    err =target_degree - current_degree;
    
    totle_err += err;
    different_err = err-last_err;
    
    if(totle_err >= LIMIT_ERR)        //积分限幅
        totle_err = LIMIT_ERR;
    if(totle_err <= -LIMIT_ERR)
        totle_err =-LIMIT_ERR;
    
    out_speed = _pid.P * err + _pid.I * totle_err + _pid.D * different_err;
    
    if(out_speed >= LIMIT_ROTATE_SPEED)          //输出速度限幅
        out_speed = LIMIT_ROTATE_SPEED;
    if(out_speed <= -LIMIT_ROTATE_SPEED)
        out_speed = -LIMIT_ROTATE_SPEED;
    
    return out_speed;
}

//@name: PID_FeedbackX
//@brief: 麦轮底盘X方向反馈调节 返回速度值mm/s
//@param:int target_distance 目标位置
//@param:int current_distance 当前位置
//@param:PID_Struct _pid   坐标反馈调节的PID组
int32_t PID_FeedbackX(float target_distance,float current_distance,PID_Struct _pid)
{
    static int32_t err = 0;                //当前位置误差
    static int32_t last_err = 0;           //上次位置误差
    static int32_t totle_err = 0;          //积分误差
    static int32_t different_err;          //微分误差
    int32_t out_speed;                     //输出速度
    
    last_err = err;
    err = target_distance - current_distance;
    
    totle_err += err;
    different_err = err-last_err;
    
    if(totle_err >= LIMIT_ERR)                  //积分限幅
        totle_err = LIMIT_ERR;
    if(totle_err <= -LIMIT_ERR)
        totle_err = -LIMIT_ERR;
    
    out_speed = _pid.P * err + _pid.I * totle_err + _pid.D * different_err;
    
    if(out_speed >= LIMIT_FEEDBACK_SPEED)                //输出限幅
        out_speed = LIMIT_FEEDBACK_SPEED;
    if(out_speed <= -LIMIT_FEEDBACK_SPEED)
        out_speed = -LIMIT_FEEDBACK_SPEED;
 
    return out_speed;
}

//@name: PID_FeedbackY
//@brief: 麦轮底盘Y方向反馈调节 返回速度值mm/s
//@param:int target_distance 目标位置
//@param:int current_distance 当前位置
//@param:PID_Struct _pid   坐标反馈调节的PID组
int32_t PID_FeedbackY(float target_distance,float current_distance,PID_Struct _pid)
{
    static int32_t err = 0;                //当前位置误差
    static int32_t last_err = 0;           //上次位置误差
    static int32_t totle_err = 0;          //积分误差
    static int32_t different_err;          //微分误差
    int32_t out_speed;                     //输出速度
    
    last_err = err;
    err = target_distance - current_distance;
    
    totle_err += err;
    different_err = err-last_err;
    
    if(totle_err >= LIMIT_ERR)                  //积分限幅
        totle_err = LIMIT_ERR;
    if(totle_err <= -LIMIT_ERR)
        totle_err = -LIMIT_ERR;
    
    out_speed = _pid.P * err + _pid.I * totle_err + _pid.D * different_err;
    
    if(out_speed >= LIMIT_FEEDBACK_SPEED)                //输出限幅
        out_speed = LIMIT_FEEDBACK_SPEED;
    if(out_speed <= -LIMIT_FEEDBACK_SPEED)
        out_speed = -LIMIT_FEEDBACK_SPEED;
 
    return out_speed;
}

//@name: PID_Mwheel_Speed_Ctrl
//@brief: 麦轮PID速度环控制 返回电流值mA
//@param:int target_speed 目标转速
//@param:int current_speed 当前转速
//@param:int num   电机ID
//@param:PID_Struct _pid   每个轮子对应的PID组
int32_t PID_Mwheel_Speed_Ctrl(float target_speed,float current_speed,int num,PID_Struct _pid)
{
    static int32_t err[4] = {0,0,0,0};                //当前速度误差
    static int32_t last_err[4] = {0,0,0,0};           //上次速度误差
    static int32_t totle_err[4] = {0,0,0,0};          //积分误差
    int32_t different_err;               //微分误差
    int32_t out_current;                 //输出电流
    
    last_err[num-1] = err[num-1];
    err[num-1] =target_speed - current_speed;
    
    totle_err[num-1] += err[num-1];
    different_err = err[num-1]-last_err[num-1];
    
    if(totle_err[num-1] >= LIMIT_ERR)                  //积分误差限幅
        totle_err[num-1] = LIMIT_ERR;
    if(totle_err[num-1] <=-LIMIT_ERR)
        totle_err[num-1] = LIMIT_ERR;
    
    out_current = _pid.P * err[num-1] + _pid.I * totle_err[num-1] + _pid.D * different_err;
    
    if(out_current >= LIMIT_SPEED)                //输出电流限幅
        out_current = LIMIT_SPEED;
    if(out_current <=-LIMIT_SPEED)
        out_current =-LIMIT_SPEED;

    return out_current;
}
