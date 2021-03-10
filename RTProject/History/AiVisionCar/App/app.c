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

/*
 * @Brief:CPU0 Main Func
 *  This Core is for Control and Data Process.
 * */
void Core0_Main()
{
      RLED.ON(RLED.Self);
    
//    for(int i = 0 ; i < 5 ; i ++)
//    {
//        KEY[i].Init(KEY[i].Self);
//        KEY[i].PressedCallBack =  Key_PressedCallBack;
//    }
    
    //RLED.OFF(RLED.Self);
    
      Screen.Test(Screen.Self);
      
      while(1)
      {
          RLED.Toggle(RLED.Self);
          os.time.delay(1.0,s);
      }
//   while(1)
//   {
//      sint16_t lspeed,rspeed;
//      
//      lspeed = LMotor.GetSpeed(LMotor.Self);
//      rspeed = RMotor.GetSpeed(RMotor.Self);
//      
//      Screen.WriteXLine(Screen.Self,0,"lspeed:%d",lspeed);
//      Screen.WriteXLine(Screen.Self,1,"rspeed:%d",rspeed);
//     
//      LMotor.SetSpeed(LMotor.Self,2000);
//      RMotor.SetSpeed(RMotor.Self,4000);
//      
//      for(int i = 0 ; i < 5 ; i ++)
//        KEY[i].Scan(KEY[i].Self);
//      
//      os.time.delay(20,ms);
//   }
}
