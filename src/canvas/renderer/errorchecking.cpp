// Code taken from The Cherno: https://www.youtube.com/watch?v=FBbPWSOQ0-w&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=10

#include "GLCommon.h"
#include "errorchecking.h"
#include <unordered_map>
#include <iostream>

std::unordered_map<unsigned int,std::string> errorCodes; //A map from the Error Codes from glGetError to a string representation

void GLGenerateErrorMap(){ //Populates errorCodes
    errorCodes[GL_INVALID_ENUM]                  = "Invalid Enum";
    errorCodes[GL_INVALID_VALUE]                 = "Invalid Value";
    errorCodes[GL_INVALID_OPERATION]             = "Invalid Operation";
    errorCodes[GL_INVALID_FRAMEBUFFER_OPERATION] = "Invalid FrameBuffer Operation";
    errorCodes[GL_OUT_OF_MEMORY]                 = "Out of Memory";
}

void GLClearError(){ //Gets rid of all of the errors from glGetError
    while(glGetError());
}

bool GLCheckError(std::string label){ //Prints out all of the errors from glGetError
    bool gotError = false;
    while(GLenum err = glGetError()){
        gotError = true;
        std::cout << label << "|OpenGL Error| (" << errorCodes[err] << ")" << std::endl;
    }
    return gotError;
}
