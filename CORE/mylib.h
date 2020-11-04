#ifndef  _MYLIB_H_
#define  _MYLIB_H_

/********************ѡ��*******************/



/******************��������*****************/

#define pi					3.14159265357					//��
#define DEG2RAD			1.74532925199433e-2f	//�Ƕȱ任�ɻ���     
#define RAD2DEG			5.72957795130823e1f		//���ȱ任�ɽǶ�


/******************�жϺ�������***************/
//�ⲿ�ж϶���
#define	KEY_WKUP_IQR				EXTI0_IRQHandler
#define KEY_WKUP_LINE				EXTI_Line0
#define KEY1_IQR 						EXTI3_IRQHandler
#define KEY1_LINE						EXTI_Line3
#define KEY0_IQR						EXTI4_IRQHandler
#define KEY0_LINE						EXTI_Line4



/***************�ж����ȼ�����**************/
#define KEY0_EXTI_PRI				0x11
#define KEY1_EXTI_PRI				0x22
#define KEY_WKUP_EXTI_PRI 	0x00


/******************�ܽŶ���*****************/

//�����ܽ�
#define KEY0_GPIO 				GPIOE
#define KEY0_PIN				  GPIO_Pin_4

#define KEY1_GPIO 				GPIOE
#define KEY1_PIN					GPIO_Pin_3

#define KEY_WKUP_GPIO 		GPIOA
#define KEY_WKUP_PIN	    GPIO_Pin_0
//LED�ܽ�
#define LED0_GPIO					GPIOF
#define	LED0_PIN					GPIO_Pin_9

#define LED1_GPIO					GPIOF
#define	LED1_PIN					GPIO_Pin_10
/******************�����*******************/

#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "usart.h"
#include "delay.h"
#include "bsp.h"
//MYLIB
#include "My_Types.h"
#include "My_GPIO.h"
#include "My_OLED_ASCII.h"
#include "My_OLED.h"
#include "My_ADC.h"
#include "My_Exti.h"
#include "RM_spi.h"
//GAMELIB
#include "led.h"
#include "key.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#endif //_MYLIB_H_
