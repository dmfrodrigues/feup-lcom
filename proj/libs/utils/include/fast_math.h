#ifndef FAST_MATH_H_INCLUDED
#define FAST_MATH_H_INCLUDED

/**
 * @defgroup fast_math  fast_math
 * @ingroup utils
 * @brief   Fast math module.
 *
 * Essentially, this module provides almost-constant time for mathematical operations
 * that do not require extreme precision.
 *
 * @{
 */

/**
 * @brief Fast sine function.
 * @param   x   Argument
 * @return      Sine of x
 */
double fm_sin(double x);
/**
 * @brief Fast cosine function.
 * @param   x   Argument
 * @return      Cosine of x
 */
double fm_cos(double x);

/**
 * @}
 */

#endif //FAST_MATH_H_INCLUDED
