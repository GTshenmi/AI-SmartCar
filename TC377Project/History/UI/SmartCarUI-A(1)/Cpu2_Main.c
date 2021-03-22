#include "include.h"

void Key2_PressedCallBack(struct key *self,void *argv,uint16_t argc)
{
    for(int i = 0;i<4;i++)
    {
        if(self == KEY[i].Self)
            CUART.WriteLine(CUART.Self,0,"KEY[%d] Pressed",i);
    }
}

void core2_main(void)
{
    os.core[2].HardWareInit = Core2_HardWareInit;
    os.core[2].Run = Core2_Main;
    os.core[2].SoftWareInit = Core2_SoftWareInit;

    os.init(2);

    os.core[2].Run();

    while(1);
}
