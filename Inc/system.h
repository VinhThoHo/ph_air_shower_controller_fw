/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "main.h"

#define SYSTEM_TIME_MAX		3600*24*365*100 /* 100 years (s) */
#define UV_TIME_MAX			3600*24*365*10
#define NEON_TIME_MAX		3600*24*365*10
#define TIME_PAGE 			15

#define DEVICE_HOLDER		0xAAAA
#define TIME_HOLDER			0xBBBB

#define auto5s 				2
#define setauto				1
/* device status struct */
typedef struct
{
	uint8_t lamp;
	uint8_t uv;
	uint8_t indoor;
	uint8_t fan;
	uint8_t aut;
} DEVICE_STATUS_T;

/* device struct */
typedef struct
{
	uint8_t setWait;
	uint8_t outdoorFlag;
	uint8_t fanFlag;
	uint8_t autoFlag;
	uint32_t autoTimeOff;
	DEVICE_STATUS_T status;
} DEVICE_T;

/* system time struct */
typedef struct
{
	uint32_t tick;
	uint32_t uvTime;
	uint32_t filterTime;
	uint32_t sysTime;
	uint32_t timedelay;
} SYS_STRUCT_T;

/* variable declare*/
extern SYS_STRUCT_T sys;
extern DEVICE_T dev;
extern uint8_t dispToggle;

/* function */
void System_Init(void);
void Device_Save_Status(void);
void Device_Load_Status(void);
void Save_SysTime_BKUP(void);
void System_Manager(void);
void AUTO_Init_Time(void);
void AUTO_Clear_Time(void);
#endif
