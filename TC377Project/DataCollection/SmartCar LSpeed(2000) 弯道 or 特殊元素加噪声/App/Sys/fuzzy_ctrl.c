#include "fuzzy_ctrl.h"

#include "sysmath.h"

float FuzzyInit(Fuzzy_TypeDef *fuzzy,fuzzy_init_t *fuzzyInitStr)
{
    fuzzy->DFF = NULL;
    fuzzy->DFFLen = 0;
    fuzzy->e[0] = 0.0;
    fuzzy->e[1] = 0.0;
    fuzzy->ec = 0.0;
    fuzzy->EFF = NULL;
    fuzzy->EFFLen = 0;
    fuzzy->Rule = NULL;
    fuzzy->RuleDim.x = 0;;
    fuzzy->RuleDim.y = 0;
    fuzzy->U = 0.0;
    fuzzy->UBuf = NULL;
    fuzzy->UBufLen = 0;
    fuzzy->UFF = NULL;
    fuzzy->UFFLen = 0;



    fuzzy->DFF = fuzzyInitStr->DFF;
    fuzzy->DFFLen = fuzzyInitStr->DFFLen;
    fuzzy->EFF = fuzzyInitStr->EFF;
    fuzzy->EFFLen = fuzzyInitStr->EFFLen;
    fuzzy->Rule = fuzzyInitStr->Rule;
    fuzzy->UBuf = fuzzyInitStr->UBuf;
    fuzzy->UFF = fuzzyInitStr->UFF;
    fuzzy->UFFLen = fuzzyInitStr->UFFLen;

    fuzzy->UBufLen = fuzzy->UFFLen;
    fuzzy->RuleDim.x = fuzzy->EFFLen;
    fuzzy->RuleDim.y = fuzzy->DFFLen;

    return 0.0;
    
}

static sint32_t FuzzyGetRule(Fuzzy_TypeDef *fuzzy,uint32_t x,uint32_t y)
{
    return *(fuzzy->Rule +x * fuzzy->RuleDim.y + y);
}

float FuzzyCtrl(Fuzzy_TypeDef *fuzzy, float target, float actual)
{
    float EF[2] = { 0.0,0.0 }, DF[2] = {0.0,0.0};

    int i = 0;

    int En = 0, Dn = 0;

    uint UnBias = 0;

    uint EnBias = 0;
    uint DnBias = 0;

    UnBias = (fuzzy->UFFLen)/2;
    EnBias = (fuzzy->EFFLen)/2;
    DnBias = (fuzzy->DFFLen)/2;

    

    sint32_t Un[4];

    float UF[4];

    for (i = 0; i < fuzzy->UBufLen; i++)
    {
        fuzzy->UBuf[i] = 0.0;
    }

    /*Calculate E and Ec.*/

    fuzzy->e[0] = fuzzy->e[1];
    fuzzy->e[1] = target - actual;

    fuzzy->ec = fuzzy->e[1] - fuzzy->e[0];

    /*Calculate Membership.*/

    if (fuzzy->e[1] < fuzzy->EFF[0])
    {
        EF[0] = 1.0;
        EF[1] = 0.0;

        En = EnBias * (-1);
    }
    else if (fuzzy->e[1] >= fuzzy->EFF[fuzzy->EFFLen - 1])
    {
        EF[0] = 0.0;
        EF[1] = 1.0;
        En = EnBias - 1;
    }
    else
    {
        for (i = 1; i < fuzzy->EFFLen; i++)
        {
            if ((fuzzy->e[1] < fuzzy->EFF[i]) && (fuzzy->e[1] >= fuzzy->EFF[i - 1])) 
            {
                En = i - 1 - EnBias;

                EF[0] = -(fuzzy->e[1] - fuzzy->EFF[i]) / (fuzzy->EFF[i] - fuzzy->EFF[i - 1]);

                EF[1] = (fuzzy->e[1] - fuzzy->EFF[i - 1]) / (fuzzy->EFF[i] - fuzzy->EFF[i - 1]);
            }
        }
    }

    if (fuzzy->ec < fuzzy->DFF[0])
    {
        DF[0] = 1.0;
        DF[1] = 0.0;

        Dn = DnBias * (-1);
    }
    else if (fuzzy->ec >= fuzzy->DFF[fuzzy->DFFLen - 1])
    {
        DF[0] = 0.0;
        DF[1] = 1.0;
        Dn = DnBias - 1;
    }
    else
    {
        for (i = 1; i < fuzzy->DFFLen; i++)
        {
            if (fuzzy->ec < fuzzy->DFF[i] && fuzzy->ec >= fuzzy->DFF[i - 1]) //ÓÐ2Ìõ¹æÔòÉúÐ§
            {
                Dn = i - 1 - DnBias;

                DF[0] = -(fuzzy->ec - fuzzy->DFF[i]) / (fuzzy->DFF[i] - fuzzy->DFF[i - 1]);

                DF[1] = (fuzzy->ec - fuzzy->DFF[i - 1]) / (fuzzy->DFF[i] - fuzzy->DFF[i - 1]);
            }
        }
    }

    /*Fuzzy Reasoning.*/

    Un[0] = FuzzyGetRule(fuzzy,En + EnBias,Dn + DnBias) + UnBias;   

    UF[0] = 1.0 * min(EF[0], DF[0]);

    Un[1] = FuzzyGetRule(fuzzy,En + EnBias + 1,Dn + DnBias) + UnBias; 
    UF[1] = 1.0 * min(EF[1], DF[0]);

    Un[2] = FuzzyGetRule(fuzzy,En + EnBias,Dn + DnBias + 1) + UnBias; 
    UF[2] = 1.0 * min(EF[0], DF[1]);

    Un[3] = FuzzyGetRule(fuzzy,En + EnBias + 1,Dn + DnBias + 1) + UnBias; 
    UF[3] = 1.0 * min(EF[1], DF[1]);

    if (Un[0] == Un[1])    
    {
        if (UF[0] > UF[1])
            UF[1] = 0;
        else
            UF[0] = 0;
    }
    if (Un[0] == Un[2])
    {
        if (UF[0] > UF[2])
            UF[2] = 0;
        else
            UF[0] = 0;
    }
    if (Un[0] == Un[3])
    {
        if (UF[0] > UF[3])
            UF[3] = 0;
        else
            UF[0] = 0;
    }

    if (Un[1] == Un[2])
    {
        if (UF[1] > UF[2])
            UF[2] = 0;
        else
            UF[1] = 0;
    }
    if (Un[1] == Un[3])
    {
        if (UF[1] > UF[3])
            UF[3] = 0;
        else
            UF[1] = 0;
    }

    if (Un[2] == Un[3])
    {
        if (UF[2] > UF[3])
            UF[3] = 0;
        else
            UF[2] = 0;
    }

    for (i = 0; i < 4; i++)
    {
        fuzzy->UBuf[Un[i]] += UF[i];
    }

    /*Anti Fuzzy(Gravity Method)*/

    float sum0 = 0.0;
    float sum1 = 0.0;

    for (i = 0; i < fuzzy->UFFLen; i++)
    {
        sum0 += fuzzy->UBuf[i] * fuzzy->UFF[i];

        sum1 += fuzzy->UBuf[i];
    }

    fuzzy->U = fdiv(sum0,sum1);

    return fuzzy->U;
}

