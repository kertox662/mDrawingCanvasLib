#ifndef UTIL_H //UTIL_H
#define UTIL_H

namespace mdcl{

// Takes a value x between low and high and outputs a value mapped linearly between mapLow and mapHigh
inline float map(float x, float low, float high, float mapLow, float mapHigh){
    float lerpRatio = (x - low)/(high-low);
    return mapLow + lerpRatio*(mapHigh - mapLow);
}


// Fast inverse square root taken from en.wikipedia.org/wiki/Fast_inverse_square_root#Overview_of_the_code
inline float invSqrt(float number){
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );
	y  = y * ( threehalfs - ( x2 * y * y ) );
	return y;
}

} //namespace mdcl

#endif //UTIL_H