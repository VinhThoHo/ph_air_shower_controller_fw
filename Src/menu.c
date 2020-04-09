/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "menu.h"
#include "main.h"
#include "menu_handler.h"
#include <string.h>
#include "lcd_bl.h"
#include "config.h"
#include "iwdg.h"
#include "system.h"

/* const TREE_VIEW treeView[] = {
    {0x0000, " ", normalEvt, 0},
    {0x0100, " 1.Date Time \n 2.Setup Filter\n 3.Setup Auto Time \n 4.Fan Speed \n", Dis1Evt, 0},
      {0x0100 + 1, " Set Date Time \n", SetTimeEvt, &treeView[1]},
      {0x0100 + 2, " Setup Filter \n", SetPressureEvt, &treeView[1]},
      {0x0100 + 3, " Setup Auto Time \n", SetAutoTimeEvt, &treeView[1]},
      {0x0100 + 4, " Fan Speed \n", SetFanSpeedEvt, &treeView[1]},
    {0x0200, " 5.Speaker \n 6.Select Display \n 7.Brighness \n 8.Backlight \n ", Dis2Evt, 0},
      {0x0200 + 1, " Speaker \n", SetSpeakerEvt, &treeView[6]},
      {0x0200 + 2, " View Page \n", SetDisplayEvt, &treeView[6]},
      {0x0200 + 3, " Brighness \n", SetBrighnessEvt, &treeView[6]},
      {0x0200 + 4, " Backlight \n", SetBackLightEvt, &treeView[6]},
    {0x0300, " 9.View Time UV \n 10.View Time Filter \n 11.View Time System \n 12.Reset UV Time \n", Dis3Evt, 0},
      {0x0300 + 1, " UV Time \n", ViewUVTimeEvt, &treeView[11]},
      {0x0300 + 2, " Filter Time \n", ViewFilterTimeEvt, &treeView[11]},
      {0x0300 + 3, " System Time \n", ViewSystemTimeEvt, &treeView[11]},
      {0x0300 + 4, " Reset UV Time \n", ResetUVTimeEvt, &treeView[11]},
    {0x0400, " 13.Reset Filter Time \n 14.Reset System Time \n 15.Date Manufacture \n ", Dis4Evt, 0},
      {0x0400 + 1, " Reset Filter Time \n", ResetFilterTimeEvt, &treeView[16]},
      {0x0400 + 2, " System Time ", ResetSystemTimeEvt, &treeView[16]},
      {0x0400 + 3, " Date Manufacture ", ManufactureEvt, &treeView[16]},
}; */

const TREE_VIEW treeView[] = {
    {0x0000, " ", normalEvt, 0},
    {0x0100, " 1.Date Time \n 2.Speaker \n 3.Select Display \n 4.Brighness \n", Dis1Evt, 0},
    {0x0100 + 1, " Set Date Time \n", SetTimeEvt, &treeView[1]},
    {0x0100 + 2, " Speaker \n", SetSpeakerEvt, &treeView[1]},
    {0x0100 + 3, " View Page \n", SetDisplayEvt, &treeView[1]},
    {0x0100 + 4, " Brighness \n", SetBrighnessEvt, &treeView[1]},
    {0x0200, " 5.Backlight \n 6.View Time Lamp \n 7.View Time AirFan \n 8.View Time System \n", Dis2Evt, 0},
    {0x0200 + 1, " Backlight \n", SetBackLightEvt, &treeView[6]},
    {0x0200 + 2, " Lamp Time \n", ViewUVTimeEvt, &treeView[6]},
    {0x0200 + 3, " AirFan Time \n", ViewFilterTimeEvt, &treeView[6]},
    {0x0200 + 4, " System Time \n", ViewSystemTimeEvt, &treeView[6]},
    {0x0300, " 9.Reset Lamp Time \n 10.Reset AirFan Time \n 11.Reset System Time \n 12.Date Manufacture \n", Dis3Evt, 0},
    {0x0300 + 1, " Reset Lamp Time \n", ResetUVTimeEvt, &treeView[11]},
    {0x0300 + 2, " Reset AirFan Time \n", ResetFilterTimeEvt, &treeView[11]},
    {0x0300 + 3, " Reset System Time ", ResetSystemTimeEvt, &treeView[11]},
    {0x0300 + 4, " Date Manufacture ", ManufactureEvt, &treeView[11]},
    // {0x0400, " Setup AirFan \n ", Dis4Evt, 0},
    {0x0400, " AirFan Time \n", SetAutoTimeEvt, &treeView[16]},
};

uint32_t btnTime;
BUTTON keyPress[5];
uint16_t menuIdx = 0;
uint16_t menuIdxLast = 0;
uint8_t menuIdxCh = 1;
uint32_t menuValueSet;
uint16_t menuCursor;
uint16_t menuTimePress = 0;
uint16_t menuTimeOut = 0;
uint16_t menuTimeNext = 0;

