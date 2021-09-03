#include "fuzzycontrol.h"
#include "include.h"
/*Servo Fuzzy Control*/
//float SEFF[7] = {-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0};
//
//float SDFF[7] = {-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0};
//
//float SUFF[13] = {-1.0,-0.8,-0.6,-0.4,-0.2,-0.1,0.0,0.1,0.2,0.4,0.6,0.8,1.0};
//
//sint32_t SFuzzyRule[7][7] =
//{
//        /*ec-----------------*/ /*e*/
//        {-6,-6,-6,-6,-5,-5,-4,},/*|*/
//        {-5,-5,-5,-4,-4,-3,-1,},/*|*/
//        {-4,-4,-3,-1, 0, 1, 2,},/*|*/
//        {-4,-3,-1, 0, 1, 3, 4,},/*|*/
//        {-2,-1, 0, 1, 3, 4, 4,},/*|*/
//        { 1, 3, 4, 4, 5, 5, 5,},/*|*/
//        { 4, 5, 5, 6, 6, 6, 6}, /*|*/
//};

float SEFF[7] = {-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0};

float SDFF[7] = {-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0};

//float SDFF[7] = {-40.0,-20.0,-10.0,0.0,10.0,20.0,40.0};

//float SUFF[13] = {-1.0,-0.8,-0.6,-0.4,-0.2,-0.1,0.0,0.1,0.2,0.4,0.6,0.8,1.0};
float SUFF[13] = {-1.0,-0.8,-0.6,-0.32,-0.18,-0.08,0.0,0.08,0.18,0.32,0.6,0.8,1.0};

//sint32_t LABMSFuzzyRule[7][7] =
//{
//                    {-6,-6,-6,-6,-5,-5,-4},
//                    {-6,-5,-5,-4,-3,-3,-2},//
//                    {-5,-4,-3,-2, 0, 2, 2},
//                    {-4,-3,-1, 0, 1, 3, 4},
//                    {-2,-2, 0, 2, 3, 4, 5},
//                    {2, 3, 3, 4, 5, 5, 6},//
//                    {4, 5, 5, 6, 6, 6, 6},
//};

sint32_t LABMSFuzzyRule[7][7] =
{
    {-6,-6,-6,-6,-4,-4,-3},
    {-6,-5,-5,-4,-3,-2,-1},
    {-5,-4,-3,-1, 0, 1, 2},
    {-4,-3,-1, 0, 1, 3, 4},
    {-2,-1, 0, 1, 3, 4, 5},
    {1, 2, 3, 4, 5, 5, 6},
    {3, 4, 4, 6, 6, 6, 6},
};

//sint32_t LABMSFuzzyRule[7][7] =
//{
//                    {-6,-6,-6,-5,-4,-4,-3},
//                    {-5,-5,-4,-4,-3,-2,-1},//
//                    {-5,-3,-2,-1, 0, 1, 2},
//                    {-4,-3,-1, 0, 1, 3, 4},
//                    {-2,-1, 0, 1, 2, 3, 5},
//                    {1, 2, 3, 4, 4, 5, 5},//
//                    {3, 4, 4, 5, 6, 6, 6},
//};

sint32_t SABMSFuzzyRule[7][7] =
{
                    {-6,-6,-6,-6,-5,-5,-4},
                    {-6,-5,-5,-4,-3,-3,-2},//
                    {-6,-4,-2,-1, 0, 1, 2},
                    {-5,-3,-1, 0, 1, 3, 5},
                    {-2,-1, 0, 1, 2, 4, 6},
                    {2, 3, 3, 4, 5, 5, 6},//
                    {4, 5, 5, 6, 6, 6, 6},
};

sint32_t SFuzzyRule[7][7];

//sint32_t SFuzzyRule[7][7] =
//{
//                    {-6,-6,-6,-6,-5,-5,-4},
//                    {-6,-5,-5,-4,-3,-3,-2},//
//                    {-5,-4,-3,-1, 0, 1, 2},
//                    {-4,-3,-1, 0, 1, 3, 4},
//                    {-2,-1, 0, 1, 3, 4, 5},
//                    {2, 3, 3, 4, 5, 5, 6},//
//                    {4, 5, 5, 6, 6, 6, 6},
//};

