/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "config.h"
#include <string.h>
#include <stdio.h>

CFG_STRUCT sys_cfg;

uint32_t Address = 0, PAGEError = 0;
/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

/* save data config to internal flash */
void CFG_Save(void)
{
	uint32_t *cfgdest, i;
	sys_cfg.systemTime = sys;
	cfgdest = (uint32_t*)&sys_cfg;
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  	EraseInitStruct.PageAddress = CONFIG_AREA_START;
  	EraseInitStruct.NbPages     = 1;
	
	//printf("sizeof cfg: %d\n", sizeof(CFG_STRUCT));
	// for(i = 0; i <= sizeof(CFG_STRUCT); i+=4)
	// {
	// 	printf("%d: 0x%.8x\n", i, *(uint32_t*)(cfgdest + i/4));
	// }
	for(i = 0; i <= sizeof(CFG_STRUCT)/PAGE_SIZE; i++)
	{
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	  	{
	  	}
	}
	Address = CONFIG_AREA_START;
	//printf("add: 0x%.8x\n\n", Address);
	for(i = 0; i <= sizeof(CFG_STRUCT); i+=4)
	{
		//printf("%d: 0x%.8x\n", i, *(uint32_t*)(cfgdest + i/4));
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address + i, *(uint32_t*)(cfgdest + i/4)) == HAL_OK);
		if(*(uint32_t*)(cfgdest + i/4) != *(uint32_t*)(Address + i))
			return;
	}
	HAL_FLASH_Lock();
}

/* load data config to variable */
void CFG_Load(void)
{
	memcpy(&sys_cfg, (void*)CONFIG_AREA_START, sizeof(CFG_STRUCT));
	if(sys_cfg.cfg_holder != CFG_HOLDER)
	{
		memset(&sys_cfg, 0x00, sizeof sys_cfg);

		sys_cfg.fanSpeed = 0;
		sys_cfg.brightness = 50;
		sys_cfg.backlight = 1;
		sys_cfg.speaker = 1;
		sys_cfg.viewPage = 0;

		sys_cfg.autoCnt = 0;
		sys_cfg.pressureVal = 300;


		sys_cfg.passWord = 1234;
		sys_cfg.passOrigin = 1212;
		sys_cfg.cfg_holder = CFG_HOLDER;

		sys_cfg.systemTime.tick = 0;
		sys_cfg.systemTime.uvTime = 0;
		sys_cfg.systemTime.filterTime = 0;
		sys_cfg.systemTime.sysTime = 0;

		sys_cfg.dateManf = localTime;

		CFG_Save();
	}
}
