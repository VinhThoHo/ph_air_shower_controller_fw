// /*
// * @Author: nhantt
// * @Date:   2020-02-12
// * @Last Modified by:   nhantt
// * @Last Modified time: 2020-02-12
// */

#include "input.h"
#include "output.h"
#include "main_screen.h"
#include "info_screen.h"
#include "system.h"

INPUT_STRUCT input;

void Input_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = In1_Pin | In2_Pin | In3_Pin | In4_Pin | In5_Pin | In6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    for (input.name = 0; input.name < NUM_INPUT; input.name++)
    {
        input.ext[input.name].active = input.ext[input.name].actived = IN_RELEASE;
        input.ext[input.name].waitRelease = 0;
        input.ext[input.name].cnt = 0;
    }
    input.tick = HAL_GetTick();
}

void Input_Get(uint8_t port_id)
{
    uint16_t inputGet;
    switch (port_id)
    {
    case 0:
        inputGet = In1();
        if (inputGet == !In1_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 1:
        inputGet = In2();
        if (inputGet == !In2_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 2:
        inputGet = In3();
        if (inputGet == !In3_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 3:
        inputGet = In4();
        if (inputGet == !In4_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 4:
        inputGet = In5();
        if (inputGet == !In5_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    case 5:
        inputGet = In6();
        if (inputGet == !In6_Pin)
            input.ext[port_id].active = IN_ACTIVE;
        else
            input.ext[port_id].active = IN_RELEASE;
        break;
    }
    if (input.ext[port_id].active != input.ext[port_id].actived)
    {
        ++(input.ext[port_id].cnt);
        if (input.ext[port_id].cnt == NUM_OF_SAMPLE_INPUT)
        {
            input.ext[port_id].actived = input.ext[port_id].active;
            input.ext[port_id].cnt = 0;
        }
    }
    else
    {
        input.ext[port_id].cnt = 0;
    }
}

// uint8_t KeyPress(uint8_t id)
// {
// 	return key.ext[id].pressed;
// }

uint8_t Input_Process(void)
{
    uint8_t currentState = 0;
    for (input.name = 0; input.name < NUM_INPUT; input.name++)
    {
        Input_Get(input.name);

        if (input.ext[input.name].actived == IN_ACTIVE)
        {
            if (input.ext[input.name].waitRelease == 0)
            {
                switch (input.name)
                {
                case IN1:
                    currentState = IN1_ON;
                    // printf("INPUT1\n");
                    break;
                case IN2:
                    currentState = IN2_ON;
                    // printf("INPUT2\n");
                    break;
                case IN3:
                    currentState = IN3_ON;
                    // printf("INPUT3\n");
                    break;
                case IN4:
                    currentState = IN4_ON;
                    //  printf("INPUT4\n");
                    break;
                case IN5:
                    currentState = IN5_ON;
                    // printf("INPUT5\n");
                    break;
                case IN6:
                    currentState = IN6_ON;
                    // printf("INPUT6\n");
                default:
                    currentState = NO_INPUT;
                    break;
                }
            }
            input.ext[input.name].waitRelease = 1;
        }
        if (input.ext[input.name].actived == IN_RELEASE)
        {
            if (input.ext[input.name].waitRelease == 1)
            {
                switch (input.name)
                {
                case IN1:
                    currentState = IN1_OFF;
                    break;
                case IN2:
                    currentState = IN2_OFF;
                    break;
                case IN3:
                    currentState = IN3_OFF;
                    break;
                case IN4:
                    currentState = IN4_OFF;
                    break;
                case IN5:
                    currentState = IN5_OFF;
                    break;
                case IN6:
                    currentState = IN6_OFF;
                    break;
                default:
                    currentState = NO_INPUT;
                    break;
                }
            }
            input.ext[input.name].waitRelease = 0;
        }
    }
    return currentState;
}

void Input_Manage(void)
{
    if (HAL_GetTick() - input.tick > 1)
    {
        if (In4() == 0) //Exit on
        {
            dev.status.lamp = 1; //Led on
            dev.status.uv = 0;   //outside door isn't lock
            dev.status.aut = 0;  //inside door isn't lock
            dev.status.fan = 0;  //air nozzle is off
        }
        else //Exit off
        {
            if ((In3() == 1) && (In2() == 0) && (In1() == 0)) //people sensor off, inside sensor off, outside sensor off
            {
                dev.status.uv = 0;   //outside door isn't lock
                dev.status.aut = 0;  //inside door isn't lock
                dev.status.lamp = 0; //Led is off
                dev.status.fan = 0;  //air nozzle is off
                /* --------add Flag display LCD-------- */
            }
            else if (In1() == 1) //outside sensor on or the outside door is opened
            {
                // dev.doorFlag = ModeWait;
                dev.doorFlag = on;
                dev.status.aut = 1; //inside door is locked
            }
            else if ((In1() == 0) && (In3() == 1) && dev.doorFlag == 1) //The outside door is opened after that is closed and no people inside
            {
                // toggle = TIME_PAGE;
                dev.fanFlag = off;
                dev.status.lamp = 0; //Led off
                dev.status.uv = 1;   //outside door is locked
                dev.status.aut = 1;  //inside door is locked
                dev.status.fan = 1;  //air nozzle is on in 5s
                dev.doorFlag == off;
            }
            else if ((In1() == 1) && (In3() == 0)) //The outside door is opened and have people inside
            {
                // toggle = TIME_PAGE;
                dev.status.lamp = 1; //Led on
                dev.status.aut = 1;  //inside door is locked
                dev.status.fan = 0;  //air nozzle is off
            }
            else if ((In1() == 0) && (In3() == 0) && dev.doorFlag == 1) //Outside sensor off, people sensor on
            {
                dev.status.lamp = 1; //Led on
                dev.status.aut = 1;  //inside door is locked
                /* ------setup time phun khi------ */
                dev.doorFlag == off;
                dev.fanFlag = on;
            }
            else if ((In1() == 0) && (In3() == 0) && dev.fanFlag == 1) //Outside sensor off, people sensor on, air nozzle off
            {
                dev.status.uv = 0;  //outside door isn't lock
                dev.status.aut = 0; //inside door isn't lock
            }
            else if (In2() == 1) //Inside sensor on
            {
                dev.status.uv = 1; //outside door is locked
            }
        }

        /* switch (Input_Process())
        {
        case IN1_ON:
            toggle = TIME_PAGE;
            dev.doorLevel = 1;
            break;
        case IN2_ON:
            toggle = TIME_PAGE;
            dev.doorLevel = 2;
            break;
        case IN3_ON:
            toggle = TIME_PAGE;
            dev.doorLevel = 3;
            break;
        case IN4_ON:
            // printf("IN4 ON\n");
            break;
        case IN5_ON:
            // printf("IN5 ON\n");
            break;
        case IN6_ON:
            // printf("IN6 ON\n");
            break;
        case IN1_OFF:
            dev.doorLevel--;
            toggle = TIME_PAGE;
            break;
        case IN2_OFF:
            dev.doorLevel--;
            toggle = TIME_PAGE;
            break;
        case IN3_OFF:
            dev.doorLevel--;
            toggle = TIME_PAGE;
            break;
        case IN4_OFF:
            break;
        case IN5_OFF:
            break;
        case IN6_OFF:
            break;
        default:
            break;
        } */
        input.tick = HAL_GetTick();
    }
}