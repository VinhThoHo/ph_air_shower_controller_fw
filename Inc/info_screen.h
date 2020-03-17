/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#ifndef __INFO_SCREEN_H
#define __INFO_SCREEN_H

#include "main.h"
#include "rtc.h"

typedef struct {
    uint8_t dot;
    uint32_t tick;
} INFO_SCREEN_STR;

extern INFO_SCREEN_STR Info_Scr;

void Info_Screen_Init(void);
void RTC_Time(DATE_TIME now);
/* ---------------Version 1--------------- */
// void Filter_Operation(uint32_t value);
void Filter_Operation(float value);
void SysWork(uint32_t value);
/* ---------------Version 2--------------- */
/* void ExhaustFilter(float value);
void DownflowFilter(float value);
void InFlow(float value);
void DownFlow(float value); 
void Temperature(float value);*/
void Door(uint8_t DStatus);
void Info_Screen_Manage(void);

#endif