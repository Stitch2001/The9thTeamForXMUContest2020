#include "MOTOR_CTRL_LIB.h"

//@name: MOTOR_CTRL
//@brief:控制机械臂电机
//@param:float init_angle 初始化角度变量
//@param:float raw_angle 角度增量
//@param:float motor_angle 编码器当前角度
float M3508_Angle_Init()                        //M3508角度初始化
{
				float init_angle;
				Motor_Init2();			    //CAN2初始化
        init_angle = Get_Motor_Float_Angle2(1);    //M3508 ID为1，如果使用的是CAN1，里面改成Angle1(下同)
				return init_angle;
}

void M3508_angle_Ctrl(float target_angle,float init_angle)                         //控制电机轴的角度，输入参数为目标角度
{
			float raw_angle, motor_angle;
			int32_t output;
			PID_Struct2 motor_pid;
	
      raw_angle = Get_Motor_Float_Angle2(1) - init_angle;	//编码器当前角度-初始角度=角度增量
      motor_angle = raw_angle / SLOWDOWN_RATE;            	 //转化为电机轴角度
			motor_pid.P = 8.3;
			motor_pid.I = 0.25;
			motor_pid.D = 0.38;
      output = PID_MotorAngle_Ctrl(target_angle,motor_angle,motor_pid);      //PID角度控制
      Motor_Send2(output,0,0,0);	//控制CAN2 ID为1的电机的机械角度
}

int32_t PID_MotorAngle_Ctrl(float target_angle,float motor_angle,PID_Struct2 _pid)
{
		static int32_t err = 0;                //当前角度误差
    static int32_t last_err = 0;           //上次角度误差
    static int32_t totle_err = 0;          //积分误差
    static int32_t different_err;          //微分误差
    int32_t out_speed;                     //输出速度
    
    last_err = err;
    err =target_angle - motor_angle;
    
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