//SEFF = {-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0};
//
//SDFF = [-15,-10.0,-5.0,0.0,5.0,10.0,15.0];
//
//SUFF = [-0.8,-0.6,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.6,0.8];
//
//            %--------EC---------
//SFuzzyRule ={
//            {-6,-6,-6,-6,-5,-4,-3},%e
//            {-6,-5,-4,-3,-3,-2,-1},% |
//            {-5,-4,-3,-1, 0, 1, 2},% |
//            {-4,-3,-1, 0, 1, 3, 4},
//            {-2,-1, 0, 1, 3, 4, 5},
//            {1, 2, 3, 3, 4, 5, 6},
//            {3, 4, 5, 6, 6, 6, 6]}},

float FuzzyControl(Fuzzy_TypeDef *fuzzy, float target, float actual)
{
    float EF[2] = { 0.0,0.0 }, DF[2] = {0.0,0.0};

    int i = 0;

    int En = 0, Dn = 0;

    sint32_t Un[4];

    float UF[4];

    float U[13];

    for (i = 0; i < 13; i++)
    {
        U[i] = 0.0;
    }

    /*Calculate E and Ec.*/

    fuzzy->e[0] = fuzzy->e[1];
    fuzzy->e[1] = target - actual;

    fuzzy->ec = fuzzy->e[1] - fuzzy->e[0];

    /*Calculate Membership.*/

    if (fuzzy->e[1] < SEFF[0])
    {
        EF[0] = 1.0;
        EF[1] = 0.0;

        En = -3;
    }
    else if (fuzzy->e[1] >= SEFF[6])
    {
        EF[0] = 0.0;
        EF[1] = 1.0;
        //En = 3;
        En = 2;
    }
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if ((fuzzy->e[1] < SEFF[i]) && (fuzzy->e[1] >= SEFF[i - 1])) //ÓÐ2Ìõ¹æÔòÉúÐ§
            {
                En = i - 1 - 3;

                EF[0] = -(fuzzy->e[1] - SEFF[i]) / (SEFF[i] - SEFF[i - 1]);

                EF[1] = (fuzzy->e[1] - SEFF[i - 1]) / (SEFF[i] - SEFF[i - 1]);
            }
        }
    }

    if (fuzzy->ec < SDFF[0])
    {
        DF[0] = 1.0;
        DF[1] = 0.0;

        Dn = -3;
    }
    else if (fuzzy->ec >= SDFF[6])
    {
        DF[0] = 0.0;
        DF[1] = 1.0;
        //Dn = 3;
        Dn = 2;
    }
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if (fuzzy->ec < SDFF[i] && fuzzy->ec >= SDFF[i - 1]) //ÓÐ2Ìõ¹æÔòÉúÐ§
            {
                Dn = i - 1 - 3;

                DF[0] = -(fuzzy->ec - SDFF[i]) / (SDFF[i] - SDFF[i - 1]);

                DF[1] = (fuzzy->ec - SDFF[i - 1]) / (SDFF[i] - SDFF[i - 1]);
            }
        }
    }

    /*Fuzzy Reasoning.*/

    Un[0] = SFuzzyRule[En + 3][Dn + 3] + 6;      //2*2 = 4Ìõ¹æÔòÉúÐ§

    UF[0] = min(EF[0], DF[0]);

    Un[1] = SFuzzyRule[En + 3 + 1][Dn + 3] + 6;
    UF[1] = min(EF[1], DF[0]);

    Un[2] = SFuzzyRule[En + 3][Dn + 3 + 1] + 6;
    UF[2] = 1.0 * min(EF[0], DF[1]);

    Un[3] = SFuzzyRule[En + 3 + 1][Dn + 3 + 1] + 6;
    UF[3] = 1.0 * min(EF[1], DF[1]);

    if (Un[0] == Un[1])    //Í¬Á¥Êô¶ÈÓïÑÔÈ¡´ó
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
        U[Un[i]] += UF[i];
    }

    /*Anti Fuzzy(Gravity Method)*/

    float sum0 = 0.0;
    float sum1 = 0.0;

    for (i = 0; i < 13; i++)
    {
        sum0 += U[i] * SUFF[i];

        sum1 += U[i];
    }

    if (fabs(sum1) < 1e-6)
        fuzzy->U = 0.0;
    else
        fuzzy->U = sum0 / sum1;

    return fuzzy->U;
}

