/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#ifndef __MENU_H
#define __MENU_H

#include "main.h"

#define 	KEY_PORT		GPIOC

#define		UP_PIN			GPIO_PIN_12
#define 	DOWN_PIN 		GPIO_PIN_11
#define 	MENU_PIN		GPIO_PIN_10
#define 	EXIT_PIN		GPIO_PIN_13

#define  	UpIn()			HAL_GPIO_ReadPin(KEY_PORT, UP_PIN)
#define  	DownIn()		HAL_GPIO_ReadPin(KEY_PORT, DOWN_PIN)
#define  	MenuIn()		HAL_GPIO_ReadPin(KEY_PORT, MENU_PIN)
#define  	ExitIn()		HAL_GPIO_ReadPin(KEY_PORT, EXIT_PIN)

#define 	NUM_KEY 	4
#define  	NUM_OF_SAMPLE_KEY              5

typedef enum {
	BTN_MENU,
	BTN_UP,
    BTN_DOWN,
    BTN_EXIT
} BTN_NAME;

typedef enum {
	BTN_PRESS,
	BTN_RELEASE
} BTN_EVENT;

typedef uint16_t (*MenuHandler)(uint8_t add, uint16_t key);
typedef void (*KeyHandler)(uint8_t key);

typedef struct _TREE_VIEW {
	uint16_t keyMode;
	const uint8_t display[90];
	MenuHandler handler;
	const struct _TREE_VIEW *parrent;
} TREE_VIEW;

typedef struct {
	KeyHandler PressedEventHandler;
	KeyHandler ReleasedEventHandler;
	uint8_t pressed;
    uint8_t press;
    uint8_t waitRelease;
    uint8_t cnt;       
} BUTTON;

extern const TREE_VIEW treeView[];
extern BUTTON keyPress[5];
extern uint16_t menuIdx;
extern uint8_t menuIdxCh;
extern uint32_t menuValueSet;
extern uint16_t menuIdxLast;
extern uint16_t menuCursor;
extern uint16_t menuTimePress;
extern uint16_t menuTimeOut;
extern uint16_t menuTimeNext;

void BUTTON_Get(uint8_t port_id);
void BUTTON_Manage(void);
void BUTTON_Init(void);
void MENU_Task(uint8_t key);
void MENU_CheckTime(void);
void MENU_Manager(uint8_t key);
#endif
