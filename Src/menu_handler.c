/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "menu.h"
#include "menu_handler.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "rtc.h"
#include "config.h"
#include "lcd_bl.h"
#include "system.h"

uint8_t buff[90];

DATE_TIME menuRtc;
uint8_t dozens, units, hundreds, thousands;

uint16_t normalEvt(uint8_t add, uint16_t key)
{
  return 0;
}

uint16_t Dis1Evt(uint8_t add, uint16_t key)
{
  menuRtc = localTime;
  return 0;
}

uint16_t Dis2Evt(uint8_t add, uint16_t key)
{
  return 0;
}

uint16_t Dis3Evt(uint8_t add, uint16_t key)
{
  return 0;
}

uint16_t Dis4Evt(uint8_t add, uint16_t key)
{
  return 0;
}

/* set RTC time function */
uint16_t SetTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuCursor = 0;
  }
  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(menuRtc.hour < 23)menuRtc.hour++;
        else menuRtc.hour = 0;
        break;
      case 1:
        if(menuRtc.min < 59)menuRtc.min++;
        else menuRtc.min = 0;
        break;
      case 2:
        if(menuRtc.sec < 59)menuRtc.sec++;
        else menuRtc.sec = 0;
        break;
      case 3:
        if(menuRtc.mday < 31)menuRtc.mday++;
        else menuRtc.mday = 0;
        break;
      case 4:
        if(menuRtc.month < 12)menuRtc.month++;
        else menuRtc.month = 0;
        break;
      case 5:
        if(menuRtc.year < 2099)menuRtc.year++;
        else menuRtc.year = 2019;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(menuRtc.hour != 0)menuRtc.hour--;
        else menuRtc.hour = 23;
        break;
      case 1:
        if(menuRtc.min != 0)menuRtc.min--;
        else menuRtc.min = 59;
        break;
      case 2:
        if(menuRtc.sec != 0)menuRtc.sec--;
        else menuRtc.sec = 59;
        break;
      case 3:
        if(menuRtc.mday > 1)menuRtc.mday--;
        else menuRtc.mday = 31;
        break;
      case 4:
        if(menuRtc.month > 1)menuRtc.month--;
        else menuRtc.month = 12;
        break;
      case 5:
        if(menuRtc.year > 2019)menuRtc.year--;
        else menuRtc.year = 2099;
        break;
    }
  }
  if(key == BTN_MENU)
  {
    if(menuCursor < 5) menuCursor++;
    else
    {
      localTime = menuRtc;
      localTime.hour = localTime.hour - 7;
      RTC_Sync(&localTime, +7);
      u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
    }
  }

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 5, 36, 0x23f1);
  switch(menuCursor)
  {
    case 0:
      /* blink hours */
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.hour);
      u8g2_DrawStr(&u8g2, 36, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, ":%02d:%02d", menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36 + 12, 36, buff);
      sprintf((char*)buff,"%02d/%02d/%04d", menuRtc.mday, menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30, 48, buff);

      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:", menuRtc.hour);
      u8g2_DrawStr(&u8g2, 36, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.min);
      u8g2_DrawStr(&u8g2, 36 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, ":%02d", menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36 + 30, 36, buff);

      sprintf((char*)buff,"%02d/%02d/%04d", menuRtc.mday, menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30, 48, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:", menuRtc.hour, menuRtc.min);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36 + 36, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%02d/%02d/%04d", menuRtc.mday, menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30, 48, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:%02d", menuRtc.hour, menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.mday);
      u8g2_DrawStr(&u8g2, 30, 48, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "/%02d/%04d", menuRtc.month, menuRtc.year);
      u8g2_DrawStr(&u8g2, 30 + 12, 48, buff);
      break;
    case 4:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:%02d", menuRtc.hour, menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      sprintf((char*)buff, "%02d/", menuRtc.mday);
      u8g2_DrawStr(&u8g2, 30, 48, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%02d", menuRtc.month);
      u8g2_DrawStr(&u8g2, 30+18, 48, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "/%04d", menuRtc.year);
      u8g2_DrawStr(&u8g2, 30 + 30, 48, buff);
      break;
    case 5:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%02d:%02d:%02d", menuRtc.hour, menuRtc.min, menuRtc.sec);
      u8g2_DrawStr(&u8g2, 36, 36, buff);

      sprintf((char*)buff, "%02d/%02d/", menuRtc.mday, menuRtc.month);
      u8g2_DrawStr(&u8g2, 30, 48, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%04d", menuRtc.year);
      u8g2_DrawStr(&u8g2, 30 + 36, 48, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  return treeView[add].keyMode;
}

uint16_t SetPressureEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = sys_cfg.pressureVal;
    menuCursor = 0;
    units = menuValueSet%10;
    dozens = (menuValueSet/10)%10;
    hundreds = menuValueSet/100;
  }
  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(hundreds < 9) hundreds++;
        else hundreds = 0;
        break;
      case 1:
        if(dozens < 9) dozens++;
        else dozens = 0;
        break;
      case 2:
        if(units < 9) units++;
        else units = 0;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(hundreds > 0) hundreds--;
        else hundreds = 9;
        break;
      case 1:
        if(dozens > 0) dozens--;
        else dozens = 9;
        break;
      case 2:
        if(units > 0) units--;
        else units = 9;
        break;
    }
  }
  if(key == BTN_MENU)
  {
    if(menuCursor < 2) menuCursor++;
    else
    {
      menuValueSet = hundreds * 100 + dozens * 10 + units;
      sys_cfg.pressureVal = menuValueSet;
      CFG_Save(); // Save config
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
    }
  }
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_10x20_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char *)buff, "%d", hundreds);
      u8g2_DrawStr(&u8g2, 36 + 10, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char *)buff, "%d%d", dozens, units);
      u8g2_DrawStr(&u8g2, 36 + 20, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_10x20_mf);
      sprintf((char *)buff, "%d", hundreds);
      u8g2_DrawStr(&u8g2, 36 + 10, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char *)buff, "%d", dozens);
      u8g2_DrawStr(&u8g2, 36 + 20, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char *)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 36 + 30, 36, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_10x20_mf);
      sprintf((char *)buff, "%d%d", hundreds, dozens);
      u8g2_DrawStr(&u8g2, 36 + 10, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char *)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 36 + 30, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 25, 36, 0x23f7);

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 80, 36, 0x23f6);

  return treeView[add].keyMode;
}

