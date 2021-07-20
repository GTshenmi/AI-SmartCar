#include "fuzzycontrol.h"

/*
  @TODO: Calculate Matrix R;        --- Finished.
  @TODO: Calculate MemberShip.
  @TODO: Optimize Fuzzy Reasoning.

  @TODO: Finish Fuzzy Rule.
  @TODO: Finish EFF DFF UFF.
*/


float E_MemberShipR[7][13] = {
/*e   -6  -5  -4  -3  -2  -1  0   1   2   3   4   5   6*/
    {1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NB
    {0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NM
    {0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NS
    {0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0},//ZO
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0},//PS
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0},//PM   
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0},//PB
};

float EC_MemberShipR[7][13] = {
/*ec  -6  -5  -4  -3  -2  -1  0   1   2   3   4   5   6*/
    {1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NB
    {0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NM
    {0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NS
    {0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0},//ZO
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0},//PS
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0},//PM   
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0},//PB
};

float U_MemberShipR[7][13] = {
/*ec  -6  -5  -4  -3  -2  -1  0   1   2   3   4   5   6*/
    {1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NB
    {0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NM
    {0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0,0.0,0.0},//NS
    {0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0,0.0,0.0},//ZO
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0,0.0,0.0},//PS
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0,0.5,0.0},//PM   
    {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.5,1.0},//PB
};

float R1[13][13];

float R2[169];

float R3[169][13];

float E_MemberShip[13];  

float EC_MemberShip[13]; 

float EEC[169];          //(E * EC) ת����һά����

float EFF[13] = {-100.0,-80.0,-60.0,-40.0,-20.0,-10.0,0.0,10.0,20.0,40.0,60.0,80.0,100.0};  

float DFF[13] = {-20.0,-15.0,-10.0,-5.0,-2.0,-1.0,0.0,1.0,2.0,5.0,10.0,15.0,20.0}; 

float UFF[13] = {-1.0,-0.8,-0.6,-0.4,-0.2,-0.1,0.0,0.1,0.2,0.4,0.6,0.8,1.0}; 

uint32_t FuzzyRule[7][7] =
/*ec -3   -2   -1    0    1    2    3*/
{                                        /* e*/
  {   3,   3,   3,   2,   2,   2,   1},  /*-3*/
  {   3,   3,   2,   2,   2,   1,   1},  /*-2*/
  {   3,   2,   2,   2,   1,   1,   0},  /*-1*/
  {   2,   2,   2,   1,   1,   0,   0},  /* 0*/
  {   0,  -1,  -1,  -2,  -2,  -2,  -3},  /* 1*/
  {  -1,  -1,  -2,  -2,  -2,  -3,  -3},  /* 2*/
  {  -1,  -2,  -2,  -2,  -3,  -3,  -3},  /* 3*/
};

float R[169][13];         

float Result[13];          

