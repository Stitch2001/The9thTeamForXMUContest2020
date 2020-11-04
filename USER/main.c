#include "rcs.h"
/******************����������Ϣ************************/
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);
/******************���̿���������Ϣ********************/
//�������ȼ�
#define CHASSIS_TASK_PRIO		5
//�����ջ��С
#define CHASSIS_STK_SIZE 		256
//������ƿ�
OS_TCB ChassisTaskTCB;
//�����ջ
CPU_STK CHASSIS_TASK_STK[CHASSIS_STK_SIZE];
//������
void chassis_task(void *p_arg);
/******************LED����������Ϣ*************/
//�������ȼ�
/*#define LED_TASK_PRIO		6
//�����ջ��С
#define LED_STK_SIZE 		256
//������ƿ�
OS_TCB LEDTaskTCB;
//�����ջ
CPU_STK LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *p_arg);*/
/**********************************************/

float init_angle;//��е�۵����ʼ�Ƕ�

static void MainTask_Init(void);

int main()
{
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);  	//ʱ�ӳ�ʼ��
	OS_CPU_SysTickInit(SystemCoreClock/OSCfg_TickRate_Hz);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	INTX_DISABLE();		//�ر��жϣ���ֹ�δ�ʱ���������ʼ������

	INTX_ENABLE();		//�����ж�
	OSInit(&err);		//��ʼ��UCOSIII

	OS_CRITICAL_ENTER();//�����ٽ�����
	//��ʼ��������
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
	             (CPU_CHAR	* )"start task", 		//������
	             (OS_TASK_PTR )start_task, 			//������
	             (void		* )0,					//���ݸ�����Ĳ���
	             (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
	             (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
	             (CPU_STK_SIZE)START_STK_SIZE / 10,	//�����ջ�����λ
	             (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
	             (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����ܵ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
	             (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ����,Ϊ0ʱΪĬ�ϳ���
	             (void   	* )0,					//�û�����Ĵ�����
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //����ѡ��
	             (OS_ERR 	* )&err);				//��Ÿú�������ʱ�Ĵ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSStart(&err);  //����UCOSIII
	while(1);
}


//��������������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	CPU_Init();
	
	MainTask_Init();	//���ܳ�ʼ��
	delay_ms(500);//��ʼ��ʱ��
	
#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);  	//ͳ������
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
	CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	//ʹ��ʱ��Ƭ��ת�ĵ��ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ���,��1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif
	OS_CRITICAL_ENTER();	//�����ٽ���
	
	//�������̿�������CHASSIS
	OSTaskCreate((OS_TCB 	* )&ChassisTaskTCB,					//������ƿ�
	             (CPU_CHAR	* )"chassis task",				//������
	             (OS_TASK_PTR )chassis_task,					//������
	             (void		* )0,												//���ݸ�����Ĳ���
	             (OS_PRIO	  )CHASSIS_TASK_PRIO,				//�������ȼ�
	             (CPU_STK   * )&CHASSIS_TASK_STK[0],	//�����ջ����ַ
	             (CPU_STK_SIZE)CHASSIS_STK_SIZE / 10, //�����ջ�����λ
	             (CPU_STK_SIZE)CHASSIS_STK_SIZE,			//�����ջ��С
	             (OS_MSG_QTY  )0,											//�����ڲ���Ϣ�����ܹ����ܵ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
	             (OS_TICK	  )0,												//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ����,Ϊ0ʱΪĬ�ϳ���
	             (void   	* )0,												//�û�����Ĵ�����
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //����ѡ��
	             (OS_ERR 	* )&err);										//��Ÿú�������ʱ�Ĵ���ֵ
							 
  //����LED��������LED
	/*OSTaskCreate((OS_TCB 	* )&LEDTaskTCB,							//������ƿ�
	             (CPU_CHAR	* )"led task",						//������
	             (OS_TASK_PTR )led_task,							//������
	             (void		* )0,												//���ݸ�����Ĳ���
	             (OS_PRIO	  )LED_TASK_PRIO,						//�������ȼ�
	             (CPU_STK   * )&LED_TASK_STK[0],			//�����ջ����ַ
	             (CPU_STK_SIZE)LED_STK_SIZE / 10,			//�����ջ�����λ
	             (CPU_STK_SIZE)LED_STK_SIZE,					//�����ջ��С
	             (OS_MSG_QTY  )0,											//�����ڲ���Ϣ�����ܹ����ܵ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
	             (OS_TICK	  )0,												//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ����,Ϊ0ʱΪĬ�ϳ���
	             (void   	* )0,												//�û�����Ĵ�����
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,//����ѡ��
	             (OS_ERR 	* )&err);										//��Ÿú�������ʱ�Ĵ���ֵ*/
	

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB, &err);		//��������
	//OS_TaskResume((OS_TCB*)&StartTaskTCB,&err);		//��������
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	while(1)
		delay_ms(5);
}

//chassis���̿������񡾼���е�����׿�������
void chassis_task(void *p_arg)
{

	p_arg = p_arg;
	while(1) 
	{
		Remote_Ctrl(init_angle);
		delay_ms(5);
	}
}

//led��������
/*void led_task(void *p_arg)
{
	p_arg = p_arg;
	while(1) 
	{
		delay_ms(1000);
	}
}*/


static void MainTask_Init()				//��ʼ������
{
		Ctrl_BaseMove_Init();		//���̿��Ƴ�ʼ�����������ֱ�����̵����ʼ����
		init_angle = M3508_Angle_Init(); 		//��е�۵����ʼ��
		//RCS_GPIO_Output_Init(GPIOC,GPIO_Pin_4);
}