void BUTTON_Setup(BUTTON *btn, KeyHandler pressEvt, KeyHandler releaseEvt)
{
  btn->PressedEventHandler = pressEvt;
  btn->ReleasedEventHandler = releaseEvt;
  btn->pressed = 1;
  btn->press = 0;
  btn->cnt = 0;
}

void BUTTON_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pins : PC9 */
  GPIO_InitStruct.Pin = UP_PIN | DOWN_PIN | MENU_PIN | EXIT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_PORT, &GPIO_InitStruct);

  BUTTON_Setup(&keyPress[0], MENU_Manager, 0);
  BUTTON_Setup(&keyPress[1], MENU_Manager, 0);
  BUTTON_Setup(&keyPress[2], MENU_Manager, 0);
  BUTTON_Setup(&keyPress[3], MENU_Manager, 0);
  btnTime = HAL_GetTick();
}

void BUTTON_Get(uint8_t port_id)
{
  switch (port_id)
  {
  case BTN_UP:
    if (UpIn() == !UP_PIN)
      keyPress[port_id].press = BTN_PRESS;
    else
      keyPress[port_id].press = BTN_RELEASE;
    break;
  case BTN_DOWN:
    if (DownIn() == !DOWN_PIN)
      keyPress[port_id].press = BTN_PRESS;
    else
      keyPress[port_id].press = BTN_RELEASE;
    break;
  case BTN_MENU:
    if (MenuIn() == !MENU_PIN)
      keyPress[port_id].press = BTN_PRESS;
    else
      keyPress[port_id].press = BTN_RELEASE;
    break;
  case BTN_EXIT:
    if (ExitIn() == !EXIT_PIN)
      keyPress[port_id].press = BTN_PRESS;
    else
      keyPress[port_id].press = BTN_RELEASE;
    break;
  }
  if (keyPress[port_id].press != keyPress[port_id].pressed)
  {
    ++(keyPress[port_id].cnt);
    // Tang thoi gian test su kien
    if (keyPress[port_id].cnt == NUM_OF_SAMPLE_KEY)
    {
      keyPress[port_id].pressed = keyPress[port_id].press;
      keyPress[port_id].cnt = 0;
    }
  }
  else
  {
    // khong co su kien thi xoa bo dem test
    keyPress[port_id].cnt = 0;
  }
}

/* menu event function */
uint16_t MENU_ToEvt(uint16_t mode, uint8_t key)
{
  uint8_t i;
  uint16_t ret = mode;
  uint8_t cnt;
  for (i = 0; i < sizeof(treeView) / sizeof(treeView[0]); i++)
  {
    HAL_IWDG_Refresh(&hiwdg);
    if (treeView[i].keyMode == mode)
    {
      if (treeView[i].handler != 0)
      {
        if ((mode == 0x0100) || (mode == 0x0200) || (mode == 0x0300))
        {
          u8g2_ClearBuffer(&u8g2);
          u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
          u8g2_SetDrawColor(&u8g2, 0);
          u8g2_DrawStr(&u8g2, ((22 - strlen(" MENU SETTING ")) / 2) * 6, 12, " MENU SETTING ");
          u8g2_SetDrawColor(&u8g2, 1);
          u8g2_DrawStr(&u8g2, 10, 24, u8x8_GetStringLineStart(0, treeView[i].display));
          u8g2_DrawStr(&u8g2, 10, 36, u8x8_GetStringLineStart(1, treeView[i].display));
          u8g2_DrawStr(&u8g2, 10, 48, u8x8_GetStringLineStart(2, treeView[i].display));
          u8g2_DrawStr(&u8g2, 10, 60, u8x8_GetStringLineStart(3, treeView[i].display));
          u8g2_DrawStr(&u8g2, 1, (menuIdxCh + 1) * 12, "->");
        }
        else
        {
          u8g2_ClearBuffer(&u8g2);
          u8g2_SetFont(&u8g2, u8g2_font_6x12_mf);
          u8g2_SetDrawColor(&u8g2, 0);
          u8g2_DrawStr(&u8g2, ((22 - strlen(treeView[i].display)) / 2) * 6, 12, treeView[i].display);
          u8g2_SetDrawColor(&u8g2, 1);
        }
        u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
        ret = (*treeView[i].handler)(i, key);
        menuIdxLast = mode;
        return ret;
      }
    }
  }
  return (ret & 0xFF00);
}

