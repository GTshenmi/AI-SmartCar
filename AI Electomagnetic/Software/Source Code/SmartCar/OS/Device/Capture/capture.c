/*
 * capture.c
 *
 *  Created on: 2020��12��14��
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
image_t Capture_Read(struct capture *self,uint16_t flags)
{
    if(self->State == Capture_Started)
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
                while(CameraGetState());
                //CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
                break;
            default:
                break;
        }
    }

    return self->ImageCache;
}

void Capture_ClearReadFinFlag(struct capture *self)
{
    if(self->State == Capture_Fin)
        self->State = Capture_Started;
}

cap_state_t Capture_GetState(struct capture *self)
{
    if(self->State == Capture_Busy)
    {
       if(!self->__GetState__())
           self->State = Capture_Fin;
    }

    return self->State;
}

void Capture_Test(struct capture *self)
{
    self->Init(self,30);
    self->Read(self,0);
    self->Report(self,self->ImageCache);
}

uint8_t Capture_Start(struct capture *self)
{
    if(self->State == Capture_Stopped)
    {
        self->__Start__();
        self->State = Capture_Started;
    }
    
    return self->State;
}

uint8_t Capture_Stop(struct capture *self)
{
    if(self->State != Capture_Stopped)
        self->__Stop__();

    self->State = Capture_Stopped;
    
    return self->State;
}

void Capture_Report(struct capture *self,image_t image)
{
    //int j,i;

    UARTx.WriteByte(self->ReportUartn,0xfe,0xffffffff);
    UARTx.WriteByte(self->ReportUartn,0xef,0xffffffff);

//    for(i = 0; i < image.Hight; i++)
//    {
//        for(j = 0; j < image.Width; j++)
//        {
//            if(image.Array[i][j] == 0xfe )  //��ֹ������֡β
//            {
//                image.Array[i][j] = 0xff;
//            }
//            UARTx.WriteByte(self->ReportUartn,image.Array[i][j],0xffffffff);
//        }
//    }

    UARTx.WriteByte(self->ReportUartn,0xef,0xffffffff);
    UARTx.WriteByte(self->ReportUartn,0xfe,0xffffffff);
}

void Capture_Show(struct capture *self,image_t image,uint8_t flags)
{
    //uint16_t color;
    
    switch(flags)
    {
      case 0:/*ԭͼ��*/
          /* ��ʾͼ�� */
//        Get_Use_Image();
//        for(int i = 0; i < Use_ROWS; i++)
//        {
//          for(int j = 0; j < Use_Line; j++)
//          {
//
//            /* ���Ҷ�ת��Ϊ RGB565 */
//            color = 0;
//
//            color = (Image_Use[i][j].gray[0] >> 3) << 11;
//            color |= (Image_Use[i][j].gray[0] >> 2) << 5;
//            color |= Image_Use[i][j].gray[0] >> 3;
//
//            LCD_DrawPoint(i,j,color);
//           }
//        }
        break;
        case 1:/*��ֵ��*/
//          if(image.Array[i][j] == 1)
//            LCD_DrawPoint((uint16_t)j,(uint16_t)i,0x0000);
//          else
//            LCD_DrawPoint((uint16_t)j,(uint16_t)i,0xffff);
          break;
        default:
          break;
    }
//    for(int i = 0; i < image.Hight; i++)
//    {
//        for(int j = 0; j < image.Width; j++)
//        {
//            uint16_t color = 0;
//            switch(flags)
//            {
//                case 0:/*ԭͼ��*/
//                    color = (uint16_t)(Use_Image[i][j] >> 3) << 11;
//                    color |= (uint16_t)(image.Array[i][j] >> 2) << 5;
//                    color |= (uint16_t)(image.Array[i][j]) >> 3;
//                    LCD_DrawPoint((uint16_t)j,(uint16_t)i,color);
//                    break;
//                case 1:/*��ֵ��*/
////                    if(image.Array[i][j] == 1)
////                        LCD_DrawPoint((uint16_t)j,(uint16_t)i,0x0000);
////                    else
////                        LCD_DrawPoint((uint16_t)j,(uint16_t)i,0xffff);
////                    break;
//                default:
//                    break;
//            }
//        }
//    }
}

uint8_t Capture_Init(struct capture *self,uint8_t fps)
{
    self->Read = Capture_Read;

    self->GetState = Capture_GetState;

    self->Test = Capture_Test;

    self->Report = Capture_Report;

    self->Show = Capture_Show;
    
    self->ClearFinFlag = Capture_ClearReadFinFlag;
    
    self->State = Capture_Stopped;
    
    self->Start = Capture_Start;
    
    self->Stop = Capture_Stop;

    self->Reg = 0;

    uint8_t res = 0;

    uint8_t *init_config = (uint8_t *)self->__InitConfig__;

    *init_config = fps;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res += self->__Init__(self->__InitConfig__);

    return res;
}




