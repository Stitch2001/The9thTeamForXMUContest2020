#ifndef _RCS_H_
#define _RCS_H_

#define DEG2RAD         1.74532925199433e-2f            //角度转弧度
#define RAD2DEG         5.72957795130823e1f							//弧度转角度
#define FLOAT_ZERO      1e-10f													//浮点0
#define PI							3.141592654f										//pi


/***********中断服务函数映射*****************/


/***********定时器*****************/


/***********LED*******************/
#define LED1_GPIO				GPIOA
#define LED1_PIN				GPIO_Pin_2			

#define LED2_GPIO				GPIOA
#define LED2_PIN				GPIO_Pin_8

#define LED3_GPIO				GPIOC
#define LED3_PIN				GPIO_Pin_13

#define LED4_GPIO				GPIOC
#define LED4_PIN				GPIO_Pin_14		

/************PWM*************************/
#define PWM1_TIM				TIM3
#define	PWM1_CH						3
#define PWM1_GPIO				GPIOC
#define PWM1_PIN				GPIO_Pin_8

#define PWM2_TIM				TIM2
#define	PWM2_CH						2
#define PWM2_GPIO				GPIOB
#define PWM2_PIN				GPIO_Pin_3

#define PWM3_TIM				TIM3
#define	PWM3_CH						1
#define PWM3_GPIO				GPIOB
#define PWM3_PIN				GPIO_Pin_4

#define PWM4_TIM				TIM3
#define	PWM4_CH						2
#define PWM4_GPIO				GPIOB
#define PWM4_PIN				GPIO_Pin_5
/************CAN*************************/
#define	CAN1_GPIO						GPIOA
#define	CAN1_TXD						GPIO_Pin_12
#define	CAN1_RXD						GPIO_Pin_11
#define CAN1_BAUD						1000000

#define	CAN2_GPIO						GPIOB
#define	CAN2_TXD						GPIO_Pin_13
#define	CAN2_RXD						GPIO_Pin_12
#define CAN2_BAUD						1000000

/************USART*************************/

#define RCS_GPS_USART				UART4
#define RCS_GPS_GPIO				GPIOC
#define RCS_GPS_TX					GPIO_Pin_10
#define RCS_GPS_RX					GPIO_Pin_11
#define RCS_GPS_BAUD				115200
#define RCS_GPS_PRI         0x10

/************RM_OLED*************************/

#define OLED_DC_GPIO_Port		GPIOB
#define OLED_DC_Pin					GPIO_Pin_9

#define OLED_SPI_MISO_GPIO  GPIOB
#define OLED_SPI_MISO_PIN		GPIO_Pin_4 

#define OLED_SPI_SCK_GPIO  	GPIOB
#define OLED_SPI_SCK_PIN		GPIO_Pin_3

#define OLED_SPI_MOSI_GPIO  GPIOA
#define OLED_SPI_MOSI_PIN		GPIO_Pin_7

#define OLED_RST_GPIO_Port	GPIOB
#define OLED_RST_Pin				GPIO_Pin_10

#define RM_OLED_SPI					SPI1

#define RM_OLED_ADC					ADC1
#define RM_OLED_ADC_GPIO		GPIOA
#define RM_OLED_ADC_PIN			GPIO_Pin_6
#define RM_OLED_ADC_CH			ADC_Channel_6

/******************库包含*******************/
//公共库
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
//系统库
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "includes.h"
#include "app_cfg.h"
#include "delay.h"
#include "bsp.h"
//RCS库
#include "RCS_Types.h"
#include "RCS_SPI.h"
#include "RCS_GPIO.h"
#include "RCS_OLED.h"
#include "RCS_OLED_ASCII.h"
#include "RCS_ADC.h"
#include "RCS_Exti.h"
#include "RCS_CAN.h"
#include "RCS_Usart.h"
#include "RCS_Timer.h"
#include "RCS_Pwm.h"
#include "RCS_GROBAL_GPS.h"
#include "RCS_MOTOR_3508.h"
#include "RCS_PIDctrl.h"
#include "RCS_Ctrl_BaseMove.h"
#include "RCS_M_BaseMove.h"
#include "RCS_Remote.h"
#include "RCS_Oled_IIC.h"
//USER库
#include "LED_LIB.h"
#include "MOTOR_CTRL_LIB.h"

#endif  //  _RCS_H_
