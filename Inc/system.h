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

#define on					1
#define off					0
#define Auto5s			    5

/* device status struct */
typedef struct
{
	uint8_t lamp;//Led
	// uint8_t uv;//outside door
	uint8_t outdoor;//outside door
	uint8_t indoor;//inside door
	// uint8_t ssout;
	// uint8_t ssin;
	uint8_t fan; //air nozzle
	uint8_t aut;
} DEVICE_STATUS_T;

/* device struct */
typedef struct
{
	uint8_t outdoorFlag;//outside door flag
	uint8_t indoorFlag;//inside door flag
	uint8_t fanFlag; //air nozzle flag
	uint8_t autoFlag;//auto air nozzle flag
	uint32_t autoTimeOff;
	DEVICE_STATUS_T status;
} DEVICE_T;

/* system time struct */
typedef struct
{
	uint32_t tick;
	uint32_t uvTime;
	// uint32_t lampTime;
	uint32_t filterTime;
	uint32_t sysTime;
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
