#include "rcs.h"
/******************启动任务信息************************/
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);
/******************底盘控制任务信息********************/
//任务优先级
#define CHASSIS_TASK_PRIO		5
//任务堆栈大小
#define CHASSIS_STK_SIZE 		256
//任务控制块
OS_TCB ChassisTaskTCB;
//任务堆栈
CPU_STK CHASSIS_TASK_STK[CHASSIS_STK_SIZE];
//任务函数
void chassis_task(void *p_arg);
/******************LED亮灭任务信息*************/
//任务优先级
/*#define LED_TASK_PRIO		6
//任务堆栈大小
#define LED_STK_SIZE 		256
//任务控制块
OS_TCB LEDTaskTCB;
//任务堆栈
CPU_STK LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *p_arg);*/
/**********************************************/
static void MainTask_Init(void);

int main()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);  	//时钟初始化
	OS_CPU_SysTickInit(SystemCoreClock/OSCfg_TickRate_Hz);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	INTX_DISABLE();		//关闭中断，防止滴答定时器对外设初始化打扰

	INTX_ENABLE();		//开启中断
	OSInit(&err);		//初始化UCOSIII

	OS_CRITICAL_ENTER();//进入临界区域
	//开始创建任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制快
	             (CPU_CHAR	* )"start task", 		//任务名
	             (OS_TASK_PTR )start_task, 			//任务函数
	             (void		* )0,					//传递给任务的参数
	             (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
	             (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
	             (CPU_STK_SIZE)START_STK_SIZE / 10,	//任务堆栈深度限位
	             (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
	             (OS_MSG_QTY  )0,					//任务内部消息队列能够接受的最大消息数目,为0时禁止接受消息
	             (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度,为0时为默认长度
	             (void   	* )0,					//用户补充的储存区
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //任务选项
	             (OS_ERR 	* )&err);				//存放该函数错误时的储存值
	OS_CRITICAL_EXIT();	//退出临界区
	OSStart(&err);  //开启UCOSIII
	while(1);
}


//开启启动任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
	
	MainTask_Init();	//功能初始化
	
	delay_ms(500);//初始化时间
	
#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
	CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	//使能时间片轮转的调度功能,时间片长度为1个系统时钟节拍,就1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建底盘控制任务CHASSIS
	OSTaskCreate((OS_TCB 	* )&ChassisTaskTCB,					//任务控制快
	             (CPU_CHAR	* )"chassis task",				//任务名
	             (OS_TASK_PTR )chassis_task,					//任务函数
	             (void		* )0,												//传递给任务的参数
	             (OS_PRIO	  )CHASSIS_TASK_PRIO,				//任务优先级
	             (CPU_STK   * )&CHASSIS_TASK_STK[0],	//任务堆栈基地址
	             (CPU_STK_SIZE)CHASSIS_STK_SIZE / 10, //任务堆栈深度限位
	             (CPU_STK_SIZE)CHASSIS_STK_SIZE,			//任务堆栈大小
	             (OS_MSG_QTY  )0,											//任务内部消息队列能够接受的最大消息数目,为0时禁止接受消息
	             (OS_TICK	  )0,												//当使能时间片轮转时的时间片长度,为0时为默认长度
	             (void   	* )0,												//用户补充的储存区
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //任务选项
	             (OS_ERR 	* )&err);										//存放该函数错误时的储存值
							 
  //创建LED控制任务LED
	/*OSTaskCreate((OS_TCB 	* )&LEDTaskTCB,							//任务控制快
	             (CPU_CHAR	* )"led task",						//任务名
	             (OS_TASK_PTR )led_task,							//任务函数
	             (void		* )0,												//传递给任务的参数
	             (OS_PRIO	  )LED_TASK_PRIO,						//任务优先级
	             (CPU_STK   * )&LED_TASK_STK[0],			//任务堆栈基地址
	             (CPU_STK_SIZE)LED_STK_SIZE / 10,			//任务堆栈深度限位
	             (CPU_STK_SIZE)LED_STK_SIZE,					//任务堆栈大小
	             (OS_MSG_QTY  )0,											//任务内部消息队列能够接受的最大消息数目,为0时禁止接受消息
	             (OS_TICK	  )0,												//当使能时间片轮转时的时间片长度,为0时为默认长度
	             (void   	* )0,												//用户补充的储存区
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,//任务选项
	             (OS_ERR 	* )&err);										//存放该函数错误时的储存值*/
	

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB, &err);		//挂起任务
	//OS_TaskResume((OS_TCB*)&StartTaskTCB,&err);		//唤醒任务
	OS_CRITICAL_EXIT();	//退出临界区
	while(1)
		delay_ms(5);
}

//chassis底盘控制任务【及机械臂气缸控制任务】
void chassis_task(void *p_arg)
{

	p_arg = p_arg;
	while(1) 
	{
		Remote_Ctrl();
		delay_ms(5);
	}
}

//led亮灭任务
/*void led_task(void *p_arg)
{
	p_arg = p_arg;
	while(1) 
	{
		delay_ms(1000);
	}
}*/


static void MainTask_Init()				//初始化函数
{
		Ctrl_BaseMove_Init();		//底盘控制初始化（包括了手柄与底盘电机初始化）
		Motor_Init2();		//CAN2初始化（用于控制机械臂）
		//RCS_GPIO_Output_Init(GPIOC,GPIO_Pin_4);
}


