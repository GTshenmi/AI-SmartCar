/*
 * nncu.c
 *
 *  Created on: 2021Äê4ÔÂ14ÈÕ
 *      Author: 936305695
 */
#include <tricore_nn.h>


/**
 * @addtogroup FC
 * @{
 */

  /**
   * @brief Q15 opt fully-connected layer function
   * @param[in]       pV          pointer to input vector
   * @param[in]       pM          pointer to matrix weights
   * @param[in]       dim_vec     length of the vector
   * @param[in]       num_of_rows number of rows in weight matrix
   * @param[in]       bias_shift  amount of left-shift for bias
   * @param[in]       out_shift   amount of right-shift for output
   * @param[in]       bias        pointer to bias
   * @param[in,out]   pOut        pointer to output vector
   * @param[in,out]   vec_buffer  pointer to buffer space for input
   * @return     The function returns <code>tricore_MATH_SUCCESS</code>
   *
   *
   * @details
   *
   * <b>Buffer size:</b>
   *
   * vec_buffer size: 0
   *
   */

tricore_status tricore_fully_connected_q15(const q15_t * pV,
                        const q15_t * pM,
                        const uint16_t dim_vec,
                        const uint16_t num_of_rows,
                        const uint16_t bias_shift,
                        const uint16_t out_shift,
                        const q15_t * bias,
                        q15_t * pOut,
                        q15_t * vec_buffer)
{
    int i, j;
    /* Run the following code as reference implementation for Cortex-M0 and Cortex-M3 */
    for (i = 0; i < num_of_rows; i++)
    {
        int32_t       sum0 = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
        int32_t ip_out = 0, mul;
        for (j = 0; j < dim_vec; j++)
        {
            mul = pV[j] * pM[i * dim_vec + j];
            ip_out += mul;
        }
        ip_out += sum0;
        pOut[i] = (q15_t) __SSAT((ip_out >> out_shift), 16);
        ip_out >>= 32;
        if (ip_out == 1 || ip_out == -2)
            i = i + 0; // i = i;
    }

    /* Return to application */
    return (TRICORE_MATH_SUCCESS);

}

/**
  * @brief Q15 neural network activation function using direct table look-up
  * @param[in,out]   data        pointer to input
  * @param[in]       size        number of elements
  * @param[in]       int_width   bit-width of the integer part, assume to be smaller than 3
  * @param[in]       type        type of activation functions
  * @return none.
  *
  * @details
  *
  * This is the direct table look-up approach.
  *
  * Assume here the integer part of the fixed-point is <= 3.
  * More than 3 just not making much sense, makes no difference with
  * saturation followed by any of these activation functions.
  */

void tricore_nn_activations_direct_q15(q15_t * data, uint16_t size, uint16_t int_width, tricore_nn_activation_type type)
{
   uint16_t  i = size;
   q15_t    *pIn = data;
   q15_t    *pOut = data;
   uint16_t  shift_size = 8 + 3 - int_width;
   uint32_t  bit_mask = 0x7FF >> int_width;
   uint32_t  full_frac = bit_mask + 1;
   const q15_t *lookup_table;

   switch (type)
   {
       case TRICORE_SIGMOID:
           lookup_table = sigmoidTable_q15;
           break;
       case TRICORE_TANH:
       default:
           lookup_table = tanhTable_q15;
           break;
   }

   while (i)
   {
       q15_t     out;
       q15_t     in = *pIn++;

#pragma warning 560
       q15_t     frac = (uint32_t) in & bit_mask;
       q15_t     value = lookup_table[__USAT(in >> shift_size, 8)];
       q15_t     value2 = lookup_table[__USAT(1 + (in >> shift_size), 8)];

       /* doing the interpolation here for better accuracy */
       out = ((q31_t) (full_frac - frac) * value + (q31_t) value2 * frac) >> shift_size;
#pragma warning default
       *pOut++ = out;
       i--;
   }

}

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup Acti
 * @{
 */

  /**
   * @brief Q15 RELU function
   * @param[in,out]   data        pointer to input
   * @param[in]       size        number of elements
   * @return none.
   *
   * @details
   *
   * Optimized relu with QSUB instructions.
   *
   */

void tricore_relu_q15(q15_t * data, uint32_t size)
{

    /* Run the following code as reference implementation for Cortex-M0 and Cortex-M3 */
    uint32_t  i;

    for (i = 0; i < size; i++)
    {
        if (data[i] < 0)
            data[i] = 0;
    }

}

void aia_nn_activations_direct_q15(q15_t* data, int size, int int_width, tricore_nn_activation_type type, int quantBits)
{
    int  i = size;
    q15_t* pIn = data;
    q15_t* pOut = data;
    const q15_t* lookup_table;

    switch (type)
    {
        case TRICORE_SIGMOID:
            lookup_table = sigmoidTable_q15;
            break;
        case TRICORE_TANH:
        default:
            lookup_table = tanhTable_q15;
            break;
    }
    while (i--)
    {
        int     out1, out2, out;
        short     in = *pIn++;
#if 1
        int max = 1 << (quantBits - 1);
        int min = -max;
        if (in >= max)
            in = max - 1;
        if (in < min)
#pragma warning 560
            in = min;
#pragma warning default
#else
        in = __SSAT(in, quantBits);
#endif
        in <<= (16 - quantBits);
        in >>= (3 - int_width);
        uint32_t ndx0 = (in >> 8) & 0xFF;
        uint32_t ndx1 = (ndx0 + 1) & 0xFF;
        uint32_t frac = in & 0xFF;
        if (ndx1 == 0x80)
            ndx1 = ndx0;
        out1 = lookup_table[ndx0];
        out2 = lookup_table[ndx1];
        // interpolation, assume each of 256 slices is linear
        out = out1 + ((out2 - out1) * frac >> 8);

        *pOut++ = (q15_t)out >> (16 - quantBits);
    }
}

void aia_relu8_q15(q15_t* data, uint32_t size, uint32_t shfIn) {
    uint32_t i;
    int a, maxVal = (8 << shfIn) - 1;
    tricore_relu_q15(data, size);
    for (i = 0; i < size; i++) {
        a = *data++;
        if (a > maxVal)
#pragma warning 560
            data[-1] = maxVal;
#pragma warning default
    }
}

