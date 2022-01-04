#include <iocc2530.h>
#include "hal_mcu.h"
#include "hal_assert.h"
#include "hal_board.h"
#include "dht11.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include <stdio.h>


#define RF_CHANNEL            25      // 2.4 GHz RF channel


#define PAN_ID                0x2007
#define SEND_ADDR             0x2530
#define RECV_ADDR             0x2520

#define NODE_TYPE             0         //0:接收节点，！0：发送节点


#define COMMAND               0x10


void io_init(void)
{
  P0SEL &= ~0x02;
  P0DIR &= ~0x02;
}

static basicRfCfg_t basicRfConfig;

int ledstatus = 0;
int tempstate = 0;

void rfSendData(void)
{
    uint8 pTxData[] = {COMMAND};
    uint8 key1;
    
    // Keep Receiver off when not needed to save power
    basicRfReceiveOff();
    
    key1 = P0_1;
   
    // Main loop
    while (TRUE) {
      dht11_update();
      if (P0_1==0 && key1!=0) {
        
        hal_led_on(1);
        basicRfSendPacket(RECV_ADDR, pTxData, sizeof pTxData);       
        hal_led_off(1);
      }
      if(dat3>=28)
      {
        if(tempstate == 0)
        {
          tempstate = 1;
          hal_led_on(1);
          basicRfSendPacket(RECV_ADDR, pTxData, sizeof pTxData);       
          hal_led_off(1);
        }
      }
      else
      {
        if(tempstate == 1)
        {
          tempstate = 0;
          hal_led_on(1);
          basicRfSendPacket(RECV_ADDR, pTxData, sizeof pTxData);       
          hal_led_off(1);
        }
        
      }
        key1 = P0_1;
     
        
        halMcuWaitMs(50);
    }
}

void rfRecvData(void)
{
  uint8 pRxData[128];
  int rlen;
  
   basicRfReceiveOn();

    // Main loop
    while (TRUE) {
        while(!basicRfPacketIsReady());
        rlen = basicRfReceive(pRxData, sizeof pRxData, NULL);
        if(rlen > 0 && pRxData[0] == COMMAND) {
          if (ledstatus == 0) {
            P1_1=0;
            ledstatus = 1;
          } else {
            P1_1=1;
             ledstatus = 0;
          }
        }
    }
}

void main(void)
{
    halMcuInit();

    io_init();
    
    hal_led_init();
    
    dht11_io_init(); 
    
    hal_uart_init();
    
//    P1_1=0;
    
    if (FAILED == halRfInit()) {
        HAL_ASSERT(FALSE);
    }

    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
#ifdef SECURITY_CCM
    basicRfConfig.securityKey = key;
#endif

    
    // Initialize BasicRF
#if NODE_TYPE
    basicRfConfig.myAddr = SEND_ADDR;
#else
    basicRfConfig.myAddr = RECV_ADDR; 
#endif
    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
#if NODE_TYPE
  rfSendData();
#else
  rfRecvData();   
#endif
}



/*------------------------------------------------------
【平    台】龙邱i.MX RT1064核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2019年3月12日
【dev.env.】IAR8.30.1及以上版本
【Target 】 i.MX RT1064
【Crystal】 24.000Mhz
【ARM PLL】 1200MHz
【SYS PLL】 600MHz
【USB PLL】 480MHz
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
--------------------------------------------------------*/

#include "include.h"
 
/*!
 * @brief Main function 
 */
int main(void)
{  
    Core0_Init();
    Core0_Main();
}