/* set Auto counter time function */
uint16_t SetAutoTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = sys_cfg.autoCnt;
    menuCursor = 0;
    dozens = menuValueSet/10;
    units = menuValueSet%10;
  }
  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(dozens < 9) dozens++;
        else dozens = 0;
        break;
      case 1:
        if(units < 9) units++;
        else units = 0;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(dozens > 0) dozens--;
        else dozens = 9;
        break;
      case 1:
        if(units > 0) units--;
        else units = 9;
        break;
    }
  }
  if(key == BTN_MENU)
  {
    if(menuCursor < 1) menuCursor++;
    else
    {
      menuValueSet = dozens * 10 + units;
      sys_cfg.autoCnt = menuValueSet;
      CFG_Save(); // Save config
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
    }
  }
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_10x20_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char *)buff, "%d", dozens);
      u8g2_DrawStr(&u8g2, 40 + 10, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      sprintf((char *)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 40 + 22, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_10x20_mf);
      sprintf((char *)buff, "%d", dozens);
      u8g2_DrawStr(&u8g2, 40 + 10, 36, buff);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char *)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 40 + 22, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 25, 36, 0x23f7);

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 80, 36, 0x23f6);
  
  return treeView[add].keyMode;
}

uint16_t SetFanSpeedEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
     menuValueSet = sys_cfg.fanSpeed;
  if(key == BTN_DOWN)
  {
    if(menuValueSet < 3)
      menuValueSet++;
    else
      menuValueSet = 0;
  }
  if(key == BTN_UP)
  {
    if(menuValueSet > 0)
      menuValueSet--;
    else
      menuValueSet = 2;
  }

  if(menuValueSet == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 26, 0x2713);
  }
  if(menuValueSet == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 36, 0x2713);
  }
  if(menuValueSet == 2)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 46, 0x2713);
  }
  if(menuValueSet == 3)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 56, 0x2713);
  }

  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 26, "DEFAULT");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 36, "FAST");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 46, "MEDIUM");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 56, "SLOW");
  if(key == BTN_MENU)
  {
    sys_cfg.fanSpeed = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 63, "Saved!");
  }
  return treeView[add].keyMode;
}

/* set speaker function */
uint16_t SetSpeakerEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.speaker;
  if(key == BTN_DOWN)
    if(menuValueSet < 1)
      menuValueSet++;
    else
      menuValueSet = 0;
  if(key == BTN_UP)
    if(menuValueSet > 0)
      menuValueSet--;
    else
      menuValueSet = 1;

  if(menuValueSet == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 30, 0x2713);
  }
  if(menuValueSet == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 45, 0x2713);
  }
  u8g2_SetFont(&u8g2, u8g2_font_7x13_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 30, "BUZZER OFF");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 45, "BUZZER ON");
  if(key == BTN_MENU)
  {
    sys_cfg.speaker = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}

