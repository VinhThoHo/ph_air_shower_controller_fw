/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "system.h"
#include "config.h"
#include "rtc.h"
#include "main_screen.h"
#include "info_screen.h"
#include <string.h>
#include <stdio.h>
#include "menu.h"
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_rtc_ex.h"
#include "buzzer.h"

SYS_STRUCT_T sys;
DEVICE_T dev;
uint8_t dispToggle;
uint32_t tick_buzzer;

// static void Save_UVTime_BKUP(void);
// static void Load_UVTime_BKUP(void);
static void Load_SysTime(void);

/* system init function */
void System_Init(void)
{
	CFG_Load();
	Load_SysTime();
	sys_cfg.backlight ? BL_Off() : BL_On();
	Set_Backlight(sys_cfg.brightness);
	main_screen_init();
	Info_Screen_Init();
	buzzer_init();
	sys.tick = HAL_GetTick();
}

// void Device_Save_Status(void)
// {
// 	uint16_t *savedest;
// 	savedest = (uint16_t*)&dev.status;
// 	// printf("padding: 0x%.4x\n", *(uint16_t*)(savedest));
// 	// printf("padding: 0x%.4x\n", *(uint16_t*)(savedest+1));
// 	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, *(uint16_t*)(savedest));
// 	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, *(uint16_t*)(savedest+1));
// 	//printf("padding: 0x%.8x\n", *(uint32_t*)(savedest));
// }

// void Device_Load_Status(void)
// {
// 	uint32_t tmp;
// 	tmp = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2));
// 	memcpy(&dev.status, &tmp, 4);
// 	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) != TIME_HOLDER)
//     {
//         dev.status.lamp = 0;
//         dev.status.uv = 0;
//         dev.status.aut = 0;
//         Device_Save_Status();
//         HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, TIME_HOLDER);
//     }
// }

/* Init auto time function */
void AUTO_Init_Time(void)
{
	dev.autoFlag = 1;
	dev.autoTimeOff = sys_cfg.autoCnt * 60;
	dev.status.uv = 1;
	dev.status.lamp = 0;
	dev.status.fan = 0;
}

/* Clear auto time function */
void AUTO_Clear_Time(void)
{
	dev.autoFlag = 0;
	dev.autoTimeOff = 0;
	dev.status.uv = 0;
	dev.status.lamp = 0;
	dev.status.aut = 0;
}

/* check auto time function */
void AUTO_Check_Time(void)
{
	if ((dev.autoFlag == 1) & (dev.autoTimeOff == 0))
	{
		dev.autoFlag = 0;
		dev.status.aut = 0;
		dev.status.uv = 0;
		buzzer_alarm_start();
		tick_buzzer = HAL_GetTick();
	}
	else if ((dev.autoFlag == 1) && (dev.autoTimeOff != 0))
	{
		dev.status.aut = dev.status.aut;
		dev.status.uv = dev.status.uv;
		dev.status.lamp = 0;
		dev.status.fan = 0;
	}
	else
	{
		dev.status.aut = dev.status.aut;
		dev.status.uv = dev.status.uv;
		dev.status.lamp = dev.status.lamp;
		dev.status.fan = dev.status.fan;
		if (HAL_GetTick() - tick_buzzer > 60000)
		{
			buzzer_alarm_stop();
			tick_buzzer = HAL_GetTick();
		}
	}
}

/* system manager */
void System_Manager(void)
{
	if (HAL_GetTick() - sys.tick >= 1000) //Every seconds
	{
		if (++dispToggle > TIME_PAGE * 2)
			dispToggle = 0;
		if (++sys.sysTime > SYSTEM_TIME_MAX)
			sys.sysTime = SYSTEM_TIME_MAX;
		if (dev.status.lamp)
			sys.uvTime++;
		if (dev.status.fan)
			sys.filterTime++;
		sys.tick = HAL_GetTick();
		Save_SysTime_BKUP();
	}
	// AUTO_Check_Time();
	handle_buzzer();
	if (menuIdx == 0)
	{
		if (sys_cfg.viewPage == 1)
		{
			u8g2_ClearBuffer(&u8g2);
			Main_Screen_Manage();
		}
		else if (sys_cfg.viewPage == 2)
		{
			u8g2_ClearBuffer(&u8g2);
			Info_Screen_Manage();
		}
		else
		{
			if (dispToggle >= TIME_PAGE)
			{
				u8g2_ClearBuffer(&u8g2);
				Info_Screen_Manage();
			}
			else
			{
				u8g2_ClearBuffer(&u8g2);
				Main_Screen_Manage();
			}
			/* if (toggle)
			{
				u8g2_ClearBuffer(&u8g2);
				Main_Screen_Manage();
			}
			else
			{
				u8g2_ClearBuffer(&u8g2);
				Info_Screen_Manage();
			} */
		}
	}
}

/* save system time backup */
void Save_SysTime_BKUP(void)
{
	uint32_t tmp = 0U;
	uint16_t i, *savedest;
	savedest = (uint16_t *)&sys;
	// not save sys.tick
	for (i = 0; i < sizeof(SYS_STRUCT_T) / 2; i++)
	{
		//printf("savedest %d :  %d\n", i, *(uint16_t*)(savedest + i));
		tmp = (uint32_t)BKP_BASE;
		tmp += ((RTC_BKP_DR3 + i) * 4U);
		*(__IO uint32_t *)tmp = (*(uint16_t *)(savedest + i) & BKP_DR3_D);
	}
}

/* load system time */
static void Load_SysTime(void)
{
	//sys.tick = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR4) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR3));
	sys.uvTime = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR6) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5));
	sys.filterTime = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR8) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR7));
	sys.sysTime = (uint32_t)(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR10) << 16 | HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR9));
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) != TIME_HOLDER)
	{
		// Read from Flash
		sys = sys_cfg.systemTime;
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, TIME_HOLDER);
	}
}
