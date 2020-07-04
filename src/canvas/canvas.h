#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include <vector>
#include <thread>

//Forward declaration of the window from the GLFW library
struct GLFWwindow;

namespace mdcl{

//Forward Declarations
class Image;
class Font;
class Color;
enum TextAlignment : unsigned char;

struct WindowDimensions{
	int width, height;
};

class Canvas{
	GLFWwindow* _window;
	WindowDimensions _winDim;
	std::string _title;

	static std::vector<std::thread> _s_canvasThreads;
	static bool _s_glfwInitialized;

	public:
	Canvas(int width, int height, std::string title);
	Canvas(WindowDimensions dim, std::string title);

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

	protected:
	//Main function definitions for the canvas.
	//Init runs once, loop runs until canvas is stopped.
	inline virtual void init(){}
	inline virtual void loop(){}

	//Event Handlers
	inline virtual void mouseMoved(){}
	inline virtual void mousePressed(){}
	inline virtual void mouseReleased(){}
	inline virtual void keyPressed(){}
	inline virtual void keyReleased(){}
	inline virtual void onFocus(){}
	inline virtual void onUnfocus(){}

	//Static functions for starting the Canvases
	static void startCanvas();
	static void runEventLoop();

	//Drawing functions
	void rect(double x, double y, double width, double height);

	void ellipse(double x, double y, double xRadius, double yRadius);

	void circle(double x, double y, double radius);

	void line(double x1, double y1, double x2, double y2);

	void text(std::string text, double x, double y);

	void image(Image img, double x, double y);

	void beginShape();
	void endShape();
	void vertex(double x, double y);

	//Colour Functions
	void clear(Color);
	void setFill(Color);
	void setOutline(Color);

	//Text/Font stuff
	void textAlign(TextAlignment);
	void setFont(Font);

};

} //namespace mdcl

#endif //CANVAS_H
