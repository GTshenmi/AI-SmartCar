/*
 * capture.c
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */
#include "capture.h"

/*
 *                                                  Capture.Stop()
 *           ___________________________________________________________________________________________________
 *          |                                  |                                |                               |
 *          |                                  |                                |                               |
 *         \|/         Capture.Start();        |          Capture.Read()        |           Read Finish.        |
 *      Capture_Stop-------------------->Capture_Start-------------------->Capture_Busy-------------------->Capture_Fin
 *                                            /|\                                                               |
 *                                             |                                                                |
 *                                             |________________________________________________________________|
 *                                                                Capture.ClearFinFlag()
 * */
image_t Cap_Read(struct capture *self,uint16_t flags)
{
    if(self->State == Capture_Start)
    {
        switch(flags)
        {
            case 0:/*����*/
                self->State = Capture_Busy;
                self->__Read__();
                self->State = Capture_Fin;
                break;
            case 1:/*��ѯ*/
                self->State = Capture_Busy;
                self->__Start__();
                break;
            default:
                break;
        }
    }

    return self->ImageCache;
}

void Cap_ClearReadFinFlag(struct capture *self)
{
    if(self->State == Capture_Fin)
        self->State = Capture_Start;
}

cap_state_t Cap_GetState(struct capture *self)
{
    if(self->State == Capture_Busy)
    {
       if(self->__GetState__())
           self->State = Capture_Fin;
    }

    return self->State;
}

void Cap_Test(struct capture *self)
{
    self->Init(self,30);
    self->Read(self,0);
    self->Report(self);
}

void Cap_Start(struct capture *self)
{
    if(self->State == Capture_Stop)
        self->State = Capture_Start;
}

void Cap_Stop(struct capture *self)
{
    if(self->State != Capture_Stop)
        self->__Stop__();

    self->State = Capture_Stop;
}

void Cap_Report(struct capture *self)
{
    int j,i;

    UART_PutChar(UART0, 0xfe);  //֡ͷ
    UART_PutChar(UART0, 0xef);  //֡ͷ

    for(i = 0; i < IMAGEH; i++)
    {
        for(j = 0; j < IMAGEW; j++)
        {
          //  if(Image_Data[i][j] == 0xfe )  //��ֹ������֡β
            {
          //      Image_Data[i][j] = 0xff;
            }
         //   UART_PutChar(UART0, Image_Data[i][j]); //��������

        }
    }
    UART_PutChar(UART0, 0xef);  //֡β
    UART_PutChar(UART0, 0xfe);  //֡β
}

void Cap_Show(struct capture *self)
{

}

uint8_t Cap_Init(struct capture *self,uint8_t fps)
{
    self->Read = Cap_Read;

    self->GetState = Cap_GetState;

    self->Test = Cap_Test;

    self->Report = Cap_Report;

    self->Show = Cap_Show;

    self->Reg = 0;

    uint8_t res = 0;

    uint8_t *init_config = (uint8_t *)self->__InitConfig__;

    *init_config = fps;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res += self->__Init__(self->__InitConfig__);

    return res;
}




