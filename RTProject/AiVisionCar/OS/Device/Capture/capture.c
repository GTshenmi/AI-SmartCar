/*
 * capture.c
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */
#include "capture.h"
#include "driver.h"

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
            case 0:/*阻塞*/
                self->State = Capture_Busy;
                self->__Read__();
                self->State = Capture_Fin;
                break;
            case 1:/*查询*/
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
    {
        self->__Stop__();
        self->State = Capture_Start;
    }
}

cap_state_t Cap_GetState(struct capture *self)
{
    if(self->State == Capture_Busy)
    {
       if(!self->__GetState__())
           self->State = Capture_Fin;
    }

    return self->State;
}

void Cap_Test(struct capture *self)
{
    self->Init(self,30);
    self->Read(self,0);
    self->Report(self,self->ImageCache);
}

uint8_t Cap_Start(struct capture *self)
{
    if(self->State == Capture_Stop)
    {
        //self->__Start__();
        self->State = Capture_Start;
    }
    return 0;
}

uint8_t Cap_Stop(struct capture *self)
{
    if(self->State != Capture_Stop)
        self->__Stop__();

    self->State = Capture_Stop;
    
    return 0;
}

void Cap_Report(struct capture *self,image_t image)
{
    int j,i;

    UARTx.WriteByte(self->ReportUartn,0xfe,0xffffffff);
    UARTx.WriteByte(self->ReportUartn,0xef,0xffffffff);

    for(i = 0; i < image.Hight; i++)
    {
        for(j = 0; j < image.Width; j++)
        {
//            if(image.Array[i][j] == 0xfe )  //防止错误发送帧尾
//            {
//                image.Array[i][j] = 0xff;
//            }
//            UARTx.WriteByte(self->ReportUartn,image.Array[i][j],0xffffffff);
        }
    }

    UARTx.WriteByte(self->ReportUartn,0xef,0xffffffff);
    UARTx.WriteByte(self->ReportUartn,0xfe,0xffffffff);
}

void Cap_Show(struct capture *self,image_t image,uint8_t flags)
{
//    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
//        SCB_DisableDCache();
//    }
//    SCB_EnableDCache();
  
    Get_Use_Image();        
    uint16_t color;
//        /* 显示图像 */
    for(int i = 0; i < Use_ROWS; i++)   
    {
            for(int j = 0; j < Use_Line; j++)
            {
                /* 将灰度转化为 RGB565 */
                color = 0;
                color = (Image_Use[i][j].gray[0] >> 3) << 11;
                color |= (Image_Use[i][j].gray[0] >> 2) << 5;
                color |= Image_Use[i][j].gray[0] >> 3;
                /* 显示 */   
                LCD_DrawPoint (i,j,color);    //显示数据
            }
     }
    
//    for(int i = 0; i < image.Hight; i++)
//    {
//        for(int j = 0; j < image.Width; j++)
//        {
//            color = 0;
//            switch(flags)
//            {
//                case 0:/*原图像*/
//                    color = (uint16_t)(image.Array[i][j].gray[0] >> 3) << 11;
//                    color |= (uint16_t)(image.Array[i][j].gray[0] >> 2) << 5;
//                    color |= (uint16_t)(image.Array[i][j].gray[0]) >> 3;
//                    //LCD_DrawPoint((uint16_t)j,(uint16_t)i,csiFrameBuf[0][i][j].rgb565);
//                    LCD_DrawPoint((uint16_t)j,(uint16_t)i,color);
//                    break;
//                case 1:/*二值化*/
//                      
////                    if(image.Array[0][i][j] == 1)
////                        LCD_DrawPoint((uint16_t)j,(uint16_t)i,0x0000);
////                    else
////                        LCD_DrawPoint((uint16_t)j,(uint16_t)i,0xffff);
//                    
//                    break;
//                default:
//                    break;
//            }
//        }
//    }
}

uint8_t Cap_Init(struct capture *self,uint8_t fps)
{
    self->Read = Cap_Read;

    self->GetState = Cap_GetState;

    self->Test = Cap_Test;

    self->Report = Cap_Report;

    self->Show = Cap_Show;
    
    self->Start = Cap_Start;
    self->Stop = Cap_Stop;
    
    self->State = Capture_Stop;

    self->Reg = 0;

    uint8_t res = 0;

    uint8_t *init_config = (uint8_t *)self->__InitConfig__;

    *init_config = fps;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res += self->__Init__(self->__InitConfig__);

    return res;
}




