//@filename: RCS_CAN.h
//@brief: CANͨѶ

#ifndef _RCS_CAN_H_
#define _RCS_CAN_H_

#include "rcs.h"
#include "stm32f4xx_can.h"
//#include "bsp.h"

void RCS_CANx_Recieve(CAN_TypeDef *CANx,CanRxMsg* RxMessage);
void RCS_CANx_Send(CAN_TypeDef *CANx,uint32_t Id, uint8_t Length, uint8_t* sendData);
void RCS_CAN1_Config(GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_Rx, uint32_t _GPIO_PinX_Tx, uint32_t _baudRate);
void RCS_CAN2_Config(GPIO_TypeDef *_GPIOx, uint32_t _GPIO_PinX_Rx, uint32_t _GPIO_PinX_Tx, uint32_t _baudRate);

#endif
