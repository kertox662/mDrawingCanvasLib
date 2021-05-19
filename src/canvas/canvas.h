#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_set>
#include "shader.h"

//Forward declaration of the window from the GLFW library
struct GLFWwindow;

namespace mdcl{

//Forward Declarations
class Image;
class Font;
class Color;
class Shape;
enum TextAlignment : unsigned char;

struct WindowDimensions{
	int width, height;
};

class Canvas{
	GLFWwindow* _window;
	WindowDimensions _winDim;
	std::string _title;
	bool _completed;
	int _frameRate;
	std::vector<Shape*> _openShapes;
	Color *_fill, *_outline;
	float _outlineThickness;
	Shader _shapeShader, _ellipseShader;
	
	static std::vector<std::thread> _s_canvasThreads;
	static bool _s_glfwInitialized;
	static std::unordered_set<Canvas*> _s_windows;
	static std::mutex _s_destructionMutex;

	public:
	Canvas(int width, int height, std::string title);
	Canvas(WindowDimensions dim, std::string title);
	// ~Canvas();

	//Properties
	inline WindowDimensions getDimensions() const{ return _winDim;}
	inline int getWidth() const{ return _winDim.width;}
	inline int getHeight() const{return _winDim.height;}
	inline std::string getTitle() const{return _title;}
	
	void setDimensions(WindowDimensions);
	void setWidth(int);
	void setHeight(int);
	void setTitle(std::string);

	private:
	//Runs the first time a canvas is created
	static void initializeCanvases();
	void compileShaders();

	protected:
	//Main function definitions for the canvas.
	//Init runs once, loop runs until canvas is stopped.
	//Cleanup runs after loop ends.
	virtual void init(){}
	virtual void loop(){}
	virtual void cleanup(){}

	//Event Handlers
	virtual void mouseMoved(){}
	virtual void mousePressed(){}
	virtual void mouseReleased(){}
	virtual void keyPressed(){}
	virtual void keyReleased(){}
	virtual void onFocus(){}
	virtual void onUnfocus(){}

	//Drawing functions
	void rect(float x, float y, float width, float height);
	void rect(std::pair<float,float> p1, std::pair<float,float> p2);
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void triangle(std::pair<float,float> p1, std::pair<float,float> p2, std::pair<float,float> p3);

	void ellipse(float x, float y, float xRadius, float yRadius);
	void ellipse(std::pair<float,float> p1, std::pair<float,float> p2);
	void circle(float x, float y, float radius);

	void line(float x1, float y1, float x2, float y2);
	void line(std::pair<float,float> p1, std::pair<float,float> p2);

	void text(std::string text, float x, float y);
	void image(Image img, float x, float y);

	void beginShape();
	void endShape();
	void vertex(float x, float y);

	//Colour Functions
	void clear(Color*);
	void setFill(Color*);
	void setOutline(Color*);

	//Text/Font stuff
	void textAlign(TextAlignment);
	void setFont(Font);

	//Static functions for starting the Canvases
	void canvasLoop();
	public:
	// static void startCanvas(Canvas& canv);
	static void startCanvas(Canvas* canv);
	static void runEventLoop();

};

} //namespace mdcl

#endif //CANVAS_H
