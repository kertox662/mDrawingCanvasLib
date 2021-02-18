#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_set>

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
	bool _completed;
	int _frameRate;
	
	static std::vector<std::thread> _s_canvasThreads;
	static bool _s_glfwInitialized;
	static std::unordered_set<Canvas*> _s_windows;
	static std::mutex _s_destructionMutex;

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
