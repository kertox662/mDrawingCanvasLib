#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include "shader.h"

class VertexBuffer;
class IndexBuffer;
class VertexArray;

namespace mdcl{

class Color;

class Shape{
    std::vector<std::pair<float,float>> _data;
    unsigned int _vaoId, _vboId, _iboId;
    
    public:
    Shape();
    ~Shape();
    void add(std::pair<float,float> val);
    void add(float x, float y);
    void removeLast();
    float* toArray(unsigned int* lenOut);
    void render(int width, int height, Shader&);
}; //struct Shape

}// namespace mdcl

#endif //SHAPE_H