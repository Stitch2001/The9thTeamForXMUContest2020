/*@filename: RCS_Ctrl_BaseMove.c
 *@author     陈志伟       
 *@brief:    综合控制底盘
 *@date: 2020-7-17
*/

#include "RCS_Ctrl_BaseMove.h"

rc_ctrl rc_value;

//底盘控制初始化
void Ctrl_BaseMove_Init(void)
{
	M_BaseMove_Init();		//麦轮底盘初始化
	RC_Init();						//遥控手柄初始化
}

//遥控手动控制
void Remote_Ctrl(void)
{
		float speed[3];				//0、1为相对车身 X Y方向的平移速度，2为总平移速度
		float speed_dir;			//平动方向
		float rotate_w;				//自转角速度
	  int gear = 1;							//速度档位
	
		Get_RC_Value(&rc_value);												//获取手柄值
		//gear = rc_value.s2 * SPEED_EQUIVALENT;				//获取档位
		speed[0] = (rc_value.ch2 - RC_CH_VALUE_OFFSET) * gear;	//右为正ch2控制水平方向
		speed[1] = (rc_value.ch3 - RC_CH_VALUE_OFFSET) * gear;	//上为正ch3控制垂直方向
		speed[2] = sqrt(speed[0]*speed[0]+speed[1]*speed[1]);		
		speed_dir = atan2(-speed[0],speed[1]) * RAD2DEG;
	
		rotate_w =-(rc_value.ch0 - RC_CH_VALUE_OFFSET) * ROTATE_EQUIVALENT * gear; //逆时针为正ch0控制自转方向
		if(rc_value.s1 == RC_SW_MID)			//左开关S1置中间时为手动模式
			BaseMove(speed[2],speed_dir,rotate_w,0,0,0,0);   //手动控制底盘
}

//@name: Point2Point_Move
//@brief: 点对点移动自动控制
//@param: float target_x		目标X坐标
//@param: float target_y		目标Y坐标
//@param: float begin_x			起始X坐标
//@param: float begin_y			起始Y坐标
//@param: float target_z		目标偏航角
//@param: float begin_z			起始偏航角
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
//@brief: 自动控制速度计算
//@param: float now_distance	当前位置离目标点的绝对距离
//@param: float max_speed			能允许运行的最大速度
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
//@brief: 自动控制偏航角计算
//@param: float target_z				目标偏航角
//@param: float begin_z					起始偏航角
//@param: float now_distance	  当前位置离目标点的绝对距离
//@param: float target_distance	起始点与目标点的绝对距离
float Angle_Cal(float target_z,float begin_z,float now_distance,float target_distance)
{
	return ((target_z - begin_z)*(1.0f - now_distance/target_distance) + begin_z);
}
