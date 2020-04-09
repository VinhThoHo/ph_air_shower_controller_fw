/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "main.h"

#define Neon_On()               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define Neon_Off()              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define OutDoor_On()            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define OutDoor_Off()           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define InDoor_On()             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define InDoor_Off()            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define Fan_On()                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET) 
#define Fan_Off()               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)

void SW_Neon(uint8_t neonStatus);
void SW_OutDoor(uint8_t outdoorStatus);
void SW_InDoor(uint8_t indoorStatus);
void SW_Fan(uint8_t fanStatus);
void Auto_Fan(uint8_t autoStatus);
void Output_Init(void);
void Output_Manage(void);
uint32_t tickcnt;
#endif