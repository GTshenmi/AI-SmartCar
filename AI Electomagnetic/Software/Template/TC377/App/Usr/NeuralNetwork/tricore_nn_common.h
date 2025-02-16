/*
 * cmsis_nncommon.h
 *
 *  Created on: 2021年4月14日
 *      Author: 936305695
 */

#ifndef APP_USR_NEURALNETWORK_TRICORE_NN_COMMON_H_
#define APP_USR_NEURALNETWORK_TRICORE_NN_COMMON_H_


#include "platform.h"

#define ARM_TANH TRICORE_TANH

#define arm_fully_connected_q15 tricore_fully_connected_q15
#define arm_relu_q15 tricore_relu_q15

//#define aia_relu8_q15

typedef enum
{
    TRICORE_MATH_SUCCESS = 0,                 /**< No error */
    TRICORE__MATH_ARGUMENT_ERROR = -1,        /**< One or more arguments are incorrect */
    TRICORE__MATH_LENGTH_ERROR = -2,          /**< Length of data buffer is incorrect */
    TRICORE__MATH_SIZE_MISMATCH = -3,         /**< Size of matrices is not compatible with the operation. */
    TRICORE__MATH_NANINF = -4,                /**< Not-a-number (NaN) or infinity is generated */
    TRICORE__MATH_SINGULAR = -5,              /**< Generated by matrix inversion if the input matrix is singular and cannot be inverted. */
    TRICORE__MATH_TEST_FAILURE = -6           /**< Test Failed  */
}tricore_status;

typedef enum
{
    TRICORE_SIGMOID = 0,
    TRICORE_TANH = 1,
}tricore_nn_activation_type;

typedef sint16_t q15_t;

typedef sint8_t q7_t;

typedef sint32_t q31_t;

#define NN_ROUND NN_ROUND1

#define NN_ROUND1(out_shift) ( 0x1 << (out_shift - 1) )

static inline int __SSAT_GNU(signed long int VAL, signed long int BITPOS)//ARM 有符号饱和指令
{
    signed long int min = -(1<<(BITPOS-1));
    signed long int max = (1<<(BITPOS-1)) - 1;
    if (VAL < min)
        return min;
    else if (VAL > max)
        return max;
    else
        return VAL;
}
#define __SSAT(VAL, BITPOS) __SSAT_GNU(VAL,BITPOS)

static inline int __USAT_GNU(signed long int VAL, signed long int BITPOS) //待验证 ARM 无符号饱和指令
{
    signed long int min = 0;
    signed long int max = (1<<(BITPOS)) - 1;

    if (VAL < min)
        return min;
    else if (VAL > max)
        return max;
    else
        return VAL;
}
#define __USAT(VAL, BITPOS) __USAT_GNU(VAL,BITPOS)


#endif /* APP_USR_NEURALNETWORK_TRICORE_NN_COMMON_H_ */
