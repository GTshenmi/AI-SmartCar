/*
 * nncu.h
 *
 *  Created on: 2021Äê4ÔÂ14ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_NEURALNETWORK_TRICORE_NN_H_
#define APP_USR_NEURALNETWORK_TRICORE_NN_H_

#include "tricore_nn_common.h"
#include "tricore_nn_table.h"

tricore_status tricore_fully_connected_q15(const q15_t * pV,
                        const q15_t * pM,
                        const uint16_t dim_vec,
                        const uint16_t num_of_rows,
                        const uint16_t bias_shift,
                        const uint16_t out_shift,
                        const q15_t * bias,
                        q15_t * pOut,
                        q15_t * vec_buffer);
void tricore_nn_activations_direct_q15(q15_t * data, uint16_t size, uint16_t int_width, tricore_nn_activation_type type);
void tricore_relu_q15(q15_t * data, uint32_t size);
void aia_nn_activations_direct_q15(q15_t* data, int size, int int_width, tricore_nn_activation_type type, int quantBits);
void aia_relu8_q15(q15_t* data, uint32_t size, uint32_t shfIn);


#endif /* APP_USR_NEURALNETWORK_TRICORE_NN_H_ */
