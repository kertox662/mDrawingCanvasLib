#include "canvas.h"
#include "font.h"
#include "exceptions.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION 1

using Canvas = mdcl::Canvas;


Canvas::Canvas(int width, int height, std::string title) : _title(title){
	this->_winDim = {width, height};
	if(!Canvas::_s_glfwInitialized){
		Canvas::initializeCanvases();
	}

	_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}



Canvas::Canvas(mdcl::WindowDimensions dim, std::string title): _winDim(dim), _title(title){
	if(!Canvas::_s_glfwInitialized){
		Canvas::initializeCanvases();
	}
	_window = glfwCreateWindow(dim.width, dim.height, title.c_str(), NULL, NULL);
}



void Canvas::initializeCanvases(){
	if(_s_glfwInitialized){
		throw mdcl::AlreadyInitializedException();
	}
	if(!glfwInit()){
		throw mdcl::GLFWInitFailureException();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //The window hints start window with specific OpenGL Context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	_s_glfwInitialized = true;
}


