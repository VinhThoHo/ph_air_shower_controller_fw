/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

/* variable declare*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* functions */
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void usart_rx_interrupt(UART_HandleTypeDef *huart);
void usart_manager(void);

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
