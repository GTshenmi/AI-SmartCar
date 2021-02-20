/*
 * model1.nncu.h
 *
 *  Created on: 2021Äê2ÔÂ4ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_NEURALNETWORK_MODEL_NNCU_H_
#define APP_USR_NEURALNETWORK_MODEL_NNCU_H_

#if defined __ICCARM__
#define __ALIGNED(x) __attribute__((aligned(x)))
#else
#define __ALIGNED(x) __attribute__((__align(x)))
#endif

extern const unsigned char tracking_model[] __ALIGNED(16);
extern const unsigned char element_model[] __ALIGNED(16);
extern const unsigned char model1[] __ALIGNED(16);

#endif /* APP_USR_NEURALNETWORK_MODEL_NNCU_H_ */
