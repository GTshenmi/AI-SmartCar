/*
 * armmath.c
 *
 *  Created on: 2021年7月23日
 *      Author: admin
 */
#include "arm_math.h"

#include <assert.h>

arm_status arm_mat_trans_f32(
 const arm_matrix_instance_f32 * pSrc,
 arm_matrix_instance_f32 * pDst)
{
    for(int i=0;i<pSrc->numRows;i++)
      {
               for(int j=0;j<pSrc->numCols;j++)
               {
                   int pos = i * pSrc->numCols + j;
                   int pos_T = j * pDst->numCols + i;
                   pDst->pData[pos_T]=pSrc->pData[pos];
               }

      }



    return 0;
}
#define assert(x)

//单位矩阵
void set_identity_matrix(
        const arm_matrix_instance_f32 * pSrcM)
{
  int i;
  int j;

  assert(pSrcM->numRows == pSrcM->numCols);

  for ( i = 0; i <pSrcM->numRows; i++)
  {
    for ( j = 0; j < pSrcM->numCols; j++)
    {
        int pos = i * pSrcM->numCols + j;
        if (i == j)
        {
            pSrcM->pData[pos] = 1.0;
        }
        else
        {
            pSrcM->pData[pos] = 0.0;
        }
    }
  }
}


//交换矩阵的两行
void swap_rows(const arm_matrix_instance_f32 * pSrcM, int r1, int r2)
{
    float tmp;

    assert(r1 != r2);

    for(int i = 0; i < pSrcM->numCols; i++)
    {
       int pos1 =r1 * pSrcM->numCols + i;
       int pos2 =r2 * pSrcM->numCols + i;

       tmp = pSrcM->pData[pos1];
       pSrcM->pData[pos1] = pSrcM->pData[pos2];
       pSrcM->pData[pos2] = tmp;
    }


}
/* 矩阵某行乘以一个系数 */
void scale_row(const arm_matrix_instance_f32 * pSrcM, int r, double scalar)
{
  int i;

  assert(scalar != 0.0);

  for ( i = 0; i < pSrcM->numCols; i++) {
      int pos_ri=r * pSrcM->numCols + i;
    pSrcM->pData[pos_ri] *= scalar;
  }
}

/* Add scalar * row r2 to row r1. */
void shear_row(const arm_matrix_instance_f32 * pSrcM, int r1, int r2, double scalar)
{
  int i;
  assert(r1 != r2);
  for ( i = 0; i < pSrcM->numCols; i++) {
      int pos_r1i=r1 * pSrcM->numCols + i;
      int pos_r2i=r2 * pSrcM->numCols + i;
      pSrcM->pData[pos_r1i] += scalar *pSrcM->pData[pos_r2i];
  }
}

arm_status arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst)
{
    int i,j,k;

    for(i=0;i<pDst->numRows;i++)
    {
        for(j=0;j<pDst->numCols;j++)
        {
            int pos = i * pDst->numCols + j;
            pDst->pData[pos]=0.0;

            for(k=0;k<pSrcA->numCols;k++)
            {
                int pos_mul_A=i * pSrcA->numCols + k;
                int pos_mul_B=k * pSrcB->numCols + j;
                pDst->pData[pos]+=pSrcA->pData[pos_mul_A]*pSrcB->pData[pos_mul_B];
            }
        }
    }

    return 0;
}

arm_status arm_mat_add_f32(
const arm_matrix_instance_f32 * pSrcA,
const arm_matrix_instance_f32 * pSrcB,
arm_matrix_instance_f32 * pDst)
{

    for(int i=0;i<pSrcA->numRows;i++)
    {
        for(int j=0;j<pSrcA->numCols;j++)
        {
            int pos = i * pSrcA->numCols + j;
            pDst->pData[pos] = pSrcA->pData[pos] + pSrcB->pData[pos];
        }
    }
    return 0;
}

arm_status arm_mat_inverse_f32(
const arm_matrix_instance_f32 * src,
arm_matrix_instance_f32 * dst)
{
    int i;
    int j;
    int r;
    double scalar;
    double shear_needed;
    int pos_ii;

    set_identity_matrix(dst);

    for ( i = 0; i < src->numRows; i++)
    {
        pos_ii = i * src->numCols + i;
        if (src->pData[pos_ii] == 0.0)
        {
          /* We must swap rows to get a nonzero diagonal element. */
            for (r = i + 1; r < src->numRows; r++)
            {
                int pos_ri=r * src->numRows + i;
                if (fabs(src->pData[pos_ri]) > 1e-6) {
                    break;
                }
            }
            if (r == src->numRows)
            {
        /* Every remaining element in this column is zero, so this
           matrix cannot be inverted. */
                return 0;


            }
            swap_rows(src, i, r);
            swap_rows(dst, i, r);
        }
        /* Scale this row to ensure a 1 along the diagonal.
           We might need to worry about overflow from a huge scalar here. */

        scalar = 1.0 / src->pData[pos_ii];
        scale_row(src, i, scalar);
        scale_row(dst, i, scalar);

        /* Zero out the other elements in this column. */
        for ( j = 0; j < src->numRows; j++)
        {
          if (i == j) {
              continue;
          }
          int pos_ji =j * src->numRows + i;
          shear_needed = -src->pData[pos_ji];
          shear_row(src, j, i, shear_needed);
          shear_row(dst, j, i, shear_needed);
        }
    }
    for(int h=0;h<16;h++)
    src->pData[h] = dst->pData[h];
    return 0;
}

arm_status arm_mat_sub_f32(
const arm_matrix_instance_f32 * pSrcA,
const arm_matrix_instance_f32 * pSrcB,
arm_matrix_instance_f32 * pDst)
{
    for(int i=0;i<pSrcA->numRows;i++)
    {
        for(int j=0;j<pSrcA->numCols;j++)
        {
            int pos = i * pSrcA->numCols + j;
            pDst->pData[pos] = pSrcA->pData[pos] - pSrcB->pData[pos];
        }
    }
    return 0;
}