uint16_t SetDisplayEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
     menuValueSet = sys_cfg.viewPage;
  if(key == BTN_DOWN)
  {
    if(menuValueSet < 2)
      menuValueSet++;
    else
      menuValueSet = 0;
  }
  if(key == BTN_UP)
  {
    if(menuValueSet > 0)
      menuValueSet--;
    else
      menuValueSet = 2;
  }

  if(menuValueSet == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 28, 0x2713);
  }
  if(menuValueSet == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 40, 0x2713);
  }
  if(menuValueSet == 2)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 52, 0x2713);
  }

  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 28, "AUTO");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 40, "MAIN PAGE");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 52, "INFO PAGE");
  if(key == BTN_MENU)
  {
    sys_cfg.viewPage = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 62, "Saved!");
  }
  return treeView[add].keyMode;
}

/* set brighness display function */
uint16_t SetBrighnessEvt(uint8_t add, uint16_t key)
{
  // printf("Key pointerFunc: %d\n", key);
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.brightness;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 5, 36, 0x2600);
  if(key == BTN_UP)
    if(menuValueSet < 100)
      menuValueSet++;
  if(key == BTN_DOWN)
    if(menuValueSet > 0)
      menuValueSet--;
  Set_Backlight((uint8_t)menuValueSet);
  
  u8g2_DrawBox(&u8g2, 20, 24, menuValueSet, 10); 
  
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 40, 48, 0x23ec);
  u8g2_DrawGlyph(&u8g2, 77, 48, 0x23eb);

  sprintf((char*)buff, "%03d%%", menuValueSet);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 48, buff);

  if(key == BTN_MENU)
  {
    sys_cfg.brightness = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}

/* set backlight function */
uint16_t SetBackLightEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = sys_cfg.backlight;

  if(key == BTN_DOWN)
  {
    if(menuValueSet < 1)
      menuValueSet++;
    else
      menuValueSet = 0;
  }
  if(key == BTN_UP)
  {
    if(menuValueSet > 0)
      menuValueSet--;
    else
      menuValueSet = 1;
  }

  if(menuValueSet == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 30, 0x2713);
    BL_On();
  }
  if(menuValueSet == 1)
  {
    u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
    u8g2_DrawGlyph(&u8g2, 4, 45, 0x2713);
    BL_Off();
  }
  u8g2_SetFont(&u8g2, u8g2_font_7x13_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 30, "BACKLIGHT ON");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*2, 45, "BACKLIGHT OFF");
  if(key == BTN_MENU)
  {
    sys_cfg.backlight = (uint8_t)menuValueSet;
    CFG_Save();
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}

/* view UV time used function */
uint16_t ViewUVTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = 0;
  if(sys.uvTime/HOUR > 9999) sys.uvTime = sys.uvTime * HOUR;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 55, 54, 0x23f0);
  sprintf((char*)buff, "%04d Hours", sys.uvTime/HOUR);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen("UV Time Worked!"))/2)*6, 24, "UV Time Worked!");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
 
  if(key == BTN_MENU)
  {
      CFG_Save();
      // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      // u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}

/* view Neon time used function */
uint16_t ViewFilterTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = 0;
  if(sys.filterTime/HOUR > 9999) sys.filterTime = sys.filterTime * HOUR;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 55, 54, 0x23f0);
  sprintf((char*)buff, "%04d Hours", sys.filterTime/HOUR);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen("Neon Time Worked!"))/2)*6, 24, "Neon Time Worked!");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
 
  if(key == BTN_MENU)
  {
      CFG_Save();
      // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      // u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}

uint16_t ViewSystemTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
    menuValueSet = 0;
  if(sys.sysTime/HOUR > 99999) sys.sysTime = sys.sysTime * HOUR;
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
  u8g2_DrawGlyph(&u8g2, 55, 54, 0x23f0);
  sprintf((char*)buff, "%05d Hours", sys.sysTime/HOUR);
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2, ((22 - strlen("Systems Time Worked!"))/2)*6, 24, "Systems Time Worked!");
  u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
 
  if(key == BTN_MENU)
  {
      CFG_Save();
      // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      // u8g2_DrawStr(&u8g2, ((22 - strlen("Saved!"))/2)*6, 60, "Saved!");
  }
  return treeView[add].keyMode;
}

