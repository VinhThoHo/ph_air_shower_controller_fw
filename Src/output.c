/*
* @Author: nhantt
* @Date:   2020-02-12
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-12
*/

#include "output.h"
#include "main_screen.h"
#include "system.h"
#include "config.h"
#include "buzzer.h"

uint8_t timeOff = 6; //Set time off default: 5s

void Output_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

    ticks = HAL_GetTick();
}

void SW_Neon(uint8_t neonStatus)
{
    if (neonStatus)
        Neon_On();
    else
        Neon_Off();
}
void SW_OutDoor(uint8_t outdoorStatus)
{
    if (outdoorStatus)
        OutDoor_On();
    else
        OutDoor_Off();
}
void SW_InDoor(uint8_t indoorStatus)
{
    if (indoorStatus)
        InDoor_On();
    else
        InDoor_Off();
}
void SW_Fan(uint8_t fanStatus)
{
    if (fanStatus)
    {
        Fan_On();
        // printf("Air Nozzle is on\n");
    }
    else
        Fan_Off();
}
void Auto_Fan(uint8_t autoStatus)
{
    if (autoStatus == auto5s)
    {
        if (HAL_GetTick() - ticks >= 1000)
        {
            if (++timedelay > 4)
            {
                --timeOff;
                dev.autoTimeOff = timeOff;
                dev.status.aut = 1;
                if (timeOff <= 0)
                {
                    // timeOff = 0;
                    dev.autoTimeOff = 0;
                    timeOff = 6; //Reset default time off: 5s
                    timedelay = 0;
                    dev.fanFlag = 0;
                    dev.outdoorFlag = 0;
                }
                dev.status.fan = 1;
                printf("Set auto 5s, timeOff = %d second\n", timeOff);
            }
            ticks = HAL_GetTick();
        }
    }
    else if (autoStatus == off)
    {
        AUTO_Clear_Time();
    }
}
void Output_Manage(void)
{
    SW_Neon(dev.status.lamp);       //Lamp
    SW_OutDoor(dev.status.outdoor); //Outside door
    SW_InDoor(dev.status.indoor);   //Inside door
    SW_Fan(dev.status.fan);         //Air Nozzle
    Auto_Fan(dev.fanFlag);          //Air Nozzle is start in 5s
}