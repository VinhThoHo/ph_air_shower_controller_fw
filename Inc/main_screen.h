/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __MAIN_SCREEN_H__
#define __MAIN_SCREEN_H__

#include "main.h"
#include "bitmaps.h"

#define F_WARNING 5

typedef struct {
    uint8_t fanRotate;
    uint8_t warnCnt;
    uint32_t tick;
} MAIN_SCREEN_T;

/* variable declare*/
extern MAIN_SCREEN_T main_scr;

/* function */
void main_screen_init(void);
void Lamp_Status(uint8_t lampStatus);
void OutDoor_Status(uint8_t outdoorStatus);
void TimerOff_Uv(uint32_t t_Uv);
void Fan_Status(uint8_t fanStatus);
void Socket_Status(uint8_t sStatus);
void Main_Screen_Manage(void);

#endif
