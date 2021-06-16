#include "fuzzycontrol.h"

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

float SUFF[13] = {-1.0,-0.8,-0.6,-0.4,-0.2,-0.1,0.0,0.1,0.2,0.4,0.6,0.8,1.0};

sint32_t LABMSFuzzyRule[7][7] =
{
                    {-6,-6,-6,-6,-5,-5,-4},
                    {-6,-5,-5,-4,-3,-3,-2},//
                    {-5,-4,-3,-2, 0, 2, 2},
                    {-4,-3,-1, 0, 1, 3, 4},
                    {-2,-2, 0, 2, 3, 4, 5},
                    {2, 3, 3, 4, 5, 5, 6},//
                    {4, 5, 5, 6, 6, 6, 6},
};

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

float FuzzyControl(fuzzy_ctrl_t *fuzzy, float target, float actual)
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


//float FuzzySpeedTable[13][13] =
//{
//        {1326,1450,1450,1450,1650,1650,1700,1700,1678,1427,1427,1427,1427,},
//        {1424,1600,1600,1600,1818,1818,1857,1857,1804,1723,1723,1723,1661,},
//        {1424,1600,1600,1600,1818,1818,1857,1857,1804,1723,1723,1723,1661,},
//        {1424,1600,1600,1600,1818,1818,1857,1857,1804,1723,1723,1723,1661,},
//        {1518,1818,1818,1818,1964,1964,2135,2135,1973,1805,1805,1805,1735,},
//        {1518,1818,1818,1818,1964,1964,2135,2135,1973,1805,1805,1805,1735,},
//        {1553,2029,2029,2029,2268,2268,2350,2350,2300,1963,1963,1963,1535,},
//        {1553,2029,2029,2029,2268,2268,2350,2350,2300,1963,1963,1963,1535,},
//        {1735,1843,1843,1843,2030,2030,2200,2200,1980,1780,1780,1780,1533,},
//        {1650,1711,1711,1711,1780,1780,1827,1827,1791,1500,1500,1500,1400,},
//        {1650,1711,1711,1711,1780,1780,1827,1827,1791,1500,1500,1500,1400,},
//        {1650,1711,1711,1711,1780,1780,1827,1827,1791,1500,1500,1500,1400,},
//        {1450,1450,1450,1450,1650,1650,1700,1700,1678,1400,1400,1400,1333,},
//};

float FuzzySpeedTable[13][13] =
{
        {1826,1950,1950,1950,2150,2150,2200,2200,2178,1927,1927,1927,1927,},
        {1924,2100,2100,2100,2318,2318,2357,2357,2304,2223,2223,2223,2161,},
        {1924,2100,2100,2100,2318,2318,2357,2357,2304,2223,2223,2223,2161,},
        {1924,2100,2100,2100,2318,2318,2357,2357,2304,2223,2223,2223,2161,},
        {2018,2318,2318,2318,2464,2464,2635,2635,2473,2305,2305,2305,2235,},
        {2018,2318,2318,2318,2464,2464,2635,2635,2473,2305,2305,2305,2235,},
        {2053,2529,2529,2529,2768,2768,2850,2850,2800,2463,2463,2463,2035,},
        {2053,2529,2529,2529,2768,2768,2850,2850,2800,2463,2463,2463,2035,},
        {2235,2343,2343,2343,2530,2530,2700,2700,2480,2280,2280,2280,2033,},
        {2150,2211,2211,2211,2280,2280,2327,2327,2291,2000,2000,2000,1900,},
        {2150,2211,2211,2211,2280,2280,2327,2327,2291,2000,2000,2000,1900,},
        {2150,2211,2211,2211,2280,2280,2327,2327,2291,2000,2000,2000,1900,},
        {1950,1950,1950,1950,2150,2150,2200,2200,2178,1900,1900,1900,1833,},
};

