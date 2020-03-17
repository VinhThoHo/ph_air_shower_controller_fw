/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "main.h"

#define Neon_On()               HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)
#define Neon_Off()              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)
#define UV_On()                 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define UV_Off()                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define Fast_Fan_On()           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define Fast_Fan_Off()          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define Medium_Fan_On()         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET) 
#define Medium_Fan_Off()        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define Slow_Fan_On()           HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)
#define Slow_Fan_Off()          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)

void Output_Init(void);
void Output_Manage(void);
void SW_Neon(uint8_t neonStatus);
void SW_Uv(uint8_t uvStatus);
void SW_Fan(uint8_t fanStatus);
void SW_Auto(uint8_t AutoStatus);
#endif