/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "main.h"
#include "system.h"
#include "rtc.h"

#define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
#define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
#define CONFIG_AREA_SIZE                  4096
#define CONFIG_AREA_START                 ((uint32_t)((0x8000000+FLASH_SIZE) - PAGE_SIZE*(CONFIG_AREA_SIZE/PAGE_SIZE)))

#define CFG_HOLDER	0x00FF55AB

/* config struct */
typedef struct {
	uint8_t fanSpeed;
	uint8_t brightness;
	uint8_t backlight;
	uint8_t speaker;
	uint8_t viewPage;
	uint16_t pressureVal;
	uint16_t autoCnt;
	uint16_t passWord;
	uint16_t passOrigin;
	SYS_STRUCT_T systemTime;
	DATE_TIME dateManf;
	uint32_t cfg_holder;
} CFG_STRUCT;

/* variable declare*/
extern CFG_STRUCT sys_cfg;

/* function */
void CFG_Save(void);
void CFG_Load(void);

#endif