void Fuzzy_CalculateMatrixR()
{
    uint32_t k = 0;

    uint32_t U_Index = 0;

    for(int i = 0 ; i < 169 ;i++)
    {
        for(int j = 0 ; j <13 ;j++)
        {
            R[i][j] = 0.0;
        }
    }

    for(int e_index = 0 ; e_index < 7 ; e_index++)
    {
        for(int ec_index = 0 ; ec_index < 7 ;ec_index++) /*Calculae U(E(i) X EC(j)) for i , j = 1,2...7*/
        {
            U_Index = FuzzyRule[e_index][ec_index] + 3;

            for(int i = 0 ; i < 13 ;i++)
            {
                for(int j = 0 ; j < 13 ;j++)
                {
                    if(E_MemberShipR[e_index][i] > EC_MemberShipR[ec_index][j])
                        R1[i][j] = EC_MemberShipR[ec_index][j];
                    else
                        R1[i][j] = E_MemberShipR[e_index][i];

                    R2[k++] = R1[i][j];      
                }
            }

            for(int i = 0 ; i < 163 ;i++)
            {
                for(int j = 0 ; j < 13 ;j++)
                {
                    if(R2[i] > U_MemberShipR[U_Index][j])
                        R3[i][j] = U_MemberShipR[U_Index][j];
                    else
                        R3[i][j] = R2[j];

                    if(R3[i][j] > R[i][j])
                        R[i][j] = R3[i][j];
                }
            }

        }
    }
}

 void Fuzzy_Ctrl(fuzzy_ctrl_t *fuzzy,float target,float actual)
 {
      /*Calculate Error and Delta Error.*/
      fuzzy->e[0] = fuzzy->e[1];
      fuzzy->e[1] = target - actual;
      
      fuzzy->ec = fuzzy->e[1] - fuzzy->e[0];

      /*Calculate MemberShip.*/

      for(int i = 0 ; i < 13 ; i++)
      {
          E_MemberShip[i] = 0.0;
          EC_MemberShip[i] = 0.0;
      }

      if(fuzzy->e[1] < EFF[0])
      {
          E_MemberShip[0] = 1.0;
          E_MemberShip[1] = 0.0;      
      }
      else if(fuzzy->e[1] > EFF[12])
      {
          E_MemberShip[12] = 1.0;
          E_MemberShip[11] = 0.0;    
      }
      else
      {
          for(int i = 1 ; i < 12 ; i++)
          {
              if(fuzzy->e[1] <= EFF[i] && fuzzy->e[1] >= EFF[i - 1])
              {
                  E_MemberShip[i] =(-fuzzy->e[1] + EFF[i])/(EFF[i] - EFF[i-1]);
                  E_MemberShip[i - 1] = (1.0 - E_MemberShip[i])/2.0;
                  E_MemberShip[i+i] = (1.0 - E_MemberShip[i])/2.0;
              }
          }         
      }

      if(fuzzy->ec < DFF[0])
      {
          EC_MemberShip[0] = 1.0;
          EC_MemberShip[1] = 0.0;             
      }
      else if(fuzzy->ec > DFF[12])
      {
          EC_MemberShip[12] = 1.0;
          EC_MemberShip[11] = 0.0;          
      }
      else
      {
          for(int i = 1 ; i < 12 ; i++)
          {
              if(fuzzy->ec <= DFF[i] && fuzzy->ec >= DFF[i - 1])
              {
                  EC_MemberShip[i] =(-fuzzy->e[1] + DFF[i])/(DFF[i] - DFF[i-1]);
                  EC_MemberShip[i - 1] = (1.0 - EC_MemberShip[i])/2.0;
                  EC_MemberShip[i+i] = (1.0 - EC_MemberShip[i])/2.0;
              }       
          }
      }

      /*Fuzzy Reasoning.*/

      uint32_t k = 0;

      for(int i = 0 ; i < 13 ; i++)                   //����ģ������ E_MemberShip[13] �� EC_MemberShip[13] �� �ѿ���������ת��1ά����ECC[169]��
      {
          for(int j = 0 ; j < 13 ; j++)
          {
              if(E_MemberShip[i] < EC_MemberShip[j])  //ȡС����
                  EEC[k] = E_MemberShip[i];         
              else
                  EEC[k] = EC_MemberShip[j];
              
              k++;
          }
      }

      float temp = 0.0;

      for(int i = 0 ; i < 16 ;i++)
          Result[i] = 0.0;

      /*Calculate ECC[169] X R[169][13]*/  
      for(int i = 0 ; i < 169 ;i++)         
      {
          for(int j = 0 ; j < 13;j++)   //*ȡС +ȡ��
          {
              if(EEC[i] > R[i][j])
                temp = R[i][j];
              else
                temp = EEC[i];
              
              if(temp > Result[j])
                  Result[j] = temp;
          }
      }

      /*��ģ��(���ķ�)*/

      fuzzy->U = 0.0;

      float sum0 = 0.0;
      float sum1 = 0.0;

      for(int i = 0 ; i < 13 ; i++)
      {
          sum0 += Result[i] * UFF[i];
          sum1 += Result[i];
      }

      fuzzy->U = sum0/sum1;
 }
