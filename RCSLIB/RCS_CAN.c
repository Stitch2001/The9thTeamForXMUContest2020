//@filename: RCS_CAN.c
//@brief: CAN通讯

#include "RCS_CAN.h"

//@name: RCS_CANx_Recieve
//@brief: CAN接收数据
//@param:CAN_TypeDef *CANx 	CAN通道
//@param:CanRxMsg* RxMessage 数据包
void RCS_CANx_Recieve(CAN_TypeDef *CANx,CanRxMsg* RxMessage)
{
    CAN_Receive(CANx, CAN_FIFO0, RxMessage);
    CAN_FIFORelease(CANx,CAN_FIFO0);
}

//CAN1中断函数(使用时需要映射成CAN1_RX0_IRQHandler)
void CAN1_RX0_Interrupt(void) 
{
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
    {
        CanRxMsg RxMessage;
        RCS_CANx_Recieve(CAN1,&RxMessage);
        CAN_FIFORelease(CAN1,CAN_FIFO0);
    }   
}

//CAN2中断服务函数(使用时需要映射成CAN2_RX0_IRQHandler)
void CAN2_RX0_Interrupt(void) 
{
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
    {
        CanRxMsg RxMessage;
        RCS_CANx_Recieve(CAN2,&RxMessage);
        CAN_FIFORelease(CAN2,CAN_FIFO0);
    }   
}

//@name: RCS_CANx_Send
//@brief: CAN发送数据
//@param:CAN_TypeDef *CANx	CAN组
//@param:uint32_t Id	发送给对应ID号
//@param:uint8_t Length 发送数据长度
//@param:uint8_t* sendData  数据
void RCS_CANx_Send(CAN_TypeDef *CANx,uint32_t Id, uint8_t Length, uint8_t* sendData)
{
		int i;
    CanTxMsg TxMessage;
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.StdId = Id;
    TxMessage.ExtId = Id;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.DLC = Length;

    for(i = 0;i < Length; i++)
    {
        TxMessage.Data[i] = sendData[i];
    }
    CAN_Transmit(CANx, &TxMessage);
}


//@name: CAN1_Config
//@brief: CAN1初始化
//@param:GPIO_TypeDef *GPIOx 	GPIO组
//@param:uint32_t   _GPIO_PinX_Rx 接受管脚号
//@param:uint32_t   _GPIO_PinX_Tx 发送管脚号
//@param: _baudRate  波特率(能被整除3000000)
void RCS_CAN1_Config(GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_Rx, uint32_t _GPIO_PinX_Tx, uint32_t _baudRate)
{
    // 定义结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    RCC_ClocksTypeDef rcc_clocks;                   //开启时钟
    RCC_GetClocksFreq(&rcc_clocks);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = _GPIO_PinX_Rx | _GPIO_PinX_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(_GPIOx, &GPIO_InitStructure);
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Rx), GPIO_AF_CAN1);	//开启管脚复用功能
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Tx), GPIO_AF_CAN1);

    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_TXFP = ENABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
		CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_Prescaler = 6000000 / _baudRate;//波特率计算公式:CAN时钟频率/((SJW + BS1 + BS2) * Prescaler)
    // CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency/ (7 * _baudRate);   
    CAN_Init(CAN1, &CAN_InitStructure);


/*******************************CAN过滤器设置********************************/
    // CAN_FilterInit(&CAN_FilterInitStructure);     //初始化CAN_FilterInitStructrue结构体变量 
    CAN_FilterInitStructure.CAN_FilterNumber = 0; //选择过滤器0,范围为0~13 
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//设置过滤器组0为屏蔽模式  
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//设置过滤器组0位宽为32位

    //标识位寄存器设置
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;  //设置标识位寄存器的高字节
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;   //设置标识位寄存器的低字节

    //屏蔽寄存器设置
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //全部接收是0x0000;屏蔽位的多机通讯ffff
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;//全部接收是0x0000;屏蔽位的多机通讯ffff


    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0; //此过滤器关联到FIFO0  
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活此过滤器组
    CAN_FilterInit(&CAN_FilterInitStructure); // //设置过滤器
/*********************************************************************************/


    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

//@name: CAN2_Config
//@brief: CAN2初始化
//@param:GPIO_TypeDef *GPIOx 	GPIO组
//@param:uint32_t   _GPIO_PinX_Rx 接受管脚号
//@param:uint32_t   _GPIO_PinX_Tx 发送管脚号
//@param: _baudRate  波特率(能被整除3000000)    
void RCS_CAN2_Config(GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_Rx, uint32_t _GPIO_PinX_Tx, uint32_t _baudRate)
{
    // 定义结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    RCC_ClocksTypeDef rcc_clocks;                   //开启时钟
    RCC_GetClocksFreq(&rcc_clocks);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);	//CAN2时钟可以不开，但CAN1时钟必须开
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = _GPIO_PinX_Rx | _GPIO_PinX_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(_GPIOx, &GPIO_InitStructure);
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Rx), GPIO_AF_CAN2);	//开启管脚复用功能
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Tx), GPIO_AF_CAN2);

    CAN_DeInit(CAN2);
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_TXFP = ENABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
		CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_Prescaler = 6000000 / _baudRate;//波特率计算公式:CAN时钟频率/((SJW + BS1 + BS2) * Prescaler)
    // CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency/ (7 * _baudRate);   
    CAN_Init(CAN2, &CAN_InitStructure);


/*******************************CAN过滤器设置********************************/
    // CAN_FilterInit(&CAN_FilterInitStructure);     //初始化CAN_FilterInitStructrue结构体变量 
    CAN_FilterInitStructure.CAN_FilterNumber = 14; //选择过滤器0,范围为14~27 
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//设置过滤器组0为屏蔽模式  
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//设置过滤器组0位宽为32位

    //标识位寄存器设置
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;  //设置标识位寄存器的高字节
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;   //设置标识位寄存器的低字节

    //屏蔽寄存器设置
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //全部接收是0x0000;屏蔽位的多机通讯ffff
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;//全部接收是0x0000;屏蔽位的多机通讯ffff


    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0; //此过滤器关联到FIFO0  
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活此过滤器组
    CAN_FilterInit(&CAN_FilterInitStructure); // //设置过滤器
/*********************************************************************************/


    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}
