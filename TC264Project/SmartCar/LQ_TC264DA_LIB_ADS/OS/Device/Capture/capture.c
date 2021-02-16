/*
 * capture.c
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
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
            case 0:/*×èÈû*/
                self->State = Capture_Busy;
                self->__Read__();
                self->State = Capture_Fin;
                break;
            case 1:/*²éÑ¯*/
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

    UART_PutChar(UART0, 0xfe);  //Ö¡Í·
    UART_PutChar(UART0, 0xef);  //Ö¡Í·

    for(i = 0; i < IMAGEH; i++)
    {
        for(j = 0; j < IMAGEW; j++)
        {
          //  if(Image_Data[i][j] == 0xfe )  //·ÀÖ¹´íÎó·¢ËÍÖ¡Î²
            {
          //      Image_Data[i][j] = 0xff;
            }
         //   UART_PutChar(UART0, Image_Data[i][j]); //·¢ËÍÊý¾Ý

        }
    }
    UART_PutChar(UART0, 0xef);  //Ö¡Î²
    UART_PutChar(UART0, 0xfe);  //Ö¡Î²
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




