// Code taken and modified from The Cherno: https://www.youtube.com/watch?v=gDtHL6hy9R8&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=15

#include "shader.h"
#include "errorchecking.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(){}

Shader::Shader(const std::string &vSource, const std::string &fSource){
    m_RendererID = CreateShader(vSource, fSource);
}

Shader::~Shader(){
    glDeleteProgram(m_RendererID);
}

void Shader::Create(const std::string &vSource, const std::string &fSource){
    m_RendererID = CreateShader(vSource, fSource);
}

void Shader::Bind() const{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const{
    glUseProgram(0);
}

template<>
void Shader::SetUniform<float, 4>(std::string &name, std::array<float,4> &data){
    Bind();
    // GLCheckError("1 ");
    int location = GetUniformLocation(name);
    // GLCheckError("2 ");
    if(location != -1){
        glUniform4f(location, data[0],data[1],data[2],data[3]);
        // GLCheckError("3 ");
    }
    Unbind();
}

unsigned int Shader::GetUniformLocation(const std::string &name){
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        return m_UniformLocations[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if(location == -1){
        std::cout << "The uniform " << name << " does not exist for this shader." << std::endl;
    }
    m_UniformLocations[name] = location;
    return location;
}

std::string Shader::LoadShader(const std::string &filepath){
    std::ifstream shaderStream(filepath);
    std::stringstream shader;
    std::string line;
    
    while(getline(shaderStream, line)){
        shader << line;
        shader << std::endl;
    }
    return shader.str();
}

unsigned int Shader::CompileShader(const unsigned int type, const std::string &source){
    unsigned int id = glCreateShader(type); //Makes empty shader
    const char* src = source.c_str(); 
    glShaderSource(id,1,&src,nullptr); //Adds data to shader (Must be C-String)
    glCompileShader(id); //Compiles

    //Reminder - Do Error Handling for the Shader source code.
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //Gets compilation status
    if(result == GL_FALSE){ //If failed compiling
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //Gets how long the error message is
        char message[length];
        glGetShaderInfoLog(id, length, &length, message); //Puts data into message buffer
        std::cout << "FAILED " << ((type == GL_VERTEX_SHADER)?"vertex":"fragment") << " Shader Compilation!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id); //Deletes shader object
        return 0;
    }

    return id; //Gives back the id for referencing the shader program
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader){
    unsigned int program = glCreateProgram(); //The full program
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader); //Vertex and Fragment Shaders
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs); //Attaching basically makes them as a single object
    glAttachShader(program, fs);
    glLinkProgram(program); //Linking combines them
    glValidateProgram(program); //Checks linking errors
    glDeleteShader(vs); //Can Delete compiled shader objects now
    glDeleteShader(fs);
    return program; //Returns program ID
}
