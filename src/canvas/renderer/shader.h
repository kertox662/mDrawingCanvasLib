// Code taken and modified from The Cherno: https://www.youtube.com/watch?v=gDtHL6hy9R8&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=15

#ifndef SHADER_H //SHADER_H
#define SHADER_H

#include "GLCommon.h"
#include <string>
#include <unordered_map>
#include <array>


class Shader{
private:
    unsigned int m_RendererID;
    //Uniform caching
    std::unordered_map<std::string, int> m_UniformLocations;

public:
    Shader();
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
    ~Shader();

    void Create(const std::string &vertexFile, const std::string &fragmentFile);

    void Bind() const;
    void Unbind() const;

    //SET UNIFORMS
    template<typename T, unsigned int count>
    void SetUniform(std::string &name, std::array<T,count> &data);

    static std::string LoadShader(const std::string &filepath);
private:
    unsigned int GetUniformLocation(const std::string &name);
    unsigned int CompileShader(const unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
};

#endif //SHADER_H