void BUTTON_Manage(void)
{
  uint8_t key;
  if (HAL_GetTick() - btnTime >= 5)
  {
    for (key = 0; key < NUM_KEY; key++)
    {
      BUTTON_Get(key);
      // if (((keyPress[key].pressed == BTN_PRESS) && (keyPress[key].waitRelease == 0)) || ((keyPress[key].pressed == BTN_PRESS) && (key == BTN_MENU) && (menuIdx == 0)))
      if ((keyPress[key].pressed == BTN_PRESS) && (key == BTN_MENU) && (menuIdx == 0))
      {
        if (menuIdx == 0)
        {
          if (++menuTimePress > 100)
          {
            menuTimePress = 0;
            menuIdx = 0x0100;
            MENU_ToEvt(menuIdx, key);
          }
        }
      }
      if ((keyPress[key].pressed == BTN_PRESS) && (keyPress[key].waitRelease == 0))
      {
        keyPress[key].waitRelease = 1;
        menuTimeOut = 60 * 100;
        // BL_On();
        if (*keyPress[key].PressedEventHandler != MENU_Manager)
          BUTTON_Setup(&keyPress[key], MENU_Manager, 0);
        else
        {
          HAL_IWDG_Refresh(&hiwdg);
          (*keyPress[key].PressedEventHandler)(key);
        }
      }
      if ((keyPress[key].pressed == BTN_RELEASE) && (keyPress[key].waitRelease == 1))
      {
        menuTimePress = 0;
        keyPress[key].waitRelease = 0;
      }
      // if (keyPress[key].pressed == BTN_RELEASE)
      //   keyPress[key].waitRelease = 0;
    }
    if (menuTimeOut != 0)
      menuTimeOut--;
    if (menuTimeOut == 0)
      menuIdx = 0;
    if (menuTimeNext != 0)
      menuTimeNext--;
    btnTime = HAL_GetTick();
  }
}

void MENU_Manager(uint8_t key)
{
  switch (key)
  {
  case BTN_DOWN:
    menuTimeNext = 6000;
    if (menuIdx != 0)
    {
      if ((menuIdx == 0x0100) || (menuIdx == 0x0200) || (menuIdx == 0x0300))
      {
        if (++menuIdxCh > 4)
        {
          menuIdxCh = 1;
          menuIdx += 0x0100;
          if (menuIdx > 0x0300)
            menuIdx = 0x0100;
        }
        //printf("idx: %.4x - IdxCh: %d\n", menuIdx, menuIdxCh);
      }
      /* else if (menuIdx == 0x0400)
      {
        if (++menuIdxCh > 3)
        {
          menuIdxCh = 1;
          menuIdx = 0x0100;
        }
      } */
      MENU_ToEvt(menuIdx, key);
    }
    else if (menuIdx == 0)
    {
      dispToggle = 0;
    }
    break;
  case BTN_UP:
    menuTimeNext = 6000;
    if (menuIdx != 0)
    {
      if ((menuIdx == 0x0100) || (menuIdx == 0x0200) || (menuIdx == 0x0300))
      {
        if (--menuIdxCh == 0)
        {
          menuIdxCh = 4;
          menuIdx -= 0x0100;
          if (menuIdx < 0x0100)
          {
            menuIdx = 0x0300;
            menuIdxCh = 4;
          }
        }
        //printf("idx: %.4x - IdxCh: %d\n", menuIdx, menuIdxCh);
      }
      /* else if (menuIdx == 0x0400)
      {
        if (--menuIdxCh == 0)
        {
          menuIdxCh = 4;
          menuIdx = 0x0200;
        }
      } */
      MENU_ToEvt(menuIdx, key);
    }
    else if (menuIdx == 0)
    {
      dispToggle = 15;
    }
    break;
  case BTN_MENU:
    if (menuIdx == 0)
    {
      if (++menuTimePress > 200)
      {
        menuTimePress = 0;
        menuIdx = 0x0100;
        MENU_ToEvt(menuIdx, key);
      }
      /* else
      {
        menuIdx = 0x0400;
        MENU_ToEvt(menuIdx, key);
        dev.setWait = 1;
        buzzer_short_beep();
        printf("Setup AirNozzle Time Auto, setWait = %d\n", dev.setWait);
      } */
    }
    else
    {
      if ((menuIdx == 0x0100) || (menuIdx == 0x0200) || (menuIdx == 0x0300))
      {
        menuIdx = menuIdx + menuIdxCh;
        menuTimeNext = 200;
        MENU_ToEvt(menuIdx, 0xFF);
      }
      else
      {
        //if(menuTimeNext == 0)
        MENU_ToEvt(menuIdx, key);
      }
    }
    break;
  case BTN_EXIT:
    if (menuIdx != 0)
    {
      if ((menuIdx != 0x0100) && (menuIdx != 0x0200) && (menuIdx != 0x0300) && (menuIdx != 0x0400))
      {
        menuIdx = (menuIdx & 0xFF00);
        printf("Exit is pressed the first time...\n");
      }
      else
      {
        menuIdx = 0;
        menuIdxCh = 1;
        printf("Exit is pressed a second time...\n");
      }
      MENU_ToEvt(menuIdx, key);
      if (dev.setWait)
        dev.setWait = 0;
      printf("setWait = %d\n", dev.setWait);
    }
    else if (menuIdx == 0)
    {
      menuIdx = 0x0400;
      MENU_ToEvt(menuIdx, key);
      dev.setWait = 1;
      buzzer_short_beep();
      printf("Setup AirNozzle Time Auto, setWait = %d\n", dev.setWait);
    }
    printf("Exit is operating...\n");
    break;
  }
}