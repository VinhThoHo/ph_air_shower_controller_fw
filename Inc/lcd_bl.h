/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#ifndef __LCD_BACKLIGHT_H__
#define __LCD_BACKLIGHT_H__

#include "main.h"

/* variable declare*/
extern TIM_HandleTypeDef htim1;

/* function */
void MX_TIM1_Init(void);
void Set_Backlight(uint8_t percent);
void BL_Off(void);
void BL_On(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#endif
