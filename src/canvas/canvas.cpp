#include "canvas.h"
#include "font.h"
#include "exceptions.h"
#include "shape.h"
#include "errorchecking.h"
#include "color.h"
#include "shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <unistd.h>

#define GL_SILENCE_DEPRECATION 1

using Canvas = mdcl::Canvas;

// Initialized static members
bool Canvas::_s_glfwInitialized;
std::vector<std::thread> Canvas::_s_canvasThreads;
std::unordered_set<Canvas*> Canvas::_s_windows;
std::mutex Canvas::_s_destructionMutex;


// Constructors
Canvas::Canvas(int width, int height, std::string title) : 
	_title(title), _completed(false), _frameRate(200){
		this->_winDim = {width, height};
	}

Canvas::Canvas(mdcl::WindowDimensions dim, std::string title): 
	_winDim(dim), _title(title), _completed(false), _frameRate(200){}


//Sets up data for glfw windows and makes error map
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
	GLGenerateErrorMap();

	_s_glfwInitialized = true;
}


//Creates window and thread for the canvas.
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


//Compiles the shaders for the canvas.
void Canvas::compileShaders(){ 
	_shapeShader.Create(getShapeVertShader(), getShapeFragShader());
	_ellipseShader.Create(getEllipseVertShader(), getEllipseFragShader());
}


//Runs the loop for the canvas.
void Canvas::canvasLoop(){
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::microseconds;


	//Makes a openGL context
	//Important to do before compiling shaders
	glfwMakeContextCurrent(_window);
	int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
	if(gladErr == 0){
		throw GLADInitFailureException();
	}

	//Does some setup for the canvas
	compileShaders();
	init();
	int frames = 60, interval = 1000000/_frameRate;
	auto tp = high_resolution_clock::now(); //Time Point

	// Keep looping while the window be open.
	while(!glfwWindowShouldClose(_window)){
		loop();
		if(!_openShapes.empty()){ //Error if some beginShape() doesn't have complementary endShape();
			std::cout << "Canvas " << _title << ": Begin shape ran without endShape" << std::endl;
			break;
		}
		glfwSwapBuffers(_window); //Show Render;

		// Wait for end of interval (based on framerate)
		auto end = high_resolution_clock::now();
		duration<double> dur = end-tp;
		auto d = duration_cast<microseconds>(dur);
		// std::cout << "Thread Time:" << d.count() << "us" << std::endl;
		if(interval > d.count())
			usleep(interval - d.count());
		tp = end;
	}
	
	// Cleans up anything that needs it.
	cleanup();
	_completed = true;
}


// runEventLoop is a blocking function that will send events to the registered
// Canvases while all of them are running.
void Canvas::runEventLoop(){
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::microseconds;


	int time = 1000000, frames = 200, interval = time/frames; //Maximum time between polls
	auto tp = high_resolution_clock::now();

	while(_s_windows.size() > 0){ //While there is still a canvas running.
		std::unordered_set<Canvas*> toRemove;
		for(auto it = _s_windows.begin(); it != _s_windows.end(); it++){ //Checks to see which canvases are done
			if((*it)->_completed){
				toRemove.insert(*it);
				continue;
			}
			// Polls events for all canvases that are not complete
			glfwMakeContextCurrent((*it)->_window);
			glfwPollEvents();
		}
		//Deletes all complete canvases.
		for(auto it = toRemove.begin(); it != toRemove.end(); it++){
			glfwDestroyWindow((*it)->_window);
			_s_windows.erase(*it);
		}

		//Waits for time until interval finishes. (Not Sure if Needed)
		// auto end = high_resolution_clock::now();
		// duration<double> dur = end-tp;
		// auto d = duration_cast<microseconds>(dur);
		// // std::cout << "Time:" << d.count() << "us" << std::endl;
		// if(interval > d.count())
		// 	usleep(interval - d.count());
		// tp = end;
	}

	//When all canvases are done, wait for threads to finish and terminate glfw.
	for(size_t i = 0; i < _s_canvasThreads.size(); i++)
		_s_canvasThreads[i].join();
	glfwTerminate();
}