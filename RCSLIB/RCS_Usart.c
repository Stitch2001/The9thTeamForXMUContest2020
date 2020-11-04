//@filename:RCS_usart.c
//@author:  Morris Zhang
//@date:    18-August-2012
//@brief: USART通讯模块
#include "RCS_Usart.h"

/*****************静态函数定义**************/
static void USART_GPIO_Pin_Config(USART_TypeDef *_USARTx, GPIO_TypeDef *_GPIOx,
                                  uint32_t _GPIO_PinX_T, uint32_t _GPIO_PinX_R);

static void USART_Config(USART_TypeDef *_USARTx, uint32_t _baudRate);

static void USART_Int_Config(USART_TypeDef *_USARTx, uint8_t _pri);
static void UART5_GPIO_Pin_Config(void);
/*******************************************/

//@name: USART_Config
//@brief: 配置USART串口通讯功能
//@param: _USARTx          需要绑定的USARTx
//@param: _GPIOx           需要绑定的GPIOx
//@param: _GPIO_PinX_T     串口输出通道
//@param:_GPIO_PinX_R      串口接受通道
//@param:_baudRate         波特率
//@param: _pri             优先级：前4位抢占优先级，后4位响应优先级
//串口接收中断标志清除函数
//USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
void RCS_USART_Config(USART_TypeDef *_USARTx, GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_T,
                      uint32_t _GPIO_PinX_R, uint32_t _baudRate, uint8_t _pri)
{
	if (_USARTx == UART5)
	{
		UART5_GPIO_Pin_Config();
	}
	else
	{
		USART_GPIO_Pin_Config(_USARTx, _GPIOx, _GPIO_PinX_T, _GPIO_PinX_R);
	}
	USART_Config(_USARTx, _baudRate);

	USART_Int_Config(_USARTx, _pri);
}

void RCS_UART5_Config(void)
{
	UART5_GPIO_Pin_Config();
	USART_Config(UART5, 9600);
	USART_Int_Config(UART5,0x23);
}

//@name: USART_GPIO_Pin_Config
//@brief: USART5 PC12 PD2配置
		
static void UART5_GPIO_Pin_Config(void)
{
	GPIO_InitTypeDef GPIO_Struct;

	// 开启时钟
	RCC_APB1PeriphClockCmd(GetRCS_RCC_APB1Periph_USART(UART5), ENABLE);

	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(GPIOC), ENABLE);
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(GPIOD), ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// 开启管脚复用功能
	GPIO_PinAFConfig(GPIOC, GetRCS_GPIO_PinSource(GPIO_Pin_12), GetRCS_GPIO_AF(UART5));
	GPIO_PinAFConfig(GPIOD, GetRCS_GPIO_PinSource(GPIO_Pin_2), GetRCS_GPIO_AF(UART5));

	//配置接收管脚
	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(GPIOC, &GPIO_Struct);

	//配置发送管脚
	GPIO_Struct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif

	GPIO_Init(GPIOD, &GPIO_Struct);
}

static void USART_GPIO_Pin_Config(USART_TypeDef *_USARTx, GPIO_TypeDef *_GPIOx,
                                  uint32_t _GPIO_PinX_T, uint32_t _GPIO_PinX_R)
{
	//创建GPIO结构体
	GPIO_InitTypeDef GPIO_Struct;

	//开启时钟
	if (_USARTx == USART1 || _USARTx == USART6)
	{

		RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_USART(_USARTx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(GetRCS_RCC_APB1Periph_USART(_USARTx), ENABLE);
	}
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_GPIOx), ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//开启管脚的串口复用功能
	GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_T), GetRCS_GPIO_AF(_USARTx));
	GPIO_PinAFConfig(_GPIOx, GetRCS_GPIO_PinSource(_GPIO_PinX_R), GetRCS_GPIO_AF(_USARTx));

	//配置发送管脚
	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = _GPIO_PinX_T;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(_GPIOx, &GPIO_Struct);

	//配置接收管脚
	GPIO_Struct.GPIO_Pin = _GPIO_PinX_R;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif

	GPIO_Init(_GPIOx, &GPIO_Struct);
}

//@name: USART_Config
//@brief: 配置串口
//@param:_USARTx       USART号
//@param:_baudRate     波特率
static void USART_Config(USART_TypeDef *_USARTx, uint32_t _baudRate)
{
	//创建串口结构体
	USART_InitTypeDef USART_InitStructure;

	// 配置USART
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = _baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(_USARTx, &USART_InitStructure);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_Cmd(_USARTx, ENABLE);

	//串口接收使能
	USART_ITConfig(_USARTx, USART_IT_RXNE, ENABLE);
}

//@name: USART_Int_Config
//@brief:USART接收中断配置
//@param: _USARTx       USART号
//@param: _pri          优先级
static void USART_Int_Config(USART_TypeDef *_USARTx,  uint8_t _pri)
{
	
	NVIC_InitTypeDef nvic_initStruct;
	nvic_initStruct.NVIC_IRQChannel = GetRCS_USART_IRQn(_USARTx);
	nvic_initStruct.NVIC_IRQChannelPreemptionPriority = (_pri >> 4 ) & 0x0f;
	nvic_initStruct.NVIC_IRQChannelSubPriority = _pri & 0x0f;
	nvic_initStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_initStruct);
}
