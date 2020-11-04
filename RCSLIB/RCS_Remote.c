/*@filename: RCS_Remoute.c
 *@author     ≥¬÷æŒ∞       
 *@brief:     RM ÷±˙Ω” ‹∂À≈‰÷√
 *@date: 2020-7-17
*/

#include "RCS_Remote.h"

/* ----------------------- Ω” ‹∞¸±‰¡ø ----------------------------------- */
unsigned char sbus_rx_buffer[2][RC_FRAME_LENGTH]; 
static rc_ctrl rc;
static mouse_ctrl mouse;
static key_ctrl key;
/* ----------------------- ∫Ø ˝∂®“Â ---------------------------- */

static void RemoteDataProcess(uint8_t *pData);

/******************************************************************************
* @fn Get_RC_Value
* @brief ªÒ»°“£øÿ÷µ
* @return rc_ctrl
*******************************************************************************/
void Get_RC_Value(rc_ctrl *rc_value)
{
	rc_value->ch0 = rc.ch0;
	rc_value->ch1 = rc.ch1;
	rc_value->ch2 = rc.ch2;
	rc_value->ch3 = rc.ch3;
	rc_value->s1 = rc.s1;
	rc_value->s2 = rc.s2;
}
/******************************************************************************
* @fn Get_Key_Value
* @brief ªÒ»°º¸≈Ã÷µ
* @return key_ctrl
*******************************************************************************/
void Get_Key_Value(key_ctrl *key_value)
{
	key_value->v = key.v;
}

/******************************************************************************
* @fn Get_Mouse_Value
* @brief ªÒ»° Û±Í÷µ
* @return rc_ctrl
*******************************************************************************/
void Get_Mouse_Value(mouse_ctrl *mouse_value)
{
	mouse_value->press_l = mouse.press_l;
	mouse_value->press_r = mouse.press_r;
	mouse_value->x = mouse.x;
	mouse_value->y = mouse.y;
	mouse_value->z = mouse.z;
}

/******************************************************************************
* @fn RC_Init
* 
* @brief configure stm32 usart2 port
* - USART Parameters
* - 100Kbps
* - 8-N-1
* - DMA Mode
* 
* @return None.
* 
* @note This code is fully tested on STM32F405RGT6 Platform, You can port 
it
* to the other platform. Using doube buffer to receive data prevent 
losing data.
*/
void RC_Init(void)
{
    USART_InitTypeDef usart2;
		GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);
	
		gpio.GPIO_Pin = GPIO_Pin_3 ;
		gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&gpio);
    
    USART_DeInit(USART2);
		usart2.USART_BaudRate = 100000;   //SBUS 100K baudrate
		usart2.USART_WordLength = USART_WordLength_8b;
		usart2.USART_StopBits = USART_StopBits_1;
		usart2.USART_Parity = USART_Parity_Even;
		usart2.USART_Mode = USART_Mode_Rx;
		usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART2,&usart2);
    
		USART_Cmd(USART2,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
    
    nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    DMA_DeInit(DMA1_Stream5);
    dma.DMA_Channel= DMA_Channel_4;
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;//∏≥µÿ÷∑
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dma.DMA_BufferSize = 18;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_Mode_Normal;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5,&dma);

    DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Stream5,ENABLE);
}

/******************************************************************************
* @fn RemoteDataProcess
* 
* @brief ¥¶¿ÌΩ” ‹∫Ûµƒ ˝æ›
* @pData a point to rc receive buffer.
* @return None.
* @note RC_CtrlData is a global variable.you can deal with it in other place.
*/
static void RemoteDataProcess(uint8_t *pData)
{
		if(pData == NULL)
		{
			return;
		}
 
		rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF; 
		rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;
		rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) | \
												 ((int16_t)pData[4] << 10)) & 0x07FF;
		rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;
		rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
		rc.s2 = ((pData[5] >> 4) & 0x0003);
		mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
		mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
		mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8); 
		mouse.press_l = pData[12];
		mouse.press_r = pData[13];
		key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);
		//your control code Ö.
}

//RM“£øÿ∆˜Ω” ’÷–∂œ
void DMA1_Stream5_IRQHandler (void)
{
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
		RemoteDataProcess(sbus_rx_buffer[0]);
	DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
  DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);//«Â≥˝÷–∂œ±Í÷æŒª
}
