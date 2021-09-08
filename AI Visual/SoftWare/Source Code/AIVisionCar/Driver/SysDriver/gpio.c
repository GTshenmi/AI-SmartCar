/*
 * gpio.c
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */
#include <gpio.h>

uint8_t GPIOx_Init(gpiox_t *gpio)
{
    if(gpio->Enable_Interrupt)
    {
        PIN_Exti(gpio->Pin,gpio->Mode);
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
        return PIN_Read(gpio->Pin);
    else
        return 1;
}
uint8_t GPIOx_Write(gpiox_t *gpio,uint8_t state)
{
    gpio->State = state;
    
    if(!gpio->Is_Shield)
        PIN_Write(gpio->Pin,gpio->State);
    else
        return 1;

    return 0;
}

uint8_t GPIOx_Reverse(gpiox_t *gpio)
{
    uint8_t state = !gpio->State;
    
    GPIOx_Write(gpio,state);
    
    return 0;
}

uint8_t GPIOx_SetDir(gpiox_t *gpio,gpio_dir_t dir)
{
    PIN_Dir(gpio->Pin,dir);
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

