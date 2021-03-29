/*
 * gpio.c
 *
 *  Created on: 2020年12月3日
 *      Author: GT_shenmi
 */
#include <gpio.h>

uint8_t GPIOx_Init(gpiox_t *gpio)
{
    if(gpio->Enable_Interrupt)
    {
        PIN_Exti(gpio->Pin,gpio->Mode);
        PIN_ExtiEnable(gpio->Pin,true);
    }
    else
    {
        if(!gpio->Is_Shield)
            PIN_InitConfig(gpio->Pin,gpio->Mode,gpio->State);
        else
            return 1;
    }

    return 0;
}

uint8_t GPIOx_Read(gpiox_t *gpio)
{
    if(!gpio->Is_Shield)
    {
        Ifx_P *port = PIN_GetModule(gpio->Pin);
        unsigned char pinIndex = PIN_GetIndex(gpio->Pin);
        return IfxPort_getPinState(port, pinIndex);
    }
    else
        return 1;

    return 0;
}
uint8_t GPIOx_Write(gpiox_t *gpio,uint8_t state)
{
    gpio->State = state;
    if(!gpio->Is_Shield)
    {
        Ifx_P *port = PIN_GetModule(gpio->Pin);
        unsigned char pinIndex = PIN_GetIndex(gpio->Pin);

        /* GPIO输出模式时 输出状态 */
        if(0 == gpio->State)
        {
            IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
        }
        else
        {
            IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
        }
    }
    else
        return 1;

    return 0;
}

uint8_t GPIOx_Reverse(gpiox_t *gpio)
{
    Ifx_P *port = PIN_GetModule(gpio->Pin);
    unsigned char pinIndex = PIN_GetIndex(gpio->Pin);

    IfxPort_togglePin(port, pinIndex);

    return 0;
}

uint8_t GPIOx_SetDir(gpiox_t *gpio,gpio_dir_t dir)
{
    Ifx_P *port = PIN_GetModule(gpio->Pin);
    unsigned char pinIndex = PIN_GetIndex(gpio->Pin);

    if(0 == dir)
    {
        /* 配置 GPIO模式 */
        IfxPort_setPinMode(port, pinIndex,  PIN_MODE_INPUT);
    }
    else
    {
        /* 配置 GPIO模式 */
        IfxPort_setPinMode(port, pinIndex,  PIN_MODE_OUTPUT);
    }

    return 0;
}
sgpio_m GPIOx =
{
        .Init = GPIOx_Init,

        .SetDir = GPIOx_SetDir,
        .Read = GPIOx_Read,
        .Write = GPIOx_Write,
        .Reverse = GPIOx_Reverse,
};

