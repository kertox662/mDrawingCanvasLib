#include "canvas.h"
#include "font.h"
#include "exceptions.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <unistd.h>

#define GL_SILENCE_DEPRECATION 1

using Canvas = mdcl::Canvas;

bool Canvas::_s_glfwInitialized;
std::vector<std::thread> Canvas::_s_canvasThreads;
std::unordered_set<Canvas*> Canvas::_s_windows;
std::mutex Canvas::_s_destructionMutex;

Canvas::Canvas(int width, int height, std::string title) : 
	_title(title), _completed(false), _frameRate(60)
	{this->_winDim = {width, height};}



Canvas::Canvas(mdcl::WindowDimensions dim, std::string title): 
	_winDim(dim), _title(title), _completed(false), _frameRate(60){}



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



void Canvas::startCanvas(Canvas* canv){
	if(!Canvas::_s_glfwInitialized){
		Canvas::initializeCanvases();
	}
	canv->_window = glfwCreateWindow(canv->getWidth(), canv->getHeight(), canv->getTitle().c_str(), NULL, NULL);
	_s_windows.insert(canv);

	_s_canvasThreads.push_back(
		std::thread (
			[canv]{canv->canvasLoop();}
		)
	);
}



void Canvas::canvasLoop(){
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::microseconds;


	glfwMakeContextCurrent(_window);
	int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
	if(gladErr == 0){
		throw GLADInitFailureException();
	}

	init();
	int frames = 60, interval = 1000000/frames;
	auto tp = high_resolution_clock::now();

	while(!glfwWindowShouldClose(_window)){
		loop();
		glfwSwapBuffers(_window);

		auto end = high_resolution_clock::now();
		duration<double> dur = end-tp;
		auto d = duration_cast<microseconds>(dur);
		// std::cout << "Thread Time:" << d.count() << "us" << std::endl;
		if(interval > d.count())
			usleep(interval - d.count());
		tp = end;
	}
	cleanup();
	
	_completed = true;
}



void Canvas::runEventLoop(){
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::microseconds;


	int frames = 60, interval = 1000000/frames;
	auto tp = high_resolution_clock::now();

	while(_s_windows.size() > 0){
		std::unordered_set<Canvas*> toRemove;
		for(auto it = _s_windows.begin(); it != _s_windows.end(); it++){
			if((*it)->_completed){
				toRemove.insert(*it);
				continue;
			}
			glfwMakeContextCurrent((*it)->_window);
			glfwPollEvents();
		}
		for(auto it = toRemove.begin(); it != toRemove.end(); it++){
			glfwDestroyWindow((*it)->_window);
			_s_windows.erase(*it);
		}
		auto end = high_resolution_clock::now();
		duration<double> dur = end-tp;
		auto d = duration_cast<microseconds>(dur);
		// std::cout << "Time:" << d.count() << "us" << std::endl;
		if(interval > d.count())
			usleep(interval - d.count());
		tp = end;
	}

	for(size_t i = 0; i < _s_canvasThreads.size(); i++)
		_s_canvasThreads[i].join();
	glfwTerminate();
}