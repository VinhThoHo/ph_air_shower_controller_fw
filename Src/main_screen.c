/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "u8g2.h"
#include <stdlib.h>
#include "config.h"
#include "rtc.h"
#include "main_screen.h"
#include "system.h"
#include "sm5852.h"

#define LCD_WIDTH   128
#define LCD_HEIGHT  64

MAIN_SCREEN_T main_scr;
static void DateTime_Display(void);
static void Function_Frame(void);
/* screen init function */
void main_screen_init(void)
{
    u8g2_ClearBuffer(&u8g2);
    main_scr.tick = HAL_GetTick();
}

/* Lamp status display function */
void Lamp_Status(uint8_t lampStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2, 9, 33, "LAMP");
    if (lampStatus)
    {
        u8g2_DrawBitmap(&u8g2, 0, 33, bmp_neon_on.width / 8, bmp_neon_on.height, bmp_neon_on.data);
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 0, 33, bmp_neon_off.width / 8, bmp_neon_off.height, bmp_neon_off.data);
    }
}

/* Outdoor status display function */
void OutDoor_Status(uint8_t outdoorStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2,((14 - strlen("OutDoor"))/2)*6, 7, "OutDoor");
    u8g2_SetFont(&u8g2, u8g2_font_10x20_tr);
    if (outdoorStatus == 0)
    {
        u8g2_DrawStr(&u8g2,((8 - strlen("LOCK"))/2)*6, 26, "LOCK");
    }
    else
    {
        u8g2_DrawStr(&u8g2,((8 - strlen("OPEN"))/2)*6, 26, "OPEN");
    }
}

void Fan_Status(uint8_t status)
{
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2, 75, 33, "FAN");

    if(main_scr.fanRotate & status)
        u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_rotate.width/8, bmp_fan_rotate.height, bmp_fan_rotate.data);
    else
        u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_origin.width/8, bmp_fan_origin.height, bmp_fan_origin.data);
}

/* auto status display function */
void Auto_Status(uint8_t status)
{
    uint8_t buff[7];
    u8g2_SetFont(&u8g2, u8g2_font_micro_tr);
    u8g2_DrawStr(&u8g2, 105, 33, "AUTO");
    if(status)
    {
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_SetFont(&u8g2, u8g2_font_9x15_mr);
        // sprintf(buff, "%02d", (dev.autoTimeOff+59)/60);
        sprintf(buff, "%02d", dev.autoTimeOff);
        u8g2_DrawStr(&u8g2, 104, 54, buff);
        u8g2_SetDrawColor(&u8g2, 1);
        if(dev.autoTimeOff == 0) 
            dev.status.aut = 0;
    }
    else
    {
        u8g2_SetFont(&u8g2, u8g2_font_8x13_mr);
        u8g2_SetDrawColor(&u8g2, 0);
        u8g2_DrawStr(&u8g2, 102, 54, "OFF");
        u8g2_SetDrawColor(&u8g2, 1);
    }
}

/* UV Time used display function */
void UV_Time(uint32_t time)
{
    uint8_t buff[5];
    u8g2_DrawFrame(&u8g2, 0, 24, 41, 20);
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    u8g2_DrawStr(&u8g2, 3, 37, "UV:");
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
    u8g2_SetDrawColor(&u8g2, 0);
    sprintf(buff, "%04d", time);
    u8g2_DrawStr(&u8g2, 18, 37, buff);
    u8g2_SetDrawColor(&u8g2, 1);
}

/* Neon Time used display function */
void Neon_Time(uint32_t time)
{
    uint8_t buff[5];
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    u8g2_DrawFrame(&u8g2, 0, 44, 41, 20);
    u8g2_DrawStr(&u8g2, 3, 57, "NE:");
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
    u8g2_SetDrawColor(&u8g2, 0);
    sprintf(buff, "%04d", time);
    u8g2_DrawStr(&u8g2, 18, 57, buff);
    u8g2_SetDrawColor(&u8g2, 1);
}

/* void Filter_Pcent(float pressure, uint16_t threshold)
{
    uint8_t percent;
    uint8_t buff[5];
    uint8_t warnBuff[30];
    uint8_t line;
    if (pressure > threshold)
        pressure = threshold;
    percent = (uint8_t)((pressure * 100) / ((float)threshold));
    percent = fabs(100 - percent);
    line = percent * 50 / 100;
    u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
    if ((percent <= 100) && (percent > 70))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter is good!");
    else if ((percent <= 70) && (percent > 30))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter medium!");
    else if ((percent <= 30) && (percent > 10))
        u8g2_DrawStr(&u8g2, 17, 21, "The filter low!");
    else if (percent <= 10)
    {
        u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
        if (main_scr.warnCnt > F_WARNING)
        {
            u8g2_DrawStr(&u8g2, 17, 21, "Please change filter!");
        }
        else
        {
            u8g2_DrawStr(&u8g2, 17, 21, "                     ");
        }
    }
    u8g2_DrawStr(&u8g2, 7, 10, "Filter:");
    u8g2_DrawFrame(&u8g2, 43, 2, 54, 10);
    sprintf(buff, "%d %%", percent);
    u8g2_DrawStr(&u8g2, 98, 10, buff);
    u8g2_DrawBox(&u8g2, 45, 4, line, 6);
} */

/* Draw Frame function */
/* static void Function_Frame(void)
{
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawFrame(&u8g2, 0, 24, 128, 40);
    u8g2_DrawHLine(&u8g2, 0, 36, 128);
    u8g2_DrawHVLine(&u8g2, 32, 24, 40, 1);
    u8g2_DrawHVLine(&u8g2, 64, 24, 40, 1);
    u8g2_DrawHVLine(&u8g2, 96, 24, 40, 1);
} */
static void Function_Frame(void)
{
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawHLine(&u8g2, 0, LCD_HEIGHT/2 - 1, LCD_WIDTH);
    u8g2_DrawVLine(&u8g2, LCD_WIDTH/2 + 1, 0, LCD_HEIGHT);
}
/* Display Date time function */
static void DateTime_Display(void)
{
    uint8_t buff[11];
    if(localTime.sec % 2)
        sprintf(buff, " %02d:%02d \n", localTime.hour, localTime.min);
    else
        sprintf(buff, " %02d %02d \n", localTime.hour, localTime.min);
    u8g2_SetFont(&u8g2, u8g2_font_10x20_mr);
    u8g2_SetDrawColor(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 0, 16, buff);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_5x8_tf);
    sprintf(buff, "%02d/%02d/%04d\n", localTime.mday, localTime.month, localTime.year);
    u8g2_DrawStr(&u8g2, 74, 14, buff);
}

/* Management devices|: Neon, UV, Fan, Socket, filter status */
void Main_Screen_Manage(void)
{
    if (HAL_GetTick() - main_scr.tick > 200)
    {
        main_scr.fanRotate = ~main_scr.fanRotate;
        if(++main_scr.warnCnt > F_WARNING*2) main_scr.warnCnt = 0;
        main_scr.tick = HAL_GetTick();
    }
    u8g2_ClearBuffer(&u8g2);
    //DateTime_Display();
    //UV_Time(sys.uvTime/3600);
    //Neon_Time(sys.fanTime/3600);
    Lamp_Status(dev.status.lamp);
    OutDoor_Status(dev.status.outdoor);
    Fan_Status(dev.status.fan);
    Auto_Status(dev.status.aut);
    Function_Frame();
    // Filter_Pcent(sm5852_1.pressure, sys_cfg.pressureVal);
}
