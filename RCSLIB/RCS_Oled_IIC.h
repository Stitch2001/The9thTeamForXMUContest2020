/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       RCS_Oled_IIC.h
 * @brief      this file contains sd card basic operating function
 * @note         
 * @Version    V1.0.0
 * @Date       JUN-15-2020      
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */

#ifndef __RCS_OLED_IIC__H
#define __RCS_OLED_IIC__H

#include "rcs.h"


#define Max_Column      128
#define Max_Row         64

#define X_WIDTH         128
#define Y_WIDTH         64

#define RM_KEY_LEFT			0.68
#define RM_KEY_RIGHT		1.39
#define RM_KEY_UP				1.98
#define RM_KEY_DOWN			2.64
#define VOLTAGE_ERROR		0.1

#define OLED_CMD        0x00
#define OLED_DATA       0x01

#define CHAR_SIZE_WIDTH     6
#define VHAR_SIZE_HIGHT     12

#define OLED_CMD_Set()      RCS_GPIO_Set(OLED_DC_GPIO_Port, OLED_DC_Pin)
#define OLED_CMD_Clr()      RCS_GPIO_Reset(OLED_DC_GPIO_Port, OLED_DC_Pin)

#define OLED_RST_Set()      RCS_GPIO_Set(OLED_RST_GPIO_Port, OLED_RST_Pin)
#define OLED_RST_Clr()      RCS_GPIO_Reset(OLED_RST_GPIO_Port, OLED_RST_Pin)

#define OLED_SDA_Clr()      GPIO_ResetBits(OLED_SPI_MOSI_GPIO,OLED_SPI_MOSI_PIN)
#define OLED_SDA_Set()      GPIO_SetBits(OLED_SPI_MOSI_GPIO,OLED_SPI_MOSI_PIN)

#define OLED_SCL_Clr()      GPIO_ResetBits(OLED_SPI_SCK_GPIO,OLED_SPI_SCK_PIN)
#define OLED_SCL_Set()      GPIO_SetBits(OLED_SPI_SCK_GPIO,OLED_SPI_SCK_PIN)

typedef enum
{
    Pen_Clear = 0x00,
    Pen_Write = 0x01,
    Pen_Inversion = 0x02,
}Pen_Typedef;

/* function define */
void oled_init(void);			//初始化RM OLED
void Read_Key(char *a);   //获取OLED的按键值
void oled_write_byte(uint8_t dat, uint8_t cmd);
void oled_display_on(void);	//开屏
void oled_display_off(void);//闭屏
void oled_refresh_gram(void);//刷新屏幕（每次写入都要刷新）
void oled_clear(Pen_Typedef pen);	//清屏
void oled_drawpoint(int8_t x, int8_t y, Pen_Typedef pen);
void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_Typedef pen);
void oled_showchar(uint8_t row, uint8_t col, uint8_t chr);
void oled_shownum(uint8_t row, uint8_t col, uint32_t num, uint8_t mode, uint8_t len);
void oled_showstring(uint8_t row, uint8_t col, uint8_t *chr);
void oled_printf(uint8_t row, uint8_t col, const char *fmt,...);
void oled_LOGO(void);

#endif