/* Reset UV time function , password: 1234*/
uint16_t ResetUVTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = 0;
    menuCursor = 0;
    dozens = units = hundreds = thousands = 0;
  }
  if(key == BTN_MENU)
  {
    if(menuCursor < 3) menuCursor++;
    else
    {
      menuValueSet = thousands*1000 + hundreds*100 + dozens*10 + units;
      if(menuValueSet == sys_cfg.passWord)
      {
        if(key == BTN_MENU)
        {
          sys.uvTime = 0;
          Save_SysTime_BKUP();
          CFG_Save();
          //sprintf((char*)buff, "%04d Hours", sys.UVTime);
          u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
          sprintf((char*)buff, "\4 Reset UV Time!\n");
          u8g2_DrawStr(&u8g2,((22 - strlen("Reset UV Time!"))/2)*6, 48, "Reset UV Time!");
        }
      }
      else
      {
        if(key == BTN_MENU)
        {
          sys.uvTime = sys.uvTime;
          //sprintf((char*)buff, "%04d Hours", sys.UVTime);
          u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
          u8g2_DrawStr(&u8g2,((22 - strlen("Wrong Password!"))/2)*6, 48, "Wrong Password!");
          menuCursor = 0;
        }
      }
    }
  }

  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(thousands < 9) thousands++;
        else thousands = 0;
        break;
      case 1:
        if(hundreds < 9) hundreds++;
        else hundreds = 0;
        break;
      case 2:
        if(dozens < 9) dozens++;
        else hundreds = 0;
        break;
      case 3:
        if(units < 9) units++;
        else units = 0;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(thousands > 0) thousands--;
        else thousands = 9;
        break;
      case 1:
        if(hundreds > 0) hundreds--;
        else hundreds = 9;
        break;
      case 2:
        if(dozens > 0) dozens--;
        else dozens = 9;
        break;
      case 3:
        if(units > 0) units--;
        else units = 9;
        break;
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2,((22 - strlen("Enter Password!"))/2)*6, 24, "Enter Password!");
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", thousands);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d%d", hundreds, dozens, units);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d", thousands);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", hundreds);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d", dozens, units);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d", thousands, hundreds);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", dozens);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      sprintf((char*)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d%d", thousands, hundreds, dozens);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  // sprintf((char*)buff, "%04d", menuValueSet);
  // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  // u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  return treeView[add].keyMode;
}

/* Reset Neon time function , password: 1234*/
uint16_t ResetFilterTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = 0;
    menuCursor = 0;
    dozens = units = hundreds = thousands = 0;
  }
  if(key == BTN_MENU)
  {
    if(menuCursor < 3) menuCursor++;
    else
    {
      menuValueSet = thousands*1000 + hundreds*100 + dozens*10 + units;
      if(menuValueSet == sys_cfg.passWord)
      {
        if(key == BTN_MENU)
        {
          sys.filterTime = 0;
          Save_SysTime_BKUP();
          CFG_Save();
          //sprintf((char*)buff, "%04d Hours", sys.UVTime);
          u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
          sprintf((char*)buff, "\4 Reset UV Time!\n");
          u8g2_DrawStr(&u8g2,((22 - strlen("Reset UV Time!"))/2)*6, 48, "Reset UV Time!");
        }
      }
      else
      {
        if(key == BTN_MENU)
        {
          sys.filterTime = sys.filterTime;
          //sprintf((char*)buff, "%04d Hours", sys.UVTime);
          u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
          u8g2_DrawStr(&u8g2,((22 - strlen("Wrong Password!"))/2)*6, 48, "Wrong Password!");
          menuCursor = 0;
        }
      }
    }
  }

  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(thousands < 9) thousands++;
        else thousands = 0;
        break;
      case 1:
        if(hundreds < 9) hundreds++;
        else hundreds = 0;
        break;
      case 2:
        if(dozens < 9) dozens++;
        else hundreds = 0;
        break;
      case 3:
        if(units < 9) units++;
        else units = 0;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(thousands > 0) thousands--;
        else thousands = 9;
        break;
      case 1:
        if(hundreds > 0) hundreds--;
        else hundreds = 9;
        break;
      case 2:
        if(dozens > 0) dozens--;
        else dozens = 9;
        break;
      case 3:
        if(units > 0) units--;
        else units = 9;
        break;
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2,((22 - strlen("Enter Password!"))/2)*6, 24, "Enter Password!");
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", thousands);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d%d", hundreds, dozens, units);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d", thousands);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", hundreds);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d", dozens, units);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d", thousands, hundreds);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", dozens);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      sprintf((char*)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d%d", thousands, hundreds, dozens);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  // sprintf((char*)buff, "%04d", menuValueSet);
  // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  // u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  return treeView[add].keyMode;
}

