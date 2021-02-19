// Code taken from The Cherno: https://www.youtube.com/watch?v=FBbPWSOQ0-w&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=10

#ifndef ERROR_CHECKING_H //ERROR_CHECKING_H
#define ERROR_CHECKING_H

#include <string>

void GLGenerateErrorMap();
void GLClearError();
bool GLCheckError(std::string label = "");

#endif //ERROR_CHECKING_H