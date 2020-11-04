//@filename: RCS_spi.h
//@brief: spi
#ifndef _RCS_SPI_H_
#define _RCS_SPI_H_

#include "rcs.h"
//ͨ��SPI��ʼ��
void InitSpi(SPI_TypeDef *_s, uint16_t _mode,
			 GPIO_TypeDef *_clkG, uint32_t _clkP,
			 GPIO_TypeDef *_mosiG, uint32_t _mosiP,
			 GPIO_TypeDef *_misoG, uint32_t _misoP,
			 uint16_t _CPOL,uint16_t _CPHA);
//RM OLED SPI ��ʼ��
void RM_spi_Init(SPI_TypeDef *SPIx,
			 GPIO_TypeDef *_clkG, uint32_t _clkP,
			 GPIO_TypeDef *_mosiG, uint32_t _mosiP,
			 GPIO_TypeDef *_misoG, uint32_t _misoP);
//SPI��������
void SPIx_WriteByte(SPI_TypeDef *SPIx ,u8 TxData);


#endif