/* Reset System time function , password: 1234*/
uint16_t ResetSystemTimeEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = 0;
    menuCursor = 0;
    dozens = units = hundreds = thousands = 0;
  }
  if(key == BTN_MENU)
  {
    if(menuCursor < 3) menuCursor++;
    else
    {
      menuValueSet = thousands*1000 + hundreds*100 + dozens*10 + units;
      if(menuValueSet == sys_cfg.passWord)
      {
        if(key == BTN_MENU)
        {
          sys.sysTime = 0;
          Save_SysTime_BKUP();
          CFG_Save();
          //sprintf((char*)buff, "%04d Hours", sys.UVTime);
          u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
          sprintf((char*)buff, "\4 Reset UV Time!\n");
          u8g2_DrawStr(&u8g2,((22 - strlen("Reset UV Time!"))/2)*6, 48, "Reset UV Time!");
        }
      }
      else
      {
        if(key == BTN_MENU)
        {
          sys.sysTime = sys.sysTime;
          //sprintf((char*)buff, "%04d Hours", sys.UVTime);
          u8g2_DrawStr(&u8g2,((22 - strlen(buff))/2)*6, 36, buff);
          u8g2_DrawStr(&u8g2,((22 - strlen("Wrong Password!"))/2)*6, 48, "Wrong Password!");
          menuCursor = 0;
        }
      }
    }
  }

  if(key == BTN_UP)
  {
    switch(menuCursor)
    {
      case 0:
        if(thousands < 9) thousands++;
        else thousands = 0;
        break;
      case 1:
        if(hundreds < 9) hundreds++;
        else hundreds = 0;
        break;
      case 2:
        if(dozens < 9) dozens++;
        else hundreds = 0;
        break;
      case 3:
        if(units < 9) units++;
        else units = 0;
        break;
    }
  }
  if(key == BTN_DOWN)
  {
    switch(menuCursor)
    {
      case 0:
        if(thousands > 0) thousands--;
        else thousands = 9;
        break;
      case 1:
        if(hundreds > 0) hundreds--;
        else hundreds = 9;
        break;
      case 2:
        if(dozens > 0) dozens--;
        else dozens = 9;
        break;
      case 3:
        if(units > 0) units--;
        else units = 9;
        break;
    }
  }
  u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  u8g2_DrawStr(&u8g2,((22 - strlen("Enter Password!"))/2)*6, 24, "Enter Password!");
  switch(menuCursor)
  {
    case 0:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", thousands);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d%d", hundreds, dozens, units);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      break;
    case 1:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d", thousands);
      u8g2_DrawStr(&u8g2, 48, 36, buff);
      /* blink minutes */
      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", hundreds);
      u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);

      sprintf((char*)buff, "%d%d", dozens, units);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      break;
    case 2:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d", thousands, hundreds);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", dozens);
      u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      sprintf((char*)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      break;
    case 3:
      u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
      sprintf((char*)buff, "%d%d%d", thousands, hundreds, dozens);
      u8g2_DrawStr(&u8g2, 48, 36, buff);

      u8g2_SetDrawColor(&u8g2, 0);
      sprintf((char*)buff, "%d", units);
      u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
      u8g2_SetDrawColor(&u8g2, 1);
      break;
  }
  // sprintf((char*)buff, "%04d", menuValueSet);
  // u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
  // u8g2_DrawStr(&u8g2, ((22 - strlen(buff))/2)*6, 36, buff);
  return treeView[add].keyMode;
}

/* Setup Date Manufacture function , password: 1212 "password Manufacture"*/
uint8_t flagReset, flagView;

