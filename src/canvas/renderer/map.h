#ifndef MAP_H //SHADER_H
#define MAP_H

namespace mdcl{

// Takes a value x between low and high and outputs a value mapped linearly between mapLow and mapHigh
inline float map(float x, float low, float high, float mapLow, float mapHigh){
    float lerpRatio = (x - low)/(high-low);
    return mapLow + lerpRatio*(mapHigh - mapLow);
}

} //namespace mdcl

#endif //MAP_H