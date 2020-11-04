//@filename: RCS_CAN.c
//@brief: CANͨѶ

#include "RCS_CAN.h"

//@name: RCS_CANx_Recieve
//@brief: CAN��������
//@param:CAN_TypeDef *CANx 	CANͨ��
//@param:CanRxMsg* RxMessage ���ݰ�
void RCS_CANx_Recieve(CAN_TypeDef *CANx,CanRxMsg* RxMessage)
{
    CAN_Receive(CANx, CAN_FIFO0, RxMessage);
    CAN_FIFORelease(CANx,CAN_FIFO0);
}

//CAN1�жϺ���(ʹ��ʱ��Ҫӳ���CAN1_RX0_IRQHandler)
void CAN1_RX0_Interrupt(void) 
{
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
    {
        CanRxMsg RxMessage;
        RCS_CANx_Recieve(CAN1,&RxMessage);
        CAN_FIFORelease(CAN1,CAN_FIFO0);
    }   
}

//CAN2�жϷ�����(ʹ��ʱ��Ҫӳ���CAN2_RX0_IRQHandler)
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
//@brief: CAN��������
//@param:CAN_TypeDef *CANx	CAN��
//@param:uint32_t Id	���͸���ӦID��
//@param:uint8_t Length �������ݳ���
//@param:uint8_t* sendData  ����
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
//@brief: CAN1��ʼ��
//@param:GPIO_TypeDef *GPIOx 	GPIO��
//@param:uint32_t   _GPIO_PinX_Rx ���ܹܽź�
//@param:uint32_t   _GPIO_PinX_Tx ���͹ܽź�
//@param: _baudRate  ������(�ܱ�����3000000)
void RCS_CAN1_Config(GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_Rx, uint32_t _GPIO_PinX_Tx, uint32_t _baudRate)
{
    // ����ṹ��
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    RCC_ClocksTypeDef rcc_clocks;                   //����ʱ��
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
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Rx), GPIO_AF_CAN1);	//�����ܽŸ��ù���
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Tx), GPIO_AF_CAN1);

    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_TXFP = ENABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
		CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_Prescaler = 6000000 / _baudRate;//�����ʼ��㹫ʽ:CANʱ��Ƶ��/((SJW + BS1 + BS2) * Prescaler)
    // CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency/ (7 * _baudRate);   
    CAN_Init(CAN1, &CAN_InitStructure);


/*******************************CAN����������********************************/
    // CAN_FilterInit(&CAN_FilterInitStructure);     //��ʼ��CAN_FilterInitStructrue�ṹ����� 
    CAN_FilterInitStructure.CAN_FilterNumber = 0; //ѡ�������0,��ΧΪ0~13 
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//���ù�������0Ϊ����ģʽ  
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//���ù�������0λ��Ϊ32λ

    //��ʶλ�Ĵ�������
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;  //���ñ�ʶλ�Ĵ����ĸ��ֽ�
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;   //���ñ�ʶλ�Ĵ����ĵ��ֽ�

    //���μĴ�������
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //ȫ��������0x0000;����λ�Ķ��ͨѶffff
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;//ȫ��������0x0000;����λ�Ķ��ͨѶffff


    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0; //�˹�����������FIFO0  
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //����˹�������
    CAN_FilterInit(&CAN_FilterInitStructure); // //���ù�����
/*********************************************************************************/


    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

//@name: CAN2_Config
//@brief: CAN2��ʼ��
//@param:GPIO_TypeDef *GPIOx 	GPIO��
//@param:uint32_t   _GPIO_PinX_Rx ���ܹܽź�
//@param:uint32_t   _GPIO_PinX_Tx ���͹ܽź�
//@param: _baudRate  ������(�ܱ�����3000000)    
void RCS_CAN2_Config(GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_Rx, uint32_t _GPIO_PinX_Tx, uint32_t _baudRate)
{
    // ����ṹ��
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    RCC_ClocksTypeDef rcc_clocks;                   //����ʱ��
    RCC_GetClocksFreq(&rcc_clocks);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);	//CAN2ʱ�ӿ��Բ�������CAN1ʱ�ӱ��뿪
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = _GPIO_PinX_Rx | _GPIO_PinX_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(_GPIOx, &GPIO_InitStructure);
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Rx), GPIO_AF_CAN2);	//�����ܽŸ��ù���
    GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_Tx), GPIO_AF_CAN2);

    CAN_DeInit(CAN2);
    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_TXFP = ENABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
		CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_Prescaler = 6000000 / _baudRate;//�����ʼ��㹫ʽ:CANʱ��Ƶ��/((SJW + BS1 + BS2) * Prescaler)
    // CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency/ (7 * _baudRate);   
    CAN_Init(CAN2, &CAN_InitStructure);


/*******************************CAN����������********************************/
    // CAN_FilterInit(&CAN_FilterInitStructure);     //��ʼ��CAN_FilterInitStructrue�ṹ����� 
    CAN_FilterInitStructure.CAN_FilterNumber = 14; //ѡ�������0,��ΧΪ14~27 
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//���ù�������0Ϊ����ģʽ  
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//���ù�������0λ��Ϊ32λ

    //��ʶλ�Ĵ�������
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;  //���ñ�ʶλ�Ĵ����ĸ��ֽ�
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;   //���ñ�ʶλ�Ĵ����ĵ��ֽ�

    //���μĴ�������
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //ȫ��������0x0000;����λ�Ķ��ͨѶffff
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;//ȫ��������0x0000;����λ�Ķ��ͨѶffff


    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0; //�˹�����������FIFO0  
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //����˹�������
    CAN_FilterInit(&CAN_FilterInitStructure); // //���ù�����
/*********************************************************************************/


    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}
