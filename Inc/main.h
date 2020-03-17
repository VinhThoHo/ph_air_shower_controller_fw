/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/


#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "u8g2.h"

extern u8g2_t u8g2;

  /* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
