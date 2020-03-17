/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern SPI_HandleTypeDef hspi1;

void MX_SPI1_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */
