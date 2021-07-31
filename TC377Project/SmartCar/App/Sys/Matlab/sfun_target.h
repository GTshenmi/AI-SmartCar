/*
 * File: sfun_target.h
 *
 * Code generated for Simulink model 'sfun_target'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sat Jul 31 20:19:11 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Infineon->TriCore
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_sfun_target_h_
#define RTW_HEADER_sfun_target_h_
#include <math.h>
#ifndef sfun_target_COMMON_INCLUDES_
# define sfun_target_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* sfun_target_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay1_DSTATE;            /* '<S1>/Unit Delay1' */
  real_T UnitDelay_DSTATE;             /* '<S1>/Unit Delay' */
} DW;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
  DW *dwork;
};

/* Model entry point functions */
extern void sfun_target_initialize(RT_MODEL *const rtM);
extern void sfun_target_step(RT_MODEL *const rtM, real_T rtU_v0, real_T *rtY_v1,
  real_T *rtY_v2);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Scope' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('ADRC_test/安排过渡环节')    - opens subsystem ADRC_test/安排过渡环节
 * hilite_system('ADRC_test/安排过渡环节/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ADRC_test'
 * '<S1>'   : 'ADRC_test/安排过渡环节'
 * '<S2>'   : 'ADRC_test/安排过渡环节/fst'
 * '<S3>'   : 'ADRC_test/安排过渡环节/fst/MATLAB Function'
 */
#endif                                 /* RTW_HEADER_sfun_target_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