uint16_t ManufactureEvt(uint8_t add, uint16_t key)
{
  memset(buff, 0, sizeof(buff));
  if(treeView[add].keyMode != menuIdxLast)
  {
    menuValueSet = 0;
    menuCursor = 0;
    dozens = units = hundreds = thousands = 0;
    menuRtc = sys_cfg.dateManf;
    flagReset = flagView = 0;
  }
  if((key == BTN_MENU) && (flagReset == 0)) 
  {
    flagReset = 1;
    menuCursor = 0;
  }
  if(flagReset)
  {
    if(key == BTN_UP || key == BTN_DOWN)
    {
      flagView = 1;
    }
  }
  if(flagView == 0)
  {
    u8g2_SetFont(&u8g2, u8g2_font_9x15_tf);
    sprintf(buff, "%02d/%02d/%04d\n", menuRtc.mday, menuRtc.month, menuRtc.year);
    u8g2_DrawStr(&u8g2, 18, 32, buff);

    sprintf(buff, " %02d:%02d:%02d \n", menuRtc.hour, menuRtc.min, menuRtc.sec);
    u8g2_SetFont(&u8g2, u8g2_font_9x15_tf);
    u8g2_DrawStr(&u8g2, 18, 48, buff);
  }
  else
  {
    if(key == BTN_MENU)
    {
      if(menuCursor < 3) menuCursor++;
      else
      {
        menuValueSet = thousands*1000 + hundreds*100 + dozens*10 + units;
        if(menuValueSet == sys_cfg.passOrigin)
        {
          if(key == BTN_MENU)
          {
            sys_cfg.dateManf = localTime;
            CFG_Save();
            u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
            u8g2_DrawStr(&u8g2,((22 - strlen("Set Date Manufacture!"))/2)*6, 48, "Set Date Manufacture!");
          }
        }
        else
        {
          if(key == BTN_MENU)
          {
            u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
            u8g2_DrawStr(&u8g2,((22 - strlen("Wrong Password!"))/2)*6, 48, "Wrong Password!");
            menuCursor = 0;
          }
        }
      }
    }

    if(key == BTN_UP)
    {
      switch(menuCursor)
      {
        case 0:
          if(thousands < 9) thousands++;
          else thousands = 0;
          break;
        case 1:
          if(hundreds < 9) hundreds++;
          else hundreds = 0;
          break;
        case 2:
          if(dozens < 9) dozens++;
          else hundreds = 0;
          break;
        case 3:
          if(units < 9) units++;
          else units = 0;
          break;
      }
    }
    if(key == BTN_DOWN)
    {
      switch(menuCursor)
      {
        case 0:
          if(thousands > 0) thousands--;
          else thousands = 9;
          break;
        case 1:
          if(hundreds > 0) hundreds--;
          else hundreds = 9;
          break;
        case 2:
          if(dozens > 0) dozens--;
          else dozens = 9;
          break;
        case 3:
          if(units > 0) units--;
          else units = 9;
          break;
      }
    }
    u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
    u8g2_DrawStr(&u8g2,((22 - strlen("Enter Password!"))/2)*6, 24, "Enter Password!");
    switch(menuCursor)
    {
      case 0:
        u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
        u8g2_SetDrawColor(&u8g2, 0);
        sprintf((char*)buff, "%d", thousands);
        u8g2_DrawStr(&u8g2, 48, 36, buff);
        u8g2_SetDrawColor(&u8g2, 1);

        sprintf((char*)buff, "%d%d%d", hundreds, dozens, units);
        u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
        break;
      case 1:
        u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
        sprintf((char*)buff, "%d", thousands);
        u8g2_DrawStr(&u8g2, 48, 36, buff);
        /* blink minutes */
        u8g2_SetDrawColor(&u8g2, 0);
        sprintf((char*)buff, "%d", hundreds);
        u8g2_DrawStr(&u8g2, 48 + 6, 36, buff);
        u8g2_SetDrawColor(&u8g2, 1);

        sprintf((char*)buff, "%d%d", dozens, units);
        u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
        break;
      case 2:
        u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
        sprintf((char*)buff, "%d%d", thousands, hundreds);
        u8g2_DrawStr(&u8g2, 48, 36, buff);

        u8g2_SetDrawColor(&u8g2, 0);
        sprintf((char*)buff, "%d", dozens);
        u8g2_DrawStr(&u8g2, 48 + 12, 36, buff);
        u8g2_SetDrawColor(&u8g2, 1);
        sprintf((char*)buff, "%d", units);
        u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
        break;
      case 3:
        u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
        sprintf((char*)buff, "%d%d%d", thousands, hundreds, dozens);
        u8g2_DrawStr(&u8g2, 48, 36, buff);

        u8g2_SetDrawColor(&u8g2, 0);
        sprintf((char*)buff, "%d", units);
        u8g2_DrawStr(&u8g2, 48 + 18, 36, buff);
        u8g2_SetDrawColor(&u8g2, 1);
        break;
    }
  }
  return treeView[add].keyMode;
}
