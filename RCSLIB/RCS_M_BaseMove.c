/*@filename: RCS_M_BaseMove.c
 *@author     陈志伟       
 *@brief:     X型麦轮底盘速度解算
 *@date: 2020-7-17
*/
#include "RCS_M_BaseMove.h"

/*******************
X型长方形麦轮底盘，默认CAN1通讯
电机编号、相对坐标系及电机正转时轮子的有效方向如下
        ↑Y
        |
 1↗----+---↘2
   |    |   |
   |    +---+----→X
   |        |长
 4↖--------↙3
       宽

绝对坐标系定义如下
       ↑Y
       |
       |
       |
     Z +---------→ X      
********************/

/********全局变量********/

static float wheel_dir[4];		   //电机正转时轮子运动的正方向
static float wheel_distance;		 //轮子和底盘中心得距离
static float wheel_w_dir[4];		 //底盘自转时自转线速度在各轮子上的方向

PID_Struct speed_pid;	//速度环PID
PID_Struct angle_pid;	//姿态环PID
PID_Struct coord_pid; //坐标反馈PID

/********静态函数********/
static void Wheel_Init(void);
static void M_PID_Init(void);

/************************/

//麦轮底盘初始化
void M_BaseMove_Init(void)
{
		Motor_Init();		//电机初始化
		M_PID_Init();		//PID初始化
		Wheel_Init();		//轮子参数初始化
}

//@name: BaseMove
//@brief: 麦轮底盘核心算法
//@param:float move_speed               平动速度(mm/s)
//@param:float move_direction           平动的方向(deg目标方向与y轴夹角,以逆时针为正，
//																								自动控制时是绝对坐标系下的方向，
//																								在手动控制下是相对车身坐标系的方向)
//@param:float rotate_speed             自转模式(rad/s逆时针为正,手动控制时开启)
//@param:float target_angle 				    偏航角模式(deg逆时针为正,自动控制时开启)
//@param:int feedback_flag              坐标反馈控制标志 0 手动模式,1 对x反馈调节,2 对y反馈调节,3 对(x,y)反馈调节
//@param:float target_pos_x             目标x坐标
//@param:float target_pos_y             目标y坐标
//feedback_flag为0时为手动控制模式，否则为自动控制模式。
void BaseMove(float move_speed,	 float move_direction,float rotate_speed,
							float target_angle,int feedback_flag,		float target_pos_x,
							float target_pos_y)
{
    float target_speed[4] = {0,0,0,0};			//目标转速
		float feedback_speed[2] = {0,0};				//X、Y方向反馈调节的速度
		float w_size =0;												//自转线速度大小
		float z_angle =0,now_pos_x=0,now_pos_y=0;				//当前偏航角 单位rad
		int16_t now_speed[4]={0,0,0,0}; 				//每个电机编码器当前转速
		int32_t out[4]={0,0,0,0};
		int i=1;
		
		now_pos_x = RCS_Get_X();
		now_pos_y = RCS_Get_Y();
		
		if(feedback_flag)		//判断自动模式还是手动模式
		{
			z_angle = RCS_Get_Z() * DEG2RAD;	//获取偏航角
			w_size = PID_Degree_Ctrl(target_angle,z_angle,angle_pid);
			//获取坐标反馈调节X方向速度和Y方向速度(坐标环PID)
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
				target_speed[i] += move_speed * cos(move_direction * DEG2RAD - z_angle - wheel_dir[i]); //平动速度在轮毂正方向投影
				target_speed[i] += w_size * cos(wheel_w_dir[i] - wheel_dir[i]);									//转动线速度在轮毂正方向投影
				target_speed[i] += feedback_speed[0] * cos(-PI/2.0f- z_angle - wheel_dir[i]);		//绝对坐标系的X方向反馈速度在轮毂正方向投影
				target_speed[i] += feedback_speed[1] * cos(   0    - z_angle - wheel_dir[i]);		//Y反馈速度在轮毂正方向投影
				target_speed[i] *= MM_S2R_MIN;				//将轮毂正方向线速度转化为轮子转速
				target_speed[i] *= SLOWDOWN_RATE;		  //将轮子转速转化为编码器转速以便于PID调速
				now_speed[i] = Get_Motor_Speed(i+1);	//获取各电机编码器转速
				out[i] = PID_Mwheel_Speed_Ctrl(target_speed[i],now_speed[i],i+1,speed_pid); //速度环
		}
		Motor_Send(out[0], out[1], out[2], out[3]);
}

//PID初始化
static void M_PID_Init(void)
{
		//速度环PID初始化
		speed_pid.P = 8.3;
		speed_pid.I = 0.25;
	  speed_pid.D = 0.38;
		
		//偏航角反馈调节PID初始化
		angle_pid.P = 70.0;
	  angle_pid.I = 0.03;
	  angle_pid.D = 4.0;
	
		//坐标反馈调节PID初始化
		coord_pid.P = 2.0;
		coord_pid.I = 0.03;
		coord_pid.D = 0;
}

//轮子参数初始化
static void Wheel_Init(void)
{
	  //设置轮毂正方向（见最上方图例）
		wheel_dir[0] = -PI/4.0f;			
		wheel_dir[1] = 3 * wheel_dir[0];
		wheel_dir[2] = -wheel_dir[1];
		wheel_dir[3] = -wheel_dir[0];
		//设置自转时自转线速度在各轮子上的方向
		wheel_w_dir[0] = atan2( BASE_WIDTH, BASE_LENGTH) + PI/2.0f;
		wheel_w_dir[1] = atan2(-BASE_WIDTH, BASE_LENGTH) + PI/2.0f;
		wheel_w_dir[2] = atan2(-BASE_WIDTH,-BASE_LENGTH) + PI/2.0f;
		wheel_w_dir[3] = atan2( BASE_WIDTH,-BASE_LENGTH) + PI/2.0f;
		//计算轮子与底盘中心距离
		wheel_distance = sqrt(BASE_LENGTH * BASE_LENGTH + BASE_WIDTH * BASE_WIDTH) / 2.0f;
}
