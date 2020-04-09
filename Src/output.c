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

uint8_t timeOff; //Set time off default: 5s
uint32_t menuValueSet;

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

    tickcnt = HAL_GetTick();
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
        Fan_On();
    else
        Fan_Off();
}

void Auto_Fan(uint8_t autoStatus)
{
    switch (autoStatus)
    {
    case auto5s:
        if (HAL_GetTick() - tickcnt >= 1000)
        {
            dispToggle = 0;
            if (++sys.timedelay >= 2) //After 2s outdoor is locked
            {
                dev.status.uv = 1;     // outside door is lock
                if (sys.timedelay > 4) //After 4s, air nozzle is on
                {
                    dev.status.aut = 1;
                    dev.autoTimeOff = timeOff;
                    if (timeOff <= 0)
                    {
                        dev.fanFlag = 0;
                        dev.outdoorFlag = 0;
                    }
                    printf("Set auto 5s, timeOff = %d second\n", timeOff);
                    timeOff--;
                    dev.status.fan = 1;
                }
            }
            printf("Timedelay = %d s\n", sys.timedelay);
            tickcnt = HAL_GetTick();
        }
        break;
    case setauto:
        if (HAL_GetTick() - tickcnt >= 1000)
        {
            dispToggle = 0;
            if (++sys.timedelay >= 2) //outside door is locked after 3s
            {
                dev.status.uv = 1;
                if ((sys.timedelay > 7) && (dev.setWait != 1)) //After 7s air nozzle is on
                {
                    dev.status.aut = 1;
                    dev.autoTimeOff = sys_cfg.autoCnt;
                    if (sys_cfg.autoCnt <= 0)
                    {
                        sys_cfg.autoCnt = menuValueSet + 1;
                        dev.fanFlag = 0;
                        dev.outdoorFlag = 0;
                    }
                    sys_cfg.autoCnt--;
                    dev.status.fan = 1;
                    // buzzer_short_beep();
                    // printf("Air nozzle is on, fanFlag = %d\r, auto = %d\n", dev.fanFlag, dev.status.aut);
                }
            }
            // printf("AutoTimeOff = %d s\n", dev.autoTimeOff);
            printf("Timedelay = %d s\n", sys.timedelay);
            tickcnt = HAL_GetTick();
        }
        break;
    case 0:
        // AUTO_Clear_Time();
        timeOff = 5; //Reset default time off: 5s
        sys.timedelay = 0;
        // if(dev.setWait)
        // sys_cfg.autoCnt = menuValueSet;
        if (sys_cfg.autoCnt == 0)
            sys_cfg.autoCnt = 10; //Reset default time off: 10s
        break;
    }
}
void Output_Manage(void)
{
    SW_Neon(dev.status.lamp);     //Lamp
    SW_OutDoor(dev.status.uv);    //Outside door
    SW_InDoor(dev.status.indoor); //Inside door
    SW_Fan(dev.status.fan);       //Air Nozzle
    Auto_Fan(dev.fanFlag);        //Air Nozzle is auto
}