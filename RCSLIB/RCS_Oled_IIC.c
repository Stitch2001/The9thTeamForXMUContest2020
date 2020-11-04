/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       RCS_Oled_IIC.c
 * @brief      this file contains sd card basic operating function
 * @note       RM oled ��װ����  
 * @Version    V1.0.0
 * @Date       Jun-15-2020      
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */

#include "rcs.h"
#include "RM_OLED_ASCII.h"

/**
 * OLED flash Addr:
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
**/

static uint8_t OLED_GRAM[128][8];

/**
 * @brief   д�����ݸ�OLED
 * @param   dat: ����
 * @param   cmd: 0x00,command 0x01,data
 * @retval  
 */
void oled_write_byte(uint8_t dat, uint8_t cmd)
{
    if (cmd != 0)
        OLED_CMD_Set();
    else
        OLED_CMD_Clr();

		if(cmd)
		{
				uint8_t i = 8;
				OLED_CMD_Set();
				_DELAY;
				OLED_SCL_Clr();
				_DELAY;
				while (i--)
				{
						if (dat & 0x80)OLED_SDA_Set();
						else OLED_SDA_Clr();
						_DELAY;
						OLED_SCL_Set();
						_DELAY;
						OLED_SCL_Clr();
						_DELAY;
						dat <<= 1;
				}
				OLED_CMD_Set();
		}
		else
		{
				uint8_t i = 8;
				OLED_CMD_Clr();
				_DELAY;
				OLED_SCL_Clr();
				_DELAY;
				while (i--)
				{
						if (dat & 0x80)OLED_SDA_Set();
						else OLED_SDA_Clr();
						_DELAY;
						OLED_SCL_Set();
						_DELAY;
						OLED_SCL_Clr();
						_DELAY;
						dat <<= 1;
				}
				OLED_CMD_Set();
		}
}


/**
 * @brief   ����OLED��ǰҪ�����λ��
 * @param   x: X����
 * @param   y: Y����
 * @retval  
 */
static void oled_set_pos(uint8_t x, uint8_t y)
{
    x += 2;
    oled_write_byte((0xb0 + y), OLED_CMD);              //set page address y
    oled_write_byte(((x&0xf0)>>4)|0x10, OLED_CMD);      //set column high address
    oled_write_byte((x&0xf0), OLED_CMD);                //set column low address
}

/**
 * @brief   ��OLED��ʾ��
 * @retval  
 */
void oled_display_on(void)
{
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x14, OLED_CMD);
    oled_write_byte(0xaf, OLED_CMD);
}

/**
 * @brief   �ر�OLED��ʾ��
 * @retval  
 */
void oled_display_off(void)
{
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x10, OLED_CMD);
    oled_write_byte(0xae, OLED_CMD);
}

/**
 * @brief   ˢ��OLED��RAM(ˢ����ʾ��)
 * @retval  
 */
void oled_refresh_gram(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        oled_set_pos(0, i);

        for (n = 0; n < 128; n++)
        {
            oled_write_byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

/**
 * @brief   ȫ������
 * @param   pen: ����ΪPen_Clear����, Pen_Write����, Pen_Inversionȡ��ǰ�෴ @Pen_Typedef
 * @retval  
 */
void oled_clear(Pen_Typedef pen)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            if (pen == Pen_Write)
                OLED_GRAM[n][i] = 0xff;
            else if (pen == Pen_Clear)
                OLED_GRAM[n][i] = 0x00;
            else
                OLED_GRAM[n][i] = 0xff - OLED_GRAM[n][i];
        }
    }
}

/**
 * @brief   ��(x, y)�����ϴ��
 * @param   x: X����
 * @param   y: Y����
 * @param   pen: Pen_Clear�����ǰλ�õ�, 
						     Pen_Write���, 
								 Pen_Inversionȡ��ǰλ�õ��෴ @Pen_Typedef
 * @retval  None
 */
void oled_drawpoint(int8_t x, int8_t y, Pen_Typedef pen)
{
    uint8_t page = 0, row = 0;

    /* check the corrdinate */
    if ((x < 0) || (x > (X_WIDTH - 1)) || (y < 0) || (y > (Y_WIDTH - 1)))
        return;

    page = y / 8;
    row = y % 8;

    if (pen == Pen_Write)
        OLED_GRAM[x][page] |= 1 << row;
    else if (pen == Pen_Inversion)
        OLED_GRAM[x][page] ^= 1 << row;
    else
        OLED_GRAM[x][page] &= ~(1 << row);
}

