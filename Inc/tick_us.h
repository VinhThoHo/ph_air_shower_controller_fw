/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#ifndef __TICK_US_H
#define __TICK_US_H

#include "main.h"

uint32_t tick_delta_time_tick(uint32_t start, uint32_t end);
uint8_t tick_expire_us(uint32_t *last_tick, uint32_t time_us);
void tick_wait_us(uint32_t wait_us);

#endif