float FuzzySpeedTable[13][13] =
{
        {2013,2112,2112,2112,2263,2263,2300,2300,2283,2093,2093,2093,2093,},
        {2093,2340,2340,2340,2473,2473,2455,2455,2340,2271,2271,2271,2271,},
        {2093,2340,2340,2340,2473,2473,2455,2455,2340,2271,2271,2271,2271,},
        {2093,2340,2340,2340,2473,2473,2455,2455,2340,2271,2271,2271,2271,},
        {2185,2471,2471,2471,2900,2900,2600,2600,2443,2442,2442,2442,2347,},
        {2185,2471,2471,2471,2900,2900,2600,2600,2443,2442,2442,2442,2347,},
        {2221,2688,2688,2688,3048,3048,3010,3010,3100,2605,2605,2605,2200,},
        {2221,2688,2688,2688,3048,3048,3010,3010,3100,2605,2605,2605,2200,},
        {2347,2442,2442,2442,2467,2467,2657,2657,2962,2420,2420,2420,2194,},
        {2263,2263,2263,2263,2328,2328,2443,2443,2423,2265,2265,2265,2071,},
        {2263,2263,2263,2263,2328,2328,2443,2443,2423,2265,2265,2265,2071,},
        {2263,2263,2263,2263,2328,2328,2443,2443,2423,2265,2265,2265,2071,},
        {2112,2112,2112,2112,2263,2263,2300,2300,2283,2071,2071,2071,2017,},
};

sint32_t MFuzzyRule[7][7] =
{
        {-6,-6,-6,-5,-4,-4,-3},
        {-5,-5,-4,-4,-3,-2,-1},//
        {-5,-3,-2,-1, 0, 1, 2},
        {-4,-3,-1, 0, 1, 3, 4},
        {-2,-1, 0, 1, 2, 3, 5},
        {1, 2, 3, 4, 4, 5, 5},//
        {3, 4, 4, 5, 6, 6, 6},
};

float MEFF[7] = {-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0};

float MDFF[7] = {-40.0,-20.0,-10.0,0.0,10.0,20.0,40.0};

float MUFF[13] = { 2000,2200,2400,2600,2800,3000,3200,3000,2800,2600,2400,2200,2000 };

float FuzzySpeedControl(Fuzzy_TypeDef *fuzzy,float target,float actual)
{
    float EFF[13] = {-90.0,-80.0,-70.0,-60.0,-30.0,-20.0,0.0,20.0,30.0,60.0,70.0,80.0,90.0};

    float DFF[13] = {-60.0,-45.0,-30.0,-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0,30.0,45.0,60.0};

    int En = 0, Dn = 0;

    float E = 0.0,D = 0.0;
    float targerSpeed;

    //float ke = 14.286,kec = 6.67;

    float e = fuzzy->e[1];
    float ec = fuzzy->ec;

    if (e < EFF[0])
    {
        E = -6;
    }
    else if (e >= EFF[6])
    {
        E = 6;
    }
    else
    {
        for (int i = 1; i <= 13; i++)
        {
            if ((e < EFF[i]) && (e >= EFF[i - 1]))
            {
                E = i - 1 - 6 + (e - EFF[i - 1])/(EFF[i] - EFF[i - 1]);
            }
        }
    }

    if (ec < DFF[0])
    {
        D = -6;
    }
    else if (ec >= DFF[6])
    {
        D = 6;
    }
    else
    {
        for (int i = 1; i <= 13; i++)
        {
            if ((ec < DFF[i]) && (ec >= DFF[i - 1]))
            {
                D = i - 1 - 6 + (e - DFF[i - 1])/(DFF[i] - DFF[i - 1]);
            }
        }
    }

    if (E > 0) {
        //En = (int)(e / ke + 0.5);
        En = (int)(E + 0.5);
    }
    else {
        //En = (int)(e / ke - 0.5);
        En = (int)(E - 0.5);
    }

    if (D > 0) {
        //Dn = (int)(ec / kec + 0.5);
        Dn = (int)(D + 0.5);
    }
    else {
        //Dn = (int)(ec / kec - 0.5);
        Dn = (int)(D - 0.5);
    }

    //En = 0,Dn = 0;

    if (En > 6)
        En = 6;
    else if (En < -6)
        En = -6;

    if (Dn > 6)
        Dn = 6;
    else if (Dn < -6)
        Dn = -6;

    targerSpeed = (FuzzySpeedTable[En + 6][Dn + 6]);

    return targerSpeed;

}

