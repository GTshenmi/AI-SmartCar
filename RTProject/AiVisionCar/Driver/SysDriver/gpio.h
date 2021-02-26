/*
 * gpio.h
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */

#ifndef DRIVER_USRDRIVER_GPIO_H_
#define DRIVER_USRDRIVER_GPIO_H_

#include "LQ_GPIO.h"
#include "platform.h"

typedef enum
{
    GPIOx_Input,GPIOx_Output
}gpio_dir_t;
typedef struct
{
        /*Reversed for GPIOx*/
       // GPIO_Name_t Pin;
        GPIO_Name_t Pin;
        uint32_t Mode;
        uint8_t State;

        bool Enable_Interrupt;
        bool Is_Shield;
}gpiox_t;

typedef struct
{
        uint8_t (*Init)(gpiox_t *);
        uint8_t (*SetDir)(gpiox_t *,gpio_dir_t);
        uint8_t (*Read)(gpiox_t *);
        uint8_t (*Write)(gpiox_t *,uint8_t state);
        uint8_t (*Reverse)(gpiox_t *);
}sgpio_m;


extern sgpio_m GPIOx;

#endif /* DRIVER_USRDRIVER_GPIO_H_ */
