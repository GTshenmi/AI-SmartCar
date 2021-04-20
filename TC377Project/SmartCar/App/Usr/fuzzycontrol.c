#include "fuzzycontrol.h"

/*
  @TODO: Calculate Matrix R;
  @TODO: Calculate MemberShip.
  @TODO: Optimize Fuzzy Reasoning.
*/

float E_MemberShip[13];  //Caluculate

float EC_MemberShip[13]; //Calculate

float EEC[169];          //(E * EC) 转换的一维向量 --- Calculate

float EFF[7] = {-100.0,-60.0,-20.0,0.0,20.0,60.0,100.0};  

float DFF[7] = {-20.0,-10.0,-5.0,0.0,5.0,10.0,20.0}; 

float UFF[7] = {-1.0,-0.6,-0.2,0.0,0.2,0.6,1.0}; 

float Rule[7][7] =                  
/*ec -3   -2   -1   0   1     2    3*/
{                                        /* e*/
  { 3.0, 3.0, 3.0, 2.0, 1.0, 1.0, 1.0},  /*-3*/
  { 3.0, 6.0,-6.0,-6.0,-6.0,-6.0,-6.0},  /*-2*/
  {-6.0,-6.0,-6.0,-6.0,-6.0,-6.0,-6.0},  /*-1*/
  {-6.0,-6.0,-6.0,-6.0,-6.0,-6.0,-6.0},  /* 0*/
  {-6.0,-6.0,-6.0,-6.0,-6.0,-6.0,-6.0},  /* 1*/
  {-6.0,-6.0,-6.0,-6.0,-6.0,-6.0,-6.0},  /* 2*/
  {-6.0,-6.0,-6.0,-6.0,-6.0,-6.0,-6.0},  /* 3*/
};

float R[169][13];         

float Result[13];          

void Fuzzy_CalculateMatrixR()
{
    
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

      for(int i = 0 ; i < 13 ; i++)                   //计算模糊集合 E_MemberShip[13] 和 EC_MemberShip[13] 的 笛卡尔积，并转成1维矩阵ECC[169]。
      {
          for(int j = 0 ; j < 13 ; j++)
          {
              if(E_MemberShip[i] < EC_MemberShip[j])  //取小运算
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
          for(int j = 0 ; j < 13;j++)   //*取小 +取大
          {
              if(EEC[i] > R[i][j])
                temp = R[i][j];
              else
                temp = EEC[i];
              
              if(temp > Result[j])
                  Result[j] = temp;
          }
      }

      /*反模糊(重心法)*/

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
