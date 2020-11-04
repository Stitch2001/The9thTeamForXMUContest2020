//@filename: RCS_OLED.h
//@date: 2019-07-21
//@author: „∆»Ò
//@brief: 6’ÎOLEDœ‘ æ∆¡
#ifndef  _RCS_OLED_H_
#define  _RCS_OLED_H_

#include "rcs.h"

#define Max_Column  128
#define Max_Row     64

#define _DELAY ;

void RCS_OLED_Init(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_DC, uint32_t GPIO_Pin_RST,
                  uint32_t GPIO_Pin_SDA, uint32_t GPIO_Pin_SCL); //≥ı ºªØ
void RCS_OLED_DISPLAY_ON(void);
void RCS_OLED_DISPLAY_OFF(void);
void RCS_OLED_Set_Pos(uint8_t x, uint8_t y);//¥Úµ„
void RCS_OLED_WR_6X8STR(uint8_t x, uint8_t y, uint8_t *str);	//œ‘ æ◊÷∑˚¥Æ
void RCS_OLED_WR_8X16STR(uint8_t x, uint8_t y, uint8_t *str);	//œ‘ æ◊÷∑˚¥Æ
void RCS_OLED_WR_DATA(uint8_t data);
void RCS_OLED_WR_CMD(uint8_t cmd);
void RCS_OLED_FULL(uint8_t ch);
void RCS_OLED_LEFT_SROLL(void);
void RCS_OLED_RIGHT_SROLL(void);
void RCS_OLED_UP_DOWN_SROLL(void);
void RCS_OLED_PutPixel(uint8_t x, uint8_t y);

#endif
