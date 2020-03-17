/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __iwdg_H
#define __iwdg_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* variable declare*/
extern IWDG_HandleTypeDef hiwdg;

/* function */
void MX_IWDG_Init(void);


#ifdef __cplusplus
}
#endif
#endif /*__ iwdg_H */
