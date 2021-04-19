#include "fuzzy_ctrl.h"

#include "sysmath.h"

/*Private*/
double Fuzzy_GetArray(double *Array,fuzzy_dim_t dim,uint32_t index_x,uint32_t index_y)
{
    if(index_y > dim.y || index_x > dim.x)
        return INFINITE;

    return *(Array + dim.y * index_x + index_y);
}

/*Public*/
fuzzy_ctrl_t Fuzzy_Init()
{
    fuzzy_ctrl_t fuzzy;

    

    return fuzzy;
}

fuzzy_ctrl_t CalcMembership(fuzzy_ctrl_t *fuzzy)
{
    fuzzy->Quantify(fuzzy->IFF[fuzzy->Q]);
}

fuzzy_ctrl_t *Fuzzy_Quantization(fuzzy_ctrl_t *fuzzy)
{   
    fuzzy->Q = -1;

    for(uint32_t i = 1 ; i <fuzzy->QuantifyDim; i++ )
    {
        if(fuzzy->Input <= fuzzy->IFF[i] && fuzzy->Input > fuzzy->IFF[i - 1])
        {
            fuzzy->Q = i;
        }
    }

    if(fuzzy->Input < fuzzy->IFF[0])
        fuzzy->Q = 0;

    if(fuzzy->Input > fuzzy->IFF[fuzzy->QuantifyDim - 1])
        fuzzy->Q = fuzzy->QuantifyDim;
    
    fuzzy->Pn = fuzzy->Q - (fuzzy->QuantifyDim/2);

    return fuzzy;
}

fuzzy_ctrl_t Fuzzy_Reasoning(fuzzy_ctrl_t *fuzzy)
{
    Fuzzy_Quantization(fuzzy);

    CalcMembership(fuzzy);

}

