/*
 * gpio.c
 *
 *  Created on: 2020年12月3日
 *      Author: GT_shenmi
 */
#include <gpio.h>

uint8_t GPIOx_Init(gpiox_t *gpio)
{
    if(gpio == NULL)
        return 1;

    if(gpio->Enable_Interrupt)
    {
        //PIN_Exti

        int i,j;
        Ifx_P *port = PIN_GetModule(gpio->Pin);
        unsigned char pinIndex = PIN_GetIndex(gpio->Pin);

        /* 关闭中断 */
        boolean        interruptState = IfxCpu_disableInterrupts();

        IfxScu_Req_In * reqPin =  0;

        /* 检测管脚 是否是 固定的17个管脚 */
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 4; j++)
            {
                if(IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
                {

                }
                else if((uint32)port == (uint32)(IfxScu_Req_In_pinTable[0][i][j]->pin.port) && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
                {
                    reqPin = IfxScu_Req_In_pinTable[0][i][j];
                }
            }
        }

        if(reqPin == 0)
        {
    #pragma warning 557         // 屏蔽警告
            while (1);          /* 请选择有外部中断功能的管脚 */
    #pragma warning 557         // 打开警告
        }

        /* 配置上下拉电阻 */
        IfxScuEru_initReqPin(reqPin, gpio->Mode);

        /* 根据输入引脚确定输入通道EICRm */
        IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel)reqPin->channelId;

        /* 配置中断模式 */
        IfxScuEru_disableFallingEdgeDetection(inputChannel);                                               /* EICRm.FENn */
        IfxScuEru_disableRisingEdgeDetection(inputChannel);                                                /* EICRm.RENn */
        if(gpio->Mode == IfxPort_InputMode_pullUp || gpio->Mode == IfxPort_InputMode_noPullDevice)
        {
            IfxScuEru_enableFallingEdgeDetection(inputChannel);               /* EICRm.FENn */
        }
        if(gpio->Mode == IfxPort_InputMode_pullDown|| gpio->Mode == IfxPort_InputMode_noPullDevice)
        {
            IfxScuEru_enableRisingEdgeDetection(inputChannel);                /* EICRm.RENn */
        }
        IfxScuEru_enableAutoClear(inputChannel);

        /* 信号的目的地 */
        IfxScuEru_InputNodePointer triggerSelect = (IfxScuEru_InputNodePointer)inputChannel;
        IfxScuEru_OutputChannel    outputChannel = (IfxScuEru_OutputChannel)inputChannel;

    //  /* Connecting Matrix, Event Trigger Logic ETL block */
        IfxScuEru_enableTriggerPulse(inputChannel);                                                        /* EICRM.EIENN */
        IfxScuEru_connectTrigger(inputChannel, triggerSelect);                                             /* EICRm.INPn */

        /* output channels, OutputGating Unit OGU block */
        IfxScuEru_setFlagPatternDetection(outputChannel, inputChannel, FALSE);                             /* IGCRm.IPENn */
        IfxScuEru_disablePatternDetectionTrigger(outputChannel);                                           /* IGCRm.GEENn */
        IfxScuEru_setInterruptGatingPattern(outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive); /* IGCEm.IGPn */

        unsigned char vectabNum = PinIrqVectabNum[(int)inputChannel % 4];
        unsigned char Priority  = PinIrqPriority[(int)inputChannel % 4];
        /* service request configuration */
        {
            volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCUERU[(int)outputChannel % 4];
            IfxSrc_init(src, vectabNum, Priority);
            IfxSrc_enable(src);
        }

        /* 安装管理中断的中断服务 */
        IfxCpu_Irq_installInterruptHandler((void *)PinIrqFuncPointer[(int)inputChannel % 4], Priority);

        /* 开启中断 */
        IfxCpu_restoreInterrupts(interruptState);

        //PIN_ExtiEnable

        port = PIN_GetModule(gpio->Pin);
        pinIndex = PIN_GetIndex(gpio->Pin);

        /* 关闭中断 */
        interruptState = IfxCpu_disableInterrupts();

        reqPin =  NULL;

        /* 检测管脚 是否是 固定的17个管脚 */
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 4; j++)
            {
                if(IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
                {

                }
                else if((uint32)port == (uint32)(IfxScu_Req_In_pinTable[0][i][j]->pin.port) && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
                {
                    reqPin = IfxScu_Req_In_pinTable[0][i][j];
                }
            }
        }

        if(reqPin == 0)
        {
    #pragma warning 557         // 屏蔽警告
            while (1);          /* 请选择有外部中断功能的管脚 */
    #pragma warning 557         // 打开警告
        }

        /* 根据输入引脚确定输入通道EICRm */
        inputChannel = (IfxScuEru_InputChannel)reqPin->channelId;

        volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCUERU[(int)inputChannel % 4];

        IfxSrc_clearOverrun(src);
        IfxSrc_clearRequest(src);

        if(true)
        {
            IfxSrc_enable(src);
        }
        else
        {
            IfxSrc_disable(src);
        }


        /* 开启中断 */
        IfxCpu_restoreInterrupts(interruptState);
    }
    else
    {
        if(!gpio->Is_Shield)
        {
            Ifx_P *port = PIN_GetModule(gpio->Pin);
            unsigned char pinIndex = PIN_GetIndex(gpio->Pin);

            /* 配置 GPIO模式 */
            IfxPort_setPinMode(port, pinIndex,  gpio->Mode);

            IfxPort_setPinPadDriver(port,  pinIndex,  IfxPort_PadDriver_cmosAutomotiveSpeed2);


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
    }

    return 0;
}

uint8_t GPIOx_Read(gpiox_t *gpio)
{
    if(gpio == NULL)
        return 1;

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
    if(gpio == NULL)
        return 1;

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
    if(gpio == NULL)
        return 1;

    Ifx_P *port = PIN_GetModule(gpio->Pin);
    unsigned char pinIndex = PIN_GetIndex(gpio->Pin);

    IfxPort_togglePin(port, pinIndex);

    return 0;
}

uint8_t GPIOx_SetDir(gpiox_t *gpio,gpio_dir_t dir)
{
    if(gpio == NULL)
        return 1;

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

