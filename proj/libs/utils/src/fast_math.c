#include <lcom/lcf.h>

#include "fast_math.h"
#include "utils.h"

#include <math.h>

double fm_sin(double x){
    if(x < 0.0)         return -fm_sin(-x);
    if(x > 2.0*M_PI) return fm_sin(x-2.0*M_PI);
    if(x > M_PI)      return -fm_sin(x-M_PI);
    if(x > 0.5*M_PI) x = M_PI - x;
    double x2 = x*x;
    double x3 = x*x2;
    double x5 = x3*x2;
    //double x7 = x5*x2;
    return x-x3*0.1666666666666666666666+x5*0.008333333333333333333333;//-x7*0.0001984126984127;
}

double fm_cos(double x){
    if(x < 0.0)         x = -x;
    if(x > 2.0*M_PI) return fm_cos(x-2.0*M_PI);
    if(x > M_PI)      x = 2.0*M_PI-x;
    if(x > 0.5*M_PI) return -fm_cos(M_PI-x);
    double x2 = x*x;
    double x4 = x2*x2;
    double x6 = x4*x2;
    //double x8 = x4*x4;
    return 1.0-x2*0.5+x4*0.041666666666666666666666-x6*0.0013888888888888888888888;//+x8*0.000024801587;
}