/*Motor Fuzzy Control*/
float MEFF_Kp[7] = {-30.0,-20.0,-10.0,0.0,10.0,20.0,30.0};
float MDFF_Kp[7] = {-30.0,-20.0,-10.0,0.0,10.0,20.0,30.0};
float MUFF_Kp[7] = {-0.3,-0.2,-0.1,0,0.1,0.2,0.3};

sint32_t MFuzzyRule_Kp[7][7] = 
{
     /*Ec*/
  {3,3,3,3,3,3,3}, /*E*/
  {3,3,3,3,3,2,2},
  {2,2,2,2,2,1,1},
  {1,1,1,0,1,1,1},
  {0,1,2,2,2,2,2},/*E = 1*/
  {3,3,3,3,3,3,3},/*E = 2*/
  {3,3,3,3,3,3,3},/*E = 3*/
};
//
//float MEFF_Ki[7] = {-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0};
//float MDFF_Ki[7] = {-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0};
//float MUFF_Ki[7] = {-0.2,-0.1,-0.05,0,0.05,0.1,0.2};

float MEFF_Ki[7] = {-30.0,-20.0,-10.0,0.0,10.0,20.0,30.0};
float MDFF_Ki[7] = {-30.0,-20.0,-10.0,0.0,10.0,20.0,30.0};
float MUFF_Ki[7] = {-0.8,-0.4,-0.2,0,0.2,0.4,0.8};

sint32_t MFuzzyRule_Ki[7][7] = 
{
    {-3,-3,-2,-2,-1,0,0},
    {-3,-3,-2,-1,-1,0,0},
    {-3,-2,-1,-1,0,1,1},
    {-2,-2,-1,0,1,2,2},
    {-2,-1,0,1,1,2,3},
    {0,0,1,1,2,3,3},
    {0,0,1,2,2,3,3},
};

float MUBuf_Kp[7];
float MUBuf_Ki[7];

float FuzzyPIDInit(Fuzzy_TypeDef *pFuzzy,Fuzzy_TypeDef *iFuzzy)
{
    fuzzy_init_t fuzzyInitStr;

    fuzzyInitStr.DFF = MDFF_Kp;
    fuzzyInitStr.DFFLen = 7;
    fuzzyInitStr.EFF = MEFF_Kp;
    fuzzyInitStr.EFFLen = 7;
    fuzzyInitStr.UBuf = MUBuf_Kp;
    fuzzyInitStr.UFF = MUFF_Kp;
    fuzzyInitStr.UFFLen = 7;
    fuzzyInitStr.Rule = &MFuzzyRule_Kp[0][0];

    FuzzyInit(pFuzzy,&fuzzyInitStr);

    fuzzyInitStr.DFF = MDFF_Ki;
    fuzzyInitStr.DFFLen = 7;
    fuzzyInitStr.EFF = MEFF_Ki;
    fuzzyInitStr.EFFLen = 7;
    fuzzyInitStr.UBuf = MUBuf_Ki;
    fuzzyInitStr.UFF = MUFF_Ki;
    fuzzyInitStr.UFFLen = 7;
    fuzzyInitStr.Rule = &MFuzzyRule_Ki[0][0];

    FuzzyInit(iFuzzy,&fuzzyInitStr);

    return 0.0;
}

float FuzzyPIDCtrl(Fuzzy_TypeDef *pFuzzy,Fuzzy_TypeDef *iFuzzy,float target,float actual)
{
    FuzzyCtrl(pFuzzy,target,actual);
    FuzzyCtrl(iFuzzy,target,actual);
    return 0.0;
}