/**
 * @brief   ��(x1, y1)��(x2, y2)����
 * @param   x1, y1: ����ʼ��λ��
 * @param   x2, y2: ���յ�λ��
 * @param   pen: Pen_Clear�����, 
							   Pen_Write����,
								 Pen_Inversionȡ��λ���෴ @Pen_Typedef
 * @retval  None
 */
void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_Typedef pen)
{
    uint8_t col = 0, row = 0;
    uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
    float k = 0.0f, b = 0.0f;

    if (y1 == y2)
    {
        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x1);

        for (col = x_st; col <= x_ed; col++)
        {
            oled_drawpoint(col, y1, pen);
        }
    }
    else if (x1 == x2)
    {
        (y1 <= y2) ? (y_st = y1):(y_st = y2);
        (y1 <= y2) ? (y_ed = y2):(y_ed = y1);

        for (row = y_st; row <= y_ed; row++)
        {
            oled_drawpoint(x1, row, pen);
        }
    }
    else
    {
        k = ((float)(y2 - y1)) / (x2 - x1);
        b = (float)y1 - k * x1;

        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x2);

        for (col = x_st; col <= x_ed; col++)
        {
            oled_drawpoint(col, (uint8_t)(col * k + b), pen);
        }
    }
}


//To add: rectangle, fillrectangle, circle, fillcircle, 

/**
 * @brief   չʾһ���ַ�
 * @param   row: �ַ���������
 * @param   col: �ַ���������
 * @param   chr: Ҫ��ʾ���ַ�
 * @retval  None
 */
void oled_showchar(uint8_t row, uint8_t col, uint8_t chr)
{
    uint8_t x = col * 6;
    uint8_t y = row * 12;
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr = chr - ' ';

    for (t = 0; t < 12; t++)
    {
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp&0x80)
                oled_drawpoint(x, y, Pen_Write);
            else
                oled_drawpoint(x, y, Pen_Clear);

            temp <<= 1;
            y++;
            if ((y - y0) == 12)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

//����m��n�η�
static uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)
        result *= m;

    return result;
}

/**
 * @brief   ��ʾһ������
 * @param   row: ������������
 * @param   col: ������������
 * @param   chr: Ҫ��ʾ������
 * @param   mode: 0x01, ��'0'���; 0x00,�ÿհ����;
 * @param   len: ���ֳ��ȣ�λ����
 * @retval  None
 */
void oled_shownum(uint8_t row, uint8_t col, uint32_t num, uint8_t mode, uint8_t len)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t -1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode == 0)
                    oled_showchar(row, col + t, ' ');
                else
                    oled_showchar(row, col + t, '0');
                continue;
            }
            else
                enshow = 1;
        }

        oled_showchar(row, col + t, temp + '0');
    }
}


/**
 * @brief   ��ʾһ���ַ���
 * @param   row: �ַ�����ʼ������
 * @param   col: �ַ�����ʼ������
 * @param   chr: �ַ�������ʼָ��
 * @retval  None
 */
void oled_showstring(uint8_t row, uint8_t col, uint8_t *chr)
{
    uint8_t n =0;

    while (chr[n] != '\0')
    {
        oled_showchar(row, col, chr[n]);
        col++;

        if (col > 20)
        {
            col = 0;
            row += 1;
        }
        n++;
    }
}

/**
 * @brief   ��ʽ����� 128*64
 * @param   row: ��ʽ������ַ�������ʼ��, 0 <= row <= 4;
 * @param   col: ��ʽ������ַ�������ʼ��, 0 <= col <= 20;
 * @param   *fmt: ��ʽ������ַ�������ʼָ��
 * @retval  None
 * @note    �ַ�������һ�г��ȵĲ��ֽ����ضϲ�����
 */
void oled_printf(uint8_t row, uint8_t col, const char *fmt,...)
{
    uint8_t LCD_BUF[128] = {0};
    uint8_t remain_size = 0;
    va_list ap;

    if ((row > 4) || (row < 1) || (col > 20) || (col < 1))
        return;

    va_start(ap, fmt);

    vsprintf((char *)LCD_BUF, fmt, ap);

    va_end(ap);

    remain_size = 21 - col;

    LCD_BUF[remain_size] = '\0';

    oled_showstring(row, col, LCD_BUF);
}


