/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

#define BUZZER_PORT GPIOB
#define BUZZER_PIN GPIO_PIN_8

#define BUZZER_ON() HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, SET)
#define BUZZER_OFF() HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, RESET)
#define BUZZER_TOGGLE() HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN)

#define SHORT_BEEP 	100
#define LONG_BEEP 	1500
#define ALARM		1000


/* buzzer state */
typedef enum
{
	STATE_SB,
	STATE_LB,
	STATE_AL,
	STATE_IDLE
} BUZZER_STATE;


/* buzzer struct manager */
typedef struct
{
	uint32_t tick;
	uint8_t state;
} BUZZER_T;

/* variable declare*/
extern BUZZER_T buzzer;

/* function */
void buzzer_short_beep(void);
void buzzer_long_beep(void);
void buzzer_alarm_start(void);
void buzzer_alarm_stop(void);
void handle_buzzer(void);
void buzzer_manage(void);
void buzzer_init(void);

#endif
