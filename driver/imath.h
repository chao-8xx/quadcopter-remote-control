#ifndef _imath_h_
#define _imath_h_

#include "stm32f10x.h"


float invSqrt(float x);


float fast_atan2(float y, float x) ;


uint16_t  throttle_limit(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max);


#endif
