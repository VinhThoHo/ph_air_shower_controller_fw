/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#include "info_screen.h"
#include "u8g2.h"
#include "bitmaps.h"
#include "sm5852.h"
#include "system.h"
#include "string.h"

#define LCD_WIDTH   128
#define LCD_HEIGHT  64

INFO_SCREEN_STR Info_Scr;

void Info_Screen_Init(void)
{   
    Info_Scr.dot = 0;
    Info_Scr.tick = HAL_GetTick();
}

/* Display Date time function */
static void DateTime_Display(void)
{
    uint8_t buff[11];
    if(localTime.sec % 2)
        sprintf(buff, " %02d:%02d \n", localTime.hour, localTime.min);
    else
        sprintf(buff, " %02d %02d \n", localTime.hour, localTime.min);
    u8g2_SetFont(&u8g2, u8g2_font_9x18B_mr);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 1, 14, buff);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_5x8_tf);
    sprintf(buff, "%02d/%02d/%04d\n", localTime.mday, localTime.month, localTime.year);
    u8g2_DrawStr(&u8g2, 7, 27, buff);
}

static void Function_Frame(void)
{
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawHLine(&u8g2, 0, LCD_HEIGHT/2 - 1, LCD_WIDTH);
    u8g2_DrawVLine(&u8g2, LCD_WIDTH/2 + 1, 0, LCD_HEIGHT);
}

/* UV Time used display function */
static void UV_Time(uint32_t time)
{
    uint8_t buff[5];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    u8g2_DrawStr(&u8g2, 2, 40, "UV :");
    u8g2_SetDrawColor(&u8g2, 0);
    sprintf(buff, "%04d", time);
    u8g2_DrawStr(&u8g2, 25, 40, buff);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 55, 40, "h");
}

/* Filter Time used display function */
static void Filter_Time(uint32_t time)
{
    uint8_t buff[5];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    
    u8g2_DrawStr(&u8g2, 2, 50, "FIL:");
    u8g2_SetDrawColor(&u8g2, 0);
    sprintf(buff, "%04d", time);
    u8g2_DrawStr(&u8g2, 25, 50, buff);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 55, 50, "h");
}

/* System Time used display function */
static void Sys_Time(uint32_t time)
{
    uint8_t buff[5];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    u8g2_DrawStr(&u8g2, 2, 60, "SYS:");
    u8g2_SetDrawColor(&u8g2, 0);
    sprintf(buff, "%05d", time);
    u8g2_DrawStr(&u8g2, 25, 60, buff);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawStr(&u8g2, 55, 60, "h");
}


static void Filter_Param(float pressure, float flow)
{
    uint8_t buff[12];
    if(pressure > 999.9) pressure = 999.9;
    if(flow > 5.0) flow = 5.0;
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + (LCD_WIDTH/2 - strlen(" Sensor ")*5)/2, 7, " Sensor ");
    u8g2_SetDrawColor(&u8g2, 1);

    u8g2_DrawHLine(&u8g2, (LCD_WIDTH/2) + (LCD_WIDTH/16), 10, (LCD_WIDTH/2) - (LCD_WIDTH/8));

    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + 5, 19, "P:");
    sprintf(buff, "%03.01f Pa", pressure);
    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + 20, 19, buff);

    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + 5, 28, "F:");
    sprintf(buff, "%2.02f m/s", flow);
    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + 20, 28, buff);
}

/* Door Status */
void Door(uint8_t level)
{
    u8g2_SetFont(&u8g2, u8g2_font_4x6_mf);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + (LCD_WIDTH/2 - strlen(" Door status ")*4)/2, LCD_HEIGHT/2 + 6, " Door status ");
    u8g2_SetDrawColor(&u8g2, 1);
    //u8g2_DrawHLine(&u8g2, LCD_WIDTH/2 + (LCD_WIDTH/16), LCD_HEIGHT/2 + 8, (LCD_WIDTH/2) - (LCD_WIDTH/8));
    //u8g2_DrawVLine(&u8g2, LCD_WIDTH/2 + LCD_WIDTH/4, LCD_HEIGHT/2 + 10, 18);
    u8g2_DrawFrame(&u8g2, LCD_WIDTH/2 + 6, LCD_HEIGHT/2 + 10, 20, 20);
    u8g2_DrawBox(&u8g2, LCD_WIDTH/2 + 10, LCD_HEIGHT/2 + 12, 12, 4);
    u8g2_DrawBox(&u8g2, LCD_WIDTH/2 + 10, LCD_HEIGHT/2 + 18, 12, 4); 
    u8g2_DrawBox(&u8g2, LCD_WIDTH/2 + 10, LCD_HEIGHT/2 + 24, 12, 4); 
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + LCD_WIDTH/4, LCD_HEIGHT/2 + 22, "CLOSE");
    switch (level)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
}

void Temperature(float temp) 
{
    uint8_t buff[10];

    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, LCD_WIDTH/2 + (LCD_WIDTH/2 - strlen("SensorTemp")*5)/2 + 2, (LCD_HEIGHT/2) + 7, "SensorTemp");
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawHLine(&u8g2, LCD_WIDTH/2 + (LCD_WIDTH/16), LCD_HEIGHT/2 + 10, (LCD_WIDTH/2) - (LCD_WIDTH/8));
    u8g2_DrawBitmap(&u8g2, (LCD_WIDTH/2) + 2, (LCD_HEIGHT/2) + 12, bmp_temp.width / 8, bmp_temp.height, bmp_temp.data);
    u8g2_SetFont(&u8g2, u8g2_font_7x13B_mf);
    sprintf(buff, "%0.2f", temp);
    u8g2_DrawStr(&u8g2, (LCD_WIDTH/2) + 16, 56, buff);
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 115, 60, 0x00b0);
    u8g2_DrawGlyph(&u8g2, 120, 56, 0x0043);
}

void Info_Screen_Manage(void)
{
    Function_Frame();
    DateTime_Display();
    UV_Time(sys.uvTime/3600);
    Filter_Time(sys.filterTime/3600);
    Sys_Time(sys.sysTime/3600);
    Filter_Param(sm5852_1.pressure, sm5852_1.flow);
    Temperature(sm5852_1.temp);
    // Door(1);
}
