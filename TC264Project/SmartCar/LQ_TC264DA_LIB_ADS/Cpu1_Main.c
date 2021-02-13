#include "include.h"

int core1_main (void)
{
    Start1:
    os.core[1].HardWareInit = Core1_HardWareInit;
    os.core[1].Run = Core1_Main;
    os.core[1].SoftWareInit = Core1_SoftWareInit;

    os.init(1);

    os.core[1].Run();

    while(1)
    {
        goto Start1;
    }

    return 0;
}
