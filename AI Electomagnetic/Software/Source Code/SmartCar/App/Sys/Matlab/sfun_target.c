/*
 * File: sfun_target.c
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

#include "sfun_target.h"

/*===========*
 * Constants *
 *===========*/
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

/*
 * UNUSED_PARAMETER(x)
 *   Used to specify that a function parameter (argument) is required but not
 *   accessed by the function body.
 */
#ifndef UNUSED_PARAMETER
# if defined(__LCC__)
#   define UNUSED_PARAMETER(x)                                   /* do nothing */
# else

/*
 * This is the semi-ANSI standard way of indicating that an
 * unused function parameter is required.
 */
#   define UNUSED_PARAMETER(x)         (void) (x)
# endif
#endif

/* Model step function */
void sfun_target_step(RT_MODEL *const rtM, real_T rtU_v0, real_T *rtY_v1, real_T
                      *rtY_v2)
{
  DW *rtDW = ((DW *) rtM->dwork);
  real_T rtb_v2k;
  real_T rtb_v1k;
  real_T rtb_v1k1;
  int32_T rtb_v1k_0;

  /* UnitDelay: '<S1>/Unit Delay1' */
  rtb_v2k = rtDW->UnitDelay1_DSTATE;

  /* UnitDelay: '<S1>/Unit Delay' */
  rtb_v1k = rtDW->UnitDelay_DSTATE;

  /* Sum: '<S1>/Sum' incorporates:
   *  Gain: '<S1>/h'
   */
  rtb_v1k1 = 0.002 * rtb_v2k + rtb_v1k;

  /* Outport: '<Root>/v1' */
  *rtY_v1 = rtb_v1k1;

  /* MATLAB Function: '<S2>/MATLAB Function' incorporates:
   *  Constant: '<S1>/h0'
   *  Constant: '<S1>/r'
   *  Inport: '<Root>/v0'
   *  Sum: '<S1>/Sum2'
   */
  rtb_v1k = (rtb_v1k - rtU_v0) + 0.01 * rtb_v2k;
  if (fabs(rtb_v1k) <= 100.0) {
    rtb_v1k = rtb_v1k / 0.01 + rtb_v2k;
  } else {
    if (rtb_v1k > 0.0) {
      rtb_v1k_0 = 1;
    } else if (rtb_v1k < 0.0) {
      rtb_v1k_0 = -1;
    } else {
      rtb_v1k_0 = 0;
    }

    rtb_v1k = (sqrt(8.0E+6 * fabs(rtb_v1k) + 1.0E+8) - 10000.0) * 0.5 * (real_T)
      rtb_v1k_0 + rtb_v2k;
  }

  if (fabs(rtb_v1k) <= 10000.0) {
    rtb_v1k /= 10000.0;
  } else if (rtb_v1k > 0.0) {
    rtb_v1k = 1.0;
  } else if (rtb_v1k < 0.0) {
    rtb_v1k = -1.0;
  } else {
    rtb_v1k = 0.0;
  }

  /* Sum: '<S1>/Sum1' incorporates:
   *  Constant: '<S1>/r'
   *  Gain: '<S1>/h1'
   *  MATLAB Function: '<S2>/MATLAB Function'
   */
  rtb_v2k += -1.0E+6 * rtb_v1k * 0.002;

  /* Outport: '<Root>/v2' */
  *rtY_v2 = rtb_v2k;

  /* Update for UnitDelay: '<S1>/Unit Delay1' */
  rtDW->UnitDelay1_DSTATE = rtb_v2k;

  /* Update for UnitDelay: '<S1>/Unit Delay' */
  rtDW->UnitDelay_DSTATE = rtb_v1k1;
}

/* Model initialize function */
void sfun_target_initialize(RT_MODEL *const rtM)
{
  /* (no initialization code required) */
  UNUSED_PARAMETER(rtM);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
