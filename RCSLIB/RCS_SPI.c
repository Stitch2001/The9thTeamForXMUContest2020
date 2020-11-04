//@filename: RCS_spi.c
//@brief: spi通讯
#include "rcs.h"
#include "RCS_SPI.h"

//@name: InitSpi
//@brief: SPI初始化
//@param:SPI_TypeDef *SPIx spix : SPI1 、SPI2 or SPI3
//@param:uint16_t _mode spi模式 : SPI_Mode_Master or SPI_Mode_Slave
//@param:GPIO_TypeDef *_clkG 时钟GPIO :
//@param:uint32_t      _clkP 时钟Pin
//@param:GPIO_TypeDef *_mosiG mosiGPIO
//@param:uint32_t      _mosiP mosiPin
//@param:GPIO_TypeDef *_misoG misoGPIO
//@param:uint32_t      _misoP misoPin
//@param:uint16_t _CPOL CPOL	SPI_Clock_Polarity
//@param:uint16_t _CPHA CPHA  SPI_Clock_Phase

void InitSpi(SPI_TypeDef *_s, uint16_t _mode,
			 GPIO_TypeDef *_clkG, uint32_t _clkP,
			 GPIO_TypeDef *_mosiG, uint32_t _mosiP,
			 GPIO_TypeDef *_misoG, uint32_t _misoP,
			 uint16_t _CPOL,uint16_t _CPHA)
{
	
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_Struct;
	
	assert_param(IS_SPI_CPOL(_CPOL));
	assert_param(IS_SPI_CPHA(_CPHA));
	if (_s == SPI1)
		RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_SPI(_s), ENABLE);
	else
		RCC_APB1PeriphClockCmd(GetRCS_RCC_APB1Periph_SPI(_s), ENABLE);

	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_clkG), ENABLE);
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_mosiG), ENABLE);
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_misoG), ENABLE);

	GPIO_PinAFConfig(_clkG, GetRCS_GPIO_PinSource(_clkP), GetRCS_GPIO_AF_SPI(_s));
	GPIO_PinAFConfig(_mosiG, GetRCS_GPIO_PinSource(_mosiP), GetRCS_GPIO_AF_SPI(_s));
	GPIO_PinAFConfig(_misoG, GetRCS_GPIO_PinSource(_misoP), GetRCS_GPIO_AF_SPI(_s));

	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = _clkP;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(_clkG, &GPIO_Struct);

	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = _mosiP;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(_mosiG, &GPIO_Struct);

	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = _misoP;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(_misoG, &GPIO_Struct);


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

	SPI_InitStructure.SPI_Mode = _mode;

	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;

	SPI_InitStructure.SPI_CPOL = _CPOL;
	SPI_InitStructure.SPI_CPHA = _CPHA;
	if (_mode == SPI_Mode_Master)
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	else
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(_s, &SPI_InitStructure);
	SPI_CalculateCRC(_s,DISABLE);
	SPI_Cmd(_s, ENABLE);
	SPI_CalculateCRC(_s,DISABLE);
	
}

//@name: RM_spi_Init
//@brief: RM_OLED SPI初始化（RM开发板OLED的SPI配置）
//@param:SPI_TypeDef *SPIx spix
//@param:GPIO_TypeDef *_clkG 时钟GPIO
//@param:uint32_t      _clkP 时钟Pin
//@param:GPIO_TypeDef *_mosiG mosiGPIO
//@param:uint32_t      _mosiP mosiPin
//@param:GPIO_TypeDef *_misoG misoGPIO
//@param:uint32_t      _misoP misoPin

void RM_spi_Init(SPI_TypeDef *SPIx,
			 GPIO_TypeDef *_clkG, uint32_t _clkP,
			 GPIO_TypeDef *_mosiG, uint32_t _mosiP,
			 GPIO_TypeDef *_misoG, uint32_t _misoP)
{
	
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_Struct;
	
	if (SPIx == SPI1)
	{
		RCC_APB2PeriphClockCmd(GetRCS_RCC_APB2Periph_SPI(SPIx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(GetRCS_RCC_APB1Periph_SPI(SPIx), ENABLE);
	}

	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_clkG), ENABLE);
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_mosiG), ENABLE);
	RCC_AHB1PeriphClockCmd(GetRCS_RCC_AHB1Periph_GPIO(_misoG), ENABLE);

	GPIO_PinAFConfig(_clkG, GetRCS_GPIO_PinSource(_clkP), GetRCS_GPIO_AF_SPI(SPIx));
	GPIO_PinAFConfig(_mosiG, GetRCS_GPIO_PinSource(_mosiP), GetRCS_GPIO_AF_SPI(SPIx));
	GPIO_PinAFConfig(_misoG, GetRCS_GPIO_PinSource(_misoP), GetRCS_GPIO_AF_SPI(SPIx));

	

	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = _clkP;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(_clkG, &GPIO_Struct);

	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = _mosiP;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(_mosiG, &GPIO_Struct);

	GPIO_StructInit(&GPIO_Struct);
	GPIO_Struct.GPIO_Pin = _misoP;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
#ifdef  OD_OUTPUT
	GPIO_Struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
#else
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
#endif
	GPIO_Init(_misoG, &GPIO_Struct);


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;

	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 10;
	SPI_Init(SPIx, &SPI_InitStructure);
	SPI_CalculateCRC(SPIx,DISABLE);
	SPI_Cmd(SPIx, ENABLE);
	SPI_CalculateCRC(SPIx,DISABLE);
}
//SPI发送数据
//@name: SPIx_WriteByte
//@brief: SPI发送数据
//@param:SPI_TypeDef *SPIx spix
//@param:u8 TxData				 数据
void SPIx_WriteByte(SPI_TypeDef *SPIx ,u8 TxData)
{                                          
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET){} 
        SPI_I2S_SendData(SPIx,TxData); 
}