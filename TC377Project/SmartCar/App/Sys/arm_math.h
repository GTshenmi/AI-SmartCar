/*
 * armmath.h
 *
 *  Created on: 2021��7��23��
 *      Author: admin
 */

#ifndef APP_SYS_ARMMATH_H_
#define APP_SYS_ARMMATH_H_

#include "os.h"

typedef enum
{
    ARM_MATH_SUCCESS = 0,                /**< No error */
    ARM_MATH_ARGUMENT_ERROR = -1,        /**< One or more arguments are incorrect */
    ARM_MATH_LENGTH_ERROR = -2,          /**< Length of data buffer is incorrect */
    ARM_MATH_SIZE_MISMATCH = -3,         /**< Size of matrices is not compatible with the operation. */
    ARM_MATH_NANINF = -4,                /**< Not-a-number (NaN) or infinity is generated */
    ARM_MATH_SINGULAR = -5,              /**< Generated by matrix inversion if the input matrix is singular and cannot be inverted. */
    ARM_MATH_TEST_FAILURE = -6           /**< Test Failed  */
}arm_status;



typedef struct
{
   uint16_t numRows;     /**< number of rows of the matrix.     */
   uint16_t numCols;     /**< number of columns of the matrix.  */
   float *pData;     /**< points to the data of the matrix. */
}arm_matrix_instance_f32;

void swap_rows(const arm_matrix_instance_f32* pSrcM, int r1, int r2);
void set_identity_matrix(
    const arm_matrix_instance_f32* pSrcM);
void scale_row(const arm_matrix_instance_f32* pSrcM, int r, double scalar);
void shear_row(const arm_matrix_instance_f32* pSrcM, int r1, int r2, double scalar);
arm_status arm_mat_trans_f32(
 const arm_matrix_instance_f32 * pSrc,
 arm_matrix_instance_f32 * pDst);

arm_status arm_mat_mult_f32(
  const arm_matrix_instance_f32 * pSrcA,
  const arm_matrix_instance_f32 * pSrcB,
  arm_matrix_instance_f32 * pDst);

arm_status arm_mat_add_f32(
const arm_matrix_instance_f32 * pSrcA,
const arm_matrix_instance_f32 * pSrcB,
arm_matrix_instance_f32 * pDst);

arm_status arm_mat_inverse_f32(
const arm_matrix_instance_f32 * src,
arm_matrix_instance_f32 * dst);

arm_status arm_mat_sub_f32(
const arm_matrix_instance_f32 * pSrcA,
const arm_matrix_instance_f32 * pSrcB,
arm_matrix_instance_f32 * pDst);

#endif /* APP_SYS_ARMMATH_H_ */