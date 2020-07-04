#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

namespace mdcl {

struct GLFWInitFailureException : public std::exception {
    const char* what() const throw (){
        return "GLFW was not able to initialize.";
    }
};

struct AlreadyInitializedException : public std::exception {
    const char* what() const throw (){
        return "Canvas::initializeCanvases has already been run.";
    }
};

} //namespace mdcl

#endif //EXCEPTIONS_H