float FuzzySpeedControl(Fuzzy_TypeDef *fuzzy,float target,float actual)
{
    fuzzy->e[0] = fuzzy->e[1];
    fuzzy->e[1] = target - actual;

    fuzzy->ec = fuzzy->e[1] - fuzzy->e[0];


    int En = 0, Dn = 0;
    float targerSpeed;
    float ke = 16.7,kec = 16.7;

    float e = fuzzy->e[1];
    float ec = fuzzy->ec;

    if (e > 0) {

        En = (int)(e / ke + 0.5);
    }
    else {

        En = (int)(e / ke - 0.5);
    }

    if (ec > 0) {

        Dn = (int)(ec / kec + 0.5);
    }
    else {
        Dn = (int)(ec / kec - 0.5);
    }

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
float MEFF_Kp[7] = {-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0};
float MDFF_Kp[7] = {-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0};
float MUFF_Kp[7] = {-2.7,-1.8,-0.9,0,0.9,1.8,2.7};
sint32_t MFuzzyRule_Kp[7][7] = 
{
    {3,3,2,2,1,0,0},
    {3,3,2,1,1,0,-1},
    {2,2,2,1,0,-1,-1},
    {2,2,1,0,-1,-2,-2},
    {1,1,0,-1,-1,-2,-2},
    {1,0,-1,-2,-2,-2,-3},
    {0,0,-2,-2,-2,-3,-3},
};

float MEFF_Ki[7] = {-75.0,-50.0,-25.0,0.0,25.0,50.0,75.0};
float MDFF_Ki[7] = {-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0};
float MUFF_Ki[7] = {-0.2,-0.1,-0.05,0,0.05,0.1,0.2};

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

float FuzzyPID(Fuzzy_TypeDef *pFuzzy,Fuzzy_TypeDef *iFuzzy,float target,float actual)
{
    FuzzyCtrl(pFuzzy,target,actual);
    FuzzyCtrl(iFuzzy,target,actual);
    return 0.0;
}





















// void Fuzzy_CalculateMatrixR()
// {
//     uint32_t k = 0;

//     uint32_t U_Index = 0;

//     for(int i = 0 ; i < 169 ;i++)
//     {
//         for(int j = 0 ; j <13 ;j++)
//         {
//             R[i][j] = 0.0;
//         }
//     }

//     for(int e_index = 0 ; e_index < 7 ; e_index++)
//     {
//         for(int ec_index = 0 ; ec_index < 7 ;ec_index++) /*Calculae U(E(i) X EC(j)) for i , j = 1,2...7*/
//         {
//             U_Index = FuzzyRule[e_index][ec_index] + 3;

//             for(int i = 0 ; i < 13 ;i++)
//             {
//                 for(int j = 0 ; j < 13 ;j++)
//                 {
//                     if(E_MemberShipR[e_index][i] > EC_MemberShipR[ec_index][j])
//                         R1[i][j] = EC_MemberShipR[ec_index][j];
//                     else
//                         R1[i][j] = E_MemberShipR[e_index][i];

//                     R2[k++] = R1[i][j];      
//                 }
//             }

//             for(int i = 0 ; i < 163 ;i++)
//             {
//                 for(int j = 0 ; j < 13 ;j++)
//                 {
//                     if(R2[i] > U_MemberShipR[U_Index][j])
//                         R3[i][j] = U_MemberShipR[U_Index][j];
//                     else
//                         R3[i][j] = R2[j];

//                     if(R3[i][j] > R[i][j])
//                         R[i][j] = R3[i][j];
//                 }
//             }

//         }
//     }
// }

//  void Fuzzy_Ctrl(fuzzy_ctrl_t *fuzzy,float target,float actual)
//  {
//       /*Calculate Error and Delta Error.*/
//       fuzzy->e[0] = fuzzy->e[1];
//       fuzzy->e[1] = target - actual;
      
//       fuzzy->ec = fuzzy->e[1] - fuzzy->e[0];

//       /*Calculate MemberShip.*/

//       for(int i = 0 ; i < 13 ; i++)
//       {
//           E_MemberShip[i] = 0.0;
//           EC_MemberShip[i] = 0.0;
//       }

//       if(fuzzy->e[1] < EFF[0])
//       {
//           E_MemberShip[0] = 1.0;
//           E_MemberShip[1] = 0.0;      
//       }
//       else if(fuzzy->e[1] > EFF[12])
//       {
//           E_MemberShip[12] = 1.0;
//           E_MemberShip[11] = 0.0;    
//       }
//       else
//       {
//           for(int i = 1 ; i < 12 ; i++)
//           {
//               if(fuzzy->e[1] <= EFF[i] && fuzzy->e[1] >= EFF[i - 1])
//               {
//                   E_MemberShip[i] =(-fuzzy->e[1] + EFF[i])/(EFF[i] - EFF[i-1]);
//                   E_MemberShip[i - 1] = (1.0 - E_MemberShip[i])/2.0;
//                   E_MemberShip[i+i] = (1.0 - E_MemberShip[i])/2.0;
//               }
//           }         
//       }

//       if(fuzzy->ec < DFF[0])
//       {
//           EC_MemberShip[0] = 1.0;
//           EC_MemberShip[1] = 0.0;             
//       }
//       else if(fuzzy->ec > DFF[12])
//       {
//           EC_MemberShip[12] = 1.0;
//           EC_MemberShip[11] = 0.0;          
//       }
//       else
//       {
//           for(int i = 1 ; i < 12 ; i++)
//           {
//               if(fuzzy->ec <= DFF[i] && fuzzy->ec >= DFF[i - 1])
//               {
//                   EC_MemberShip[i] =(-fuzzy->e[1] + DFF[i])/(DFF[i] - DFF[i-1]);
//                   EC_MemberShip[i - 1] = (1.0 - EC_MemberShip[i])/2.0;
//                   EC_MemberShip[i+i] = (1.0 - EC_MemberShip[i])/2.0;
//               }       
//           }
//       }

//       /*Fuzzy Reasoning.*/

//       uint32_t k = 0;

//       for(int i = 0 ; i < 13 ; i++)                   //����ģ������ E_MemberShip[13] �� EC_MemberShip[13] �� �ѿ���������ת��1ά����ECC[169]��
//       {
//           for(int j = 0 ; j < 13 ; j++)
//           {
//               if(E_MemberShip[i] < EC_MemberShip[j])  //ȡС����
//                   EEC[k] = E_MemberShip[i];         
//               else
//                   EEC[k] = EC_MemberShip[j];
              
//               k++;
//           }
//       }

//       float temp = 0.0;

//       for(int i = 0 ; i < 16 ;i++)
//           Result[i] = 0.0;

//       /*Calculate ECC[169] X R[169][13]*/  
//       for(int i = 0 ; i < 169 ;i++)         
//       {
//           for(int j = 0 ; j < 13;j++)   //*ȡС +ȡ��
//           {
//               if(EEC[i] > R[i][j])
//                 temp = R[i][j];
//               else
//                 temp = EEC[i];
              
//               if(temp > Result[j])
//                   Result[j] = temp;
//           }
//       }

//       /*��ģ��(���ķ�)*/

//       fuzzy->U = 0.0;

//       float sum0 = 0.0;
//       float sum1 = 0.0;

//       for(int i = 0 ; i < 13 ; i++)
//       {
//           sum0 += Result[i] * UFF[i];
//           sum1 += Result[i];
//       }

//       fuzzy->U = sum0/sum1;
//  }
