#include "imath.h"

// Fast inverse square-root

float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}


float my_abs(float f)
{
	if (f >= 0.0f)
	{
		return f;
	}

	return -f;
}


uint16_t  throttle_limit(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max)
{
	if(thr_in<thr_min)	thr_in = thr_min;
	if(thr_in>thr_max)	thr_in = thr_max;
	
	return thr_in;
}


