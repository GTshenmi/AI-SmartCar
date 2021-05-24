#include "fuzzycontrol.h"

/*
 *
 *
 * @TODO:完成连续论域的模糊控制
 *
 * 
 * 
 */

float EFF[13] = {-100.0,-80.0,-60.0,-40.0,-20.0,-10.0,0.0,10.0,20.0,40.0,60.0,80.0,100.0};  

float DFF[13] = {-20.0,-15.0,-10.0,-5.0,-2.0,-1.0,0.0,1.0,2.0,5.0,10.0,15.0,20.0}; 

float UFF[13] = {-1.0,-0.8,-0.6,-0.4,-0.2,-0.1,0.0,0.1,0.2,0.4,0.6,0.8,1.0}; 

//uint32_t FuzzyRule[13][13];

uint32_t FuzzyRule[7][7];

///*ec -3   -2   -1    0    1    2    3*/
//{                                        /* e*/
//  {   3,   3,   3,   2,   2,   2,   1},  /*-3*/
//  {   3,   3,   2,   2,   2,   1,   1},  /*-2*/
//  {   3,   2,   2,   2,   1,   1,   0},  /*-1*/
//  {   2,   2,   2,   1,   1,   0,   0},  /* 0*/
//  {   0,  -1,  -1,  -2,  -2,  -2,  -3},  /* 1*/
//  {  -1,  -1,  -2,  -2,  -2,  -3,  -3},  /* 2*/
//  {  -1,  -2,  -2,  -2,  -3,  -3,  -3},  /* 3*/
//};

float FuzzyControl(fuzzy_ctrl_t *fuzzy,float target,float actual)
{
    float EF[2],DF[2];

    uint32_t En = 0,Dn = 0;

    uint32_t Un[4];

    float UF[13];

    float U[13];

    for(int i = 0 ; i < 13 ;i++)
    {
        U[i] = 0.0;
    }

    /*Calculate E and Ec.*/

    fuzzy->e[0] = fuzzy->e[1];
    fuzzy->e[1] = target - actual;
      
    fuzzy->ec = fuzzy->e[1] - fuzzy->e[0];

    /*Calculate Membership.*/

    if(fuzzy->e[1] < EFF[0])
    {
        EF[0] = 1.0;
        EF[1] = 0.0;    

        En = -6;
    }
    else if(fuzzy->e[1] > EFF[12])
    {
        EF[0] = 0.0;  
        EF[1] = 1.0;  
        En = 6;
    }
    else
    {
        for(int i = 1 ; i <= 12 ; i++)
        {
            if(fuzzy->e[1] <= EFF[i] && fuzzy->e[1] >= EFF[i - 1]) //有2条规则生效
            {
                En = i - 1 - 6;

                EF[0] = -(fuzzy->e[1] - EFF[i])/(EFF[i] - EFF[i - 1]);

                EF[1] = (fuzzy->e[1] - EFF[i - 1])/(EFF[i] - EFF[i - 1]);
            }
        }         
    }

    if(fuzzy->ec < DFF[0])
    {
        DF[0] = 1.0;
        DF[1] = 0.0;    

        Dn = -6;
    }
    else if(fuzzy->ec > DFF[12])
    {
        DF[0] = 0.0;  
        DF[1] = 1.0;  
        Dn = 5;
    }
    else
    {
        for(int i = 1 ; i <= 12 ; i++)
        {
            if(fuzzy->ec <= DFF[i] && fuzzy->ec >= DFF[i - 1]) //有2条规则生效
            {
                En = i - 1 - 6;

                DF[0] = -(fuzzy->e[1] - DFF[i])/(DFF[i] - DFF[i - 1]);

                DF[1] = (fuzzy->e[1] - DFF[i - 1])/(DFF[i] - DFF[i - 1]);
            }
        }         
    }

    /*Fuzzy Reasoning.*/
    
    Un[0] = FuzzyRule[En + 6][Dn + 6] + 6;      //2*2 = 4条规则生效

    UF[0] = min(EF[0],DF[0]);

    Un[1] = FuzzyRule[En + 6 + 1][Dn + 6] + 6;
    UF[1] = min(EF[1],DF[0]);

    Un[2] = FuzzyRule[En + 6][Dn + 6 + 1] + 6;
    UF[2] = min(EF[0],DF[1]);

    Un[3] = FuzzyRule[En + 6 + 1][Dn + 6 + 1] + 6;
    UF[3] = min(EF[1],DF[1]);

    if(Un[0] == Un[1])    //同隶属度语言取大
    {
        if(UF[0] > UF[1])
            UF[1] = 0;
        else
            UF[0] = 0;
    }
    if(Un[0] == Un[2])
    {
        if(UF[0] > UF[2])
            UF[2] = 0;
        else
            UF[0] = 0;
    }
    if(Un[0] == Un[3])
    {
        if(UF[0] > UF[3])
            UF[3] = 0;
        else
            UF[0] = 0;
    }

    if(Un[1] == Un[2])
    {
        if(UF[1] > UF[2])
            UF[2] = 0;
        else
            UF[1] = 0;
    }
    if(Un[1] == Un[3])
    {
        if(UF[1] > UF[3])
            UF[3] = 0;
        else
            UF[1] = 0;
    }

    if(Un[2] == Un[3])
    {
        if(UF[2] > UF[3])
            UF[3] = 0;
        else
            UF[2] = 0;
    }

    for(int i = 0 ; i < 4 ;i++)
    {
        U[Un[i]] += UF[i];
    }   

    /*Anti Fuzzy(Gravity Method)*/

    float sum0 = 0.0;
    float sum1 = 0.0;

    for(int i = 0 ; i < 4 ;i++)
    {
        sum0 += U[i] * UFF[i];

        sum1 += U[i];
    }

    fuzzy->U = sum0/sum1;

    return fuzzy->U;
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

//       for(int i = 0 ; i < 13 ; i++)                   //计算模糊集合 E_MemberShip[13] 和 EC_MemberShip[13] 的 笛卡尔积，并转成1维矩阵ECC[169]。
//       {
//           for(int j = 0 ; j < 13 ; j++)
//           {
//               if(E_MemberShip[i] < EC_MemberShip[j])  //取小运算
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
//           for(int j = 0 ; j < 13;j++)   //*取小 +取大
//           {
//               if(EEC[i] > R[i][j])
//                 temp = R[i][j];
//               else
//                 temp = EEC[i];
              
//               if(temp > Result[j])
//                   Result[j] = temp;
//           }
//       }

//       /*反模糊(重心法)*/

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
