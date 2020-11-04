//@filename: RCS_OLED.c
//@date: 2019-07-21
//@author: 闫锐
//@brief: 6针OLED显示屏
#include "RCS_OLED.h"

#define OLED_DC_L                GPIO_ResetBits(oled_gpio,oled_dc_pin)
#define OLED_DC_H                GPIO_SetBits(oled_gpio,oled_dc_pin)
#define OLED_RST_L               GPIO_ResetBits(oled_gpio,oled_rst_pin)
#define OLED_RST_H               GPIO_SetBits(oled_gpio,oled_rst_pin)
#define OLED_SDA_L               GPIO_ResetBits(oled_gpio,oled_sda_pin)
#define OLED_SDA_H               GPIO_SetBits(oled_gpio,oled_sda_pin)
#define OLED_SCL_L               GPIO_ResetBits(oled_gpio,oled_scl_pin)
#define OLED_SCL_H               GPIO_SetBits(oled_gpio,oled_scl_pin)

GPIO_TypeDef* oled_gpio = NULL;		
uint32_t oled_dc_pin;
uint32_t oled_rst_pin;
uint32_t oled_sda_pin;
uint32_t oled_scl_pin;


//@name: RCS_OLED_Init
//@brief: 6针oled显示屏初始化
//@param:GPIO_TypeDef *GPIOx 	GPIO组
//@param:uint32_t   GPIO_Pin_DC DC管脚号
//@param:uint32_t   GPIO_Pin_RST RST管脚号
//@param:uint32_t   GPIO_Pin_SDA SDA管脚号
//@param:uint32_t   GPIO_Pin_SCL SCL管脚号
void RCS_OLED_Init(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_DC, uint32_t GPIO_Pin_RST,
                  uint32_t GPIO_Pin_SDA, uint32_t GPIO_Pin_SCL)
{
    oled_gpio = GPIOx;
    oled_dc_pin = GPIO_Pin_DC;
    oled_rst_pin = GPIO_Pin_RST;
    oled_sda_pin = GPIO_Pin_SDA;
    oled_scl_pin = GPIO_Pin_SCL;

    RCS_GPIO_Output_Init(GPIOx, GPIO_Pin_DC);
    RCS_GPIO_Output_Init(GPIOx, GPIO_Pin_RST);
    RCS_GPIO_Output_Init(GPIOx, GPIO_Pin_SDA);
    RCS_GPIO_Output_Init(GPIOx, GPIO_Pin_SCL);

    OLED_SCL_H;
    OLED_RST_L;
		delay_ms(100);
    OLED_RST_H;

    RCS_OLED_WR_CMD(0xae);//关闭oled显示
    RCS_OLED_WR_CMD(0x00);//设置低位光标col位置
    RCS_OLED_WR_CMD(0x10);//设置高位光标col位置
    RCS_OLED_WR_CMD(0x40);//设置开始行位置(0->63)=>(0x40->7f)
    RCS_OLED_WR_CMD(0x81);//设置对比度
    RCS_OLED_WR_CMD(0xcf);//设置对比度为207
    RCS_OLED_WR_CMD(0xa1);//Set Segment Re-map(自己测试)
    RCS_OLED_WR_CMD(0xc8);//Set COM/Row Scan Direction  (自己测试)
    RCS_OLED_WR_CMD(0xa6);//设置正常显示模式 0xa7为反显
    RCS_OLED_WR_CMD(0xa8);//--set multiplex ratio(1 to 64)
    RCS_OLED_WR_CMD(0x3f);//--1/64 duty
    RCS_OLED_WR_CMD(0xd3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    RCS_OLED_WR_CMD(0x00);//-not offset
    RCS_OLED_WR_CMD(0xd5);//设置显示时钟(振荡频率,divide比例)
    RCS_OLED_WR_CMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    RCS_OLED_WR_CMD(0xd9);//--set pre-charge period
    RCS_OLED_WR_CMD(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    RCS_OLED_WR_CMD(0xda);//--set com pins hardware configuration
    RCS_OLED_WR_CMD(0x12);
    RCS_OLED_WR_CMD(0xdb);//--set vcomh
    RCS_OLED_WR_CMD(0x40);//Set VCOM Deselect Level
    RCS_OLED_WR_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    RCS_OLED_WR_CMD(0x02);
    RCS_OLED_WR_CMD(0x8d);//--set Charge Pump enable/disable
    RCS_OLED_WR_CMD(0x14);//--set(0x10) disable
    RCS_OLED_WR_CMD(0xa4);// Disable Entire Display On (0xa4/0xa5)
    RCS_OLED_WR_CMD(0xa6);// Disable Inverse Display On (0xa6/0xa7)
    RCS_OLED_WR_CMD(0xaf);//--turn on oled panel
    RCS_OLED_FULL(0x00);
    RCS_OLED_Set_Pos(0, 0);
}
//开启显示屏
void RCS_OLED_DISPLAY_ON(void)
{
    RCS_OLED_WR_CMD(0X8d);
    RCS_OLED_WR_CMD(0X14);
    RCS_OLED_WR_CMD(0Xaf);
}
//关闭显示屏
void RCS_OLED_DISPLAY_OFF(void)
{
    RCS_OLED_WR_CMD(0X8d);
    RCS_OLED_WR_CMD(0X10);
    RCS_OLED_WR_CMD(0Xae);
}
//@name: RCS_OLED_Set_Pos
//@brief: 在显示屏上打点
//@param:uint8_t   x 点所在的列数
//@param:uint8_t   y 点所在的行数
void RCS_OLED_Set_Pos(uint8_t x, uint8_t y)
{
    RCS_OLED_WR_CMD(0xb0 + y);
    RCS_OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10);
    RCS_OLED_WR_CMD((x & 0x0f) | 0x01);
}
//@name: RCS_OLED_WR_6X8STR
//@brief: 在显示屏上写字体大小为6x8字符串
//@param:uint8_t   x 字符串起始字符所在的列数
//@param:uint8_t   y 字符串起始字符所在的行数
//@param:uint8_t   *str 字符串指针
void RCS_OLED_WR_6X8STR(uint8_t x, uint8_t y, uint8_t *str)
{
		uint8_t i = 0;
		uint8_t j =0;
		for(i=0; str[i] !='\0' ; i++)
    {
        if (x > 122)
        {
            x = 0;
            y++;
        }
        RCS_OLED_Set_Pos(x, y);
        for (j = 0; j < 6 ; j++)
            RCS_OLED_WR_DATA(F6x8[str[i] - 32][j]);
        x += 6;
    }
}
//@name: RCS_OLED_WR_6X8STR
//@brief: 在显示屏上写字体大小为8x16字符串
//@param:uint8_t   x 字符串起始字符所在的列数
//@param:uint8_t   y 字符串起始字符所在的行数
//@param:uint8_t   *str 字符串指针
void RCS_OLED_WR_8X16STR(uint8_t x, uint8_t y, uint8_t *str)//x是列,y是行,一个字符占16行8列
{
    uint8_t ch;
		uint8_t i;
		uint8_t j;
		uint8_t k;
    for (i = 0; str[i] != '\0'; i++)
    {
        ch = str[i] - 32;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        RCS_OLED_Set_Pos(x, y);
        for (j = 0; j < 8; j++)
            RCS_OLED_WR_DATA(F8X16[ch * 16 + j]);
        RCS_OLED_Set_Pos(x, y + 1);
        for (k = 0; k < 8; k++)
            RCS_OLED_WR_DATA(F8X16[ch * 16 + k + 8]);
        x += 8;
    }
}
//写入数据
void RCS_OLED_WR_DATA(uint8_t data)
{
    uint8_t i = 8;
    OLED_DC_H;
    _DELAY;
    OLED_SCL_L;
    _DELAY;
    while (i--)
    {
        if (data & 0x80)OLED_SDA_H;
        else OLED_SDA_L;
        _DELAY;
        OLED_SCL_H;
        _DELAY;
        OLED_SCL_L;
        _DELAY;
        data <<= 1;
    }
    OLED_DC_H;
}

//发送命令
void RCS_OLED_WR_CMD(uint8_t cmd)
{
    uint8_t i = 8;
    OLED_DC_L;
    _DELAY;
    OLED_SCL_L;
    _DELAY;
    while (i--)
    {
        if (cmd & 0x80)OLED_SDA_H;
        else OLED_SDA_L;
        _DELAY;
        OLED_SCL_H;
        _DELAY;
        OLED_SCL_L;
        _DELAY;
        cmd <<= 1;
    }
    OLED_DC_H;
}

//满屏
void RCS_OLED_FULL(uint8_t ch)
{
    uint8_t i, j;
    for (i = 0; i < 8; i++)
    {
        RCS_OLED_Set_Pos(0, i);
        for (j = 0; j < 128; j++)
            RCS_OLED_WR_DATA(ch);
    }
}

//向左滚动
void RCS_OLED_LEFT_SROLL(void)
{
    RCS_OLED_WR_CMD(0x27);
    RCS_OLED_WR_CMD(0x00);
    RCS_OLED_WR_CMD(0x00);
    RCS_OLED_WR_CMD(0x00);
    RCS_OLED_WR_CMD(0x01);
    RCS_OLED_WR_CMD(0x00);
    RCS_OLED_WR_CMD(0xff);
    RCS_OLED_WR_CMD(0x2f);   //0x2f 水平滚动开始,0x2e 水平滚动停止
}

//向右滚动
void RCS_OLED_RIGHT_SROLL(void)
{
    RCS_OLED_WR_CMD(0x26);
    RCS_OLED_WR_CMD(0x00);
    RCS_OLED_WR_CMD(0x00);
    RCS_OLED_WR_CMD(0x04);
    RCS_OLED_WR_CMD(0x01);
    RCS_OLED_WR_CMD(0x00);
    RCS_OLED_WR_CMD(0xff);
    RCS_OLED_WR_CMD(0x2f);
}

//上下滚动
void RCS_OLED_UP_DOWN_SROLL(void)
{
    RCS_OLED_WR_CMD(0x29);   //[29h]: Vertical and Right Horizontal Scroll;[2Ah]: Vertical and Left Horizontal Scroll
    RCS_OLED_WR_CMD(0x00);   //Dummy byte(Set as 00h)
    RCS_OLED_WR_CMD(0x00);   //Define start page address(0~7)
    RCS_OLED_WR_CMD(0x01);   //Set time interval between each scroll step in terms of  frame frequency
    RCS_OLED_WR_CMD(0x01);   //Define end page address(0~7),The value of start page address must be larger or equal to end page address
    RCS_OLED_WR_CMD(0x00);   //Vertical scrolling offset(0~63)
    RCS_OLED_WR_CMD(0x2f);   //Activate scroll
}

//@name: RCS_OLED_PutPixel
//@brief: 放置像素
//@param:uint8_t   x 像素所在的列数
//@param:uint8_t   y 像素所在的行数
void RCS_OLED_PutPixel(uint8_t x, uint8_t y)
{
    uint8_t data1;  //data1当前点的数据

    RCS_OLED_Set_Pos(x, y);
    data1 = 0x01 << (y % 8);
    RCS_OLED_WR_CMD(0xb0 + (y >> 3));
    RCS_OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10);
    RCS_OLED_WR_CMD((x & 0x0f) | 0x00);
    RCS_OLED_WR_DATA(data1);
}
