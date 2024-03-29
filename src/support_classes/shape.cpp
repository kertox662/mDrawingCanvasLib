#include "shape.h"
#include "shaders.h"
#include "GLCommon.h"
#include "errorchecking.h"
#include "color.h"
#include "util.h"

#include <iostream>

mdcl::Shape::Shape(){
    glGenBuffers(1,&_vboId);
    glGenBuffers(1,&_iboId);
    glGenVertexArrays(1,&_vaoId);
}

mdcl::Shape::~Shape(){
    glDeleteBuffers(1,&_vboId);
    glDeleteBuffers(1,&_iboId);
    glDeleteVertexArrays(1,&_vaoId);
}

void mdcl::Shape::add(std::pair<float,float> val){
    _data.push_back(val);
}

void mdcl::Shape::add(float x, float y){
    _data.push_back({x,y});
}

void mdcl::Shape::removeLast(){
    _data.pop_back();
}

float* mdcl::Shape::toArray(unsigned int* lenOut){
    float* arr = new float[_data.size()*2];
    for(int i = 0; i < _data.size(); i++){
        arr[2*i] = _data[i].first;
        arr[2*i + 1] = _data[i].second;
    }
    *lenOut = _data.size()*2;
    return arr;
}

void mdcl::Shape::render(int width, int height, int thickness, Shader& shapeShader){
    unsigned int len;
    float* arr = toArray(&len);
    for(int i = 0; i < len; i++){
        int high = height, mlow = 1, mhigh = -1;
        if(i%2 == 0){
            high = width, mlow = -1, mhigh = 1;
        }
        arr[i] = mdcl::map(arr[i], 0, high, mlow, mhigh);
    }
    
    GLClearError();
    glBindVertexArray(_vaoId);
    if(GLCheckError("Shape VAO "));

    shapeShader.Bind();
    if(GLCheckError("Shape VBO0 "));
    glBindBuffer(GL_ARRAY_BUFFER, _vboId);
    if(GLCheckError("Shape VBO1 "));
    glBufferData(GL_ARRAY_BUFFER, len*sizeof(float), arr, GL_DYNAMIC_DRAW);
    if(GLCheckError("Shape VBO2 "));
    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
    if(GLCheckError("Shape VBO3 "));
    glEnableVertexAttribArray(0);
    if(GLCheckError("Shape VBO4 "));
    delete[] arr;
    if(GLCheckError("Shape VBO ")) return;


    unsigned int count = len/2;
    unsigned int* indices = new unsigned int[count];
    for(int i = 0; i < count; i++){
        indices[i] = i;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
    delete[] indices;
    if(GLCheckError("Shape IBO ")) return;

    std::pair<float,float>* normals = new std::pair<float,float>[count];
    float* borderPoints = new float[count*4];
    float* borderIndices = new float[count*6];

    for(int i = 0; i < count; i++){
        unsigned int next = (i+1)%count;
        std::pair<float,float> direction = std::pair<float,float>{arr[2*next] - arr[2*i],
                                                                  arr[2*next+1] - arr[2*i+1]};
        normals[i].first = -direction.second;
        normals[i].second = direction.first;

        float magSq = normals[i].first * normals[i].first + normals[i].second * normals[i].second;
        float normalizeFactor = invSqrt(magSq) * thickness * 0.5f;
        normals[i].first *= normalizeFactor;
        normals[i].second *= normalizeFactor;
    }

    for(int i = 0; i < count; i++){
        unsigned int next = (i+1)%count;
        borderPoints[4*i]   = arr[2*i]   + normals[i].first + normals[next].first;
        borderPoints[4*i+1] = arr[2*i+1] + normals[i].second + normals[next].second;
        borderPoints[4*i+2] = arr[2*i]   - normals[i].first - normals[next].first;
        borderPoints[4*i+3] = arr[2*i+1] - normals[i].second - normals[next].second;
        
        borderIndices[6*i]   = 2*i;
        borderIndices[6*i+1] = 2*next;
        borderIndices[6*i+2] = 2*next+1;
        borderIndices[6*i+3] = 2*i;
        borderIndices[6*i+4] = 2*i+1;
        borderIndices[6*i+5] = 2*next+1;
    }

    
    // std::string name = "u_Color";
    // shapeShader.SetUniform<float,4>(name,glColor);
    // if(GLCheckError("Shape Uniform ")) return;

    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    // glClearStencil(0);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
    glDrawElements(GL_TRIANGLE_FAN, count, GL_UNSIGNED_INT, nullptr);

    if(GLCheckError("Shape Stencil ")) return;

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDrawElements(GL_TRIANGLE_FAN, count, GL_UNSIGNED_INT, nullptr);

    if(GLCheckError("Shape Draw ")) return;



}