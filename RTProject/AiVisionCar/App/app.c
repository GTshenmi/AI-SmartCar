/*
 * app.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 *  @Brief:
 *      This file is the top file of the whole project.
 *      function in this file is to be executed in the main function or the interrupt function.
 */
#include <app.h>
#include "include.h"

void Key_PressedCallBack(struct key *self,void *argv,uint16_t argc)
{
      for(int i = 0 ; i < 5 ; i ++)
      {
          if(self == KEY[i].Self)
              Screen.WriteXLine(Screen.Self,3,"KEY[%d] Pressed.",i);
      }
}
void CaptureDemo1()
{
    Capture.Init(Capture.Self,100);
    
    while (1)
    {   
        Capture.Start(Capture.Self);
        RLED.Toggle(RLED.Self);
        
        Capture.Read(Capture.Self,0);
        Capture.Stop(Capture.Self);
        
        Capture.Show(Capture.Self,Capture.ImageCache,0);
    }  
}

void CaptureDemo2()
{
    Capture.Init(Capture.Self,100);
    Capture.Start(Capture.Self);
    
    while (1)
    {            
        Capture.Read(Capture.Self,1);
        
        while(Capture.GetState(Capture.Self) != Capture_Fin); 
        
        Capture.Show(Capture.Self,Capture.ImageCache,0);
        
        Capture.ClearFinFlag(Capture.Self);
    }
}

void CaptureDemo3()
{
    Capture.Init(Capture.Self,100);
    Capture.Start(Capture.Self);
    
    while (1)
    {            
        Capture.Read(Capture.Self,1);
    }
    
    
    /*
        In Other Function,Such as Interrupt:
        if(Capture.GetState(Capture.Self) == Capture_Fin);
        {
             Image Process.....
             Capture.Show(Capture.Self,Capture.ImageCache,0);
             Capture.ClearFinFlag(Capture.Self);
        }
    */
}
/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
    RLED.ON(RLED.Self);
    
    Screen.Init(Screen.Self);
    
    for(int i = 0 ; i < 5 ; i ++)
    {  
        KEY[i].Init(KEY[i].Self);
        KEY[i].PressedCallBack = Key_PressedCallBack;
    }
    CaptureDemo1();
    
    while(1);
    
}
