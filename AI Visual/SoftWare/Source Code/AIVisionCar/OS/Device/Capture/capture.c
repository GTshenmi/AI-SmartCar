/*
 * capture.c
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */
#include "capture.h"
#include "driver.h"
#include "devices_config.h"

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
            case 0:/*阻塞*/
                self->State = Capture_Busy;
                self->__Read__();
                self->State = Capture_Fin;
                break;
            case 1:/*查询*/
                self->State = Capture_Busy;
//                while(CameraGetState());
                CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
                break;
            default:
                break;
        }
    }

    return self->ImageCache[0];
}

void Capture_ClearReadFinFlag(struct capture *self)
{
    if(self->State == Capture_Fin)
    {
        self->State = Capture_Started;
        
//        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
    }
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
    //self->Report(self,self->ImageCache);
}

uint8_t Capture_Start(struct capture *self)
{
    if(self->State == Capture_Stopped)
    {
       SCB_DisableDCache();
       SCB_EnableDCache();  //刷新D-Cache	
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

void Capture_Report(struct capture *self,image_t *image)
{
    int j,i;

    UARTx.WriteByte(self->ReportUartn,0xfe,0xffffffff);
    UARTx.WriteByte(self->ReportUartn,0xef,0xffffffff);

    for(i = 0; i < image->Hight; i++)
    {
        for(j = 0; j < image->Width; j++)
        {
            if(image->Array[i][j].gray[0] == 0xfe )  //防止错误发送帧尾
            {
                image->Array[i][j].gray[0] = 0xff;
            }
            UARTx.WriteByte(self->ReportUartn,image->Array[i][j].gray[0],0xffffffff);
        }
    }

    UARTx.WriteByte(self->ReportUartn,0xef,0xffffffff);
    UARTx.WriteByte(self->ReportUartn,0xfe,0xffffffff);
}

void Capture_Show(struct capture *self,image_t *image,uint8_t flags)
{
    uint16_t color;    
    
    pixel_t pixel;
    
//    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
//        SCB_DisableDCache();
//    }
//    SCB_EnableDCache();
    
    Screen.__SetArea__(0,0,image->Width - 1,image->Hight - 1);
    
    switch(flags)
    {
      case 0:/*原图像*/
          /* 显示图像 */
        
        for(int j = 0; j < image->Hight; j++)
        {
           for(int i = 0; i < image->Width; i++) 
           {
              /* 将灰度转化为 RGB565 */
              color = 0;
              
              pixel = image->Array[j][i]; 
              
              color = (pixel.gray[0] >> 3) << 11;
              color |= (pixel.gray[0] >> 2) << 5;
              color |= pixel.gray[0] >> 3;
              
              Screen.__FastSetPixel__(color);   
              //Screen.SetPixel(Screen.Self,i,j,color);
           }
        }
        break;
        case 1:/*二值化*/
          
         for(int j = 0; j < image->Hight; j++)
         {
            for(int i = 0; i < image->Width; i++) 
            {          
                if(image->Array[j][i].binary == 1)
                  Screen.__FastSetPixel__(BLACK);  
                else
                  Screen.__FastSetPixel__(WHITE);          
            }
         }
          break;
        default:
          break;
    }
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