//��ʾlogo
void oled_LOGO(void)
{
	  uint8_t temp_char = 0;
    uint8_t x = 0, y = 0;
    uint8_t i = 0;
    oled_clear(Pen_Clear);

    for(; y < 64; y += 8)
    {
        for(x = 0; x < 128; x++)
        {
            temp_char = LOGO_BMP[x][y/8];
            for(i = 0; i < 8; i++)
            {
                if(temp_char & 0x80) oled_drawpoint(x, y + i,Pen_Write);
                else oled_drawpoint(x,y + i,Pen_Clear);
                temp_char <<= 1;
            }
        }
    }
    oled_refresh_gram();
}
/**
 * @brief   ��ȡRM OLED�İ���ֵ
 * @retval  ���ص��Ƿ�����ַ���mid left right up down
 */
void Read_Key(char *a)
{
	char dir[5];
	float voltage = 0;
	voltage = RCS_Get_Voltage(RM_OLED_ADC);
	if(voltage < RM_KEY_LEFT - VOLTAGE_ERROR)  strcpy(dir,"mid");
	else if(voltage < RM_KEY_RIGHT - VOLTAGE_ERROR)	strcpy(dir,"left");
	else if(voltage < RM_KEY_UP - VOLTAGE_ERROR)	strcpy(dir,"right");
	else if(voltage < RM_KEY_DOWN - VOLTAGE_ERROR)	strcpy(dir,"up");
	else strcpy(dir,"down");
	a = dir;
}

/**
 * @brief   ��ʼ��RM OLED
 * @retval  None
 */
void oled_init(void)
{
		RM_spi_Init(RM_OLED_SPI,
			 OLED_SPI_SCK_GPIO, OLED_SPI_SCK_PIN,
			 OLED_SPI_MOSI_GPIO, OLED_SPI_MOSI_PIN,
			 OLED_SPI_MISO_GPIO, OLED_SPI_MISO_PIN);					//SPIͨѶ��ʼ��
		//OLED��ģ�ⰴ����ʼ��
		RCS_ADC_Init(RM_OLED_ADC,RM_OLED_ADC_GPIO,RM_OLED_ADC_PIN,RM_OLED_ADC_CH);
		RCS_GPIO_Output_Init(OLED_DC_GPIO_Port, OLED_DC_Pin);
		RCS_GPIO_Output_Init(OLED_RST_GPIO_Port, OLED_RST_Pin);
	
    OLED_RST_Clr();
    delay_ms(500);
    OLED_RST_Set();

    oled_write_byte(0xae, OLED_CMD);    //turn off oled panel
    oled_write_byte(0x00, OLED_CMD);    //set low column address
    oled_write_byte(0x10, OLED_CMD);    //set high column address
    oled_write_byte(0x40, OLED_CMD);    //set start line address
    oled_write_byte(0x81, OLED_CMD);    //set contrast control resigter
    oled_write_byte(0xcf, OLED_CMD);    //set SEG output current brightness
    oled_write_byte(0xa1, OLED_CMD);    //set SEG/column mapping
    oled_write_byte(0xc8, OLED_CMD);    //set COM/row scan direction
    oled_write_byte(0xa6, OLED_CMD);    //set nomarl display
    oled_write_byte(0xa8, OLED_CMD);    //set multiplex display
    oled_write_byte(0x3f, OLED_CMD);    //1/64 duty
    oled_write_byte(0xd3, OLED_CMD);    //set display offset
    oled_write_byte(0x00, OLED_CMD);    //not offest
    oled_write_byte(0xd5, OLED_CMD);    //set display clock divide ratio/oscillator frequency
    oled_write_byte(0x80, OLED_CMD);    //set divide ratio 
    oled_write_byte(0xd9, OLED_CMD);    //set pre-charge period
    oled_write_byte(0xf1, OLED_CMD);    //pre-charge: 15 clocks, discharge: 1 clock
    oled_write_byte(0xda, OLED_CMD);    //set com pins hardware configuration 
    oled_write_byte(0x12, OLED_CMD);    //
    oled_write_byte(0xdb, OLED_CMD);    //set vcomh
    oled_write_byte(0x40, OLED_CMD);    //set vcom deselect level
    oled_write_byte(0x20, OLED_CMD);    //set page addressing mode
    oled_write_byte(0x02, OLED_CMD);    //
    oled_write_byte(0x8d, OLED_CMD);    //set charge pump enable/disable
    oled_write_byte(0x14, OLED_CMD);    //charge pump disable
    oled_write_byte(0xa4, OLED_CMD);    //disable entire dispaly on
    oled_write_byte(0xa6, OLED_CMD);    //disable inverse display on
    oled_write_byte(0xaf, OLED_CMD);    //turn on oled panel
    oled_write_byte(0xaf, OLED_CMD);    //display on

    oled_clear(Pen_Clear);
    oled_set_pos(0, 0);

}
