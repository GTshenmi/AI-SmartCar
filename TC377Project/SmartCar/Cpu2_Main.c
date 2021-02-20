#include "include.h"

void core2_main(void)
{
    os.core[2].HardWareInit = Core2_HardWareInit;
    os.core[2].Run = Core2_Main;
    os.core[2].SoftWareInit = Core2_SoftWareInit;

    os.init(2);

    os.core[2].Run();

    while(1);
}
