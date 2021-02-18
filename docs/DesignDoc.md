<style>
  h1{
	color:#54afbc;
  }

  h2{
	color:#70c19A
  }

  h3{
	color:#ffb449
  }
</style>

# mDrawingCanvasLib
### Draft Version  <ins> _0.0.1_ </ins>
## Objective
mDrawingCanvasLib (mdcl) is a library with the aim to make graphics in C++ easier. The main inspiration for this is the Processing Language from the Processing Foundation. The goal is to give users using the library an interface simple but effective tools for drawing graphics. As it is now, many libraries feel very verbose in their structure when even making a simple app. This library will try to eliminate the steps required to make a viable app, but still give the option to interact with the windows at a lower level.

## Dependencies
This project will need some other libraries for giving the interface to the graphics aspects:
* GLFW - The main library for graphics windows and an OpenGL context.
* GLAD/GLEW (unsure) - Allows for loading of OpenGL api functions.
* GLM - A mathematics library for use with OpenGL.
* stb - Mainly for images, maybe for other tasks too.
* FreeType - Fonts and text rendering

## Structure
### Basic Structure
All components of *mdcl* will be part of the "mdcl" namespace.

*mdcl* will define a base class (*bc*) for a Canvas. To use the Canvas, users will be expected to define a custom class inheriting from the Canvas *bc* and override certain functions. The functions will be responsible for a initialization of the Canvas and defining the drawing functions for the render loop. The derived canvases will then be constructed and queued up into a list for order. Finally, the user will run a static run/execute method which will first run all of the init/setup methods in the derived classes, followed by an infinite loop where each canvas is drawn to.

### Making the Canvases
The library will provide a base class for the user to make derived Canvases from. It will provide the drawing functions for basic shapes like ellipses, rectangles, and lines. In addition, it will allow for text and images to be displayed. The user will then make a derived class from this base class. To be able to define functionality, the user will rewrite the *init* and *loop* (names subject to change). These functions will then be run when the user runs the static *run* method.

The methods of the base classes will be public/protected (likely public).

It will have multithreaded support (and will probably automatically be multithreaded) with certain restrictions. Firstly, the Canvases must be made on the main thread because GLFW only allows window creation on that thread. Secondly, GLFW only allows event polling from the main thread, therefore a polling loop will need to block that thread, probably from the static *run* method.

### Classes Available to the User
The following classes will be part of the mdcl namespace, used for various storage
* Image - For loading and displaying images such as JPEGs and PNGs (Unsure about SVG images).
* Font - For loading fonts in which text can be displayed
* Vector - Math stuff, mostly for the user
* Matrix - Math and transformations for the user. Good for storing transformations that can be loaded back later (Matrix stack).

### Drawing Functionality

Current plans involve 2D drawing capabilities. It is planned to be split into basic primitives, and images, which involve the Image class. The drawn features will include the following:
* Rectangle
* Lines
* Circles/Ellipses
* Custom shapes from user coordinates
* Text
* Images
* Various curves and arcs

These features (apart from images) will have 2 different colours, a fill colour, and an outline colour, which will be modified through separate functions.

Achieving this functionality will be done through implmenting different shaders for each feature. The only concerns currently are shaders for the lines and curves.

Drawing will be allocated to their own threads. First the canvases will be made on the main thread. Next the user will pass them in, either a pointer or a reference (maybe one will be forced?), to a function that starts the thread. What this thread will do is first run the setup/init method of the canvas, then it will run an infinite render loop which calls the Canvas' draw method. The render loop can be temporarily stopped with a method that stops looping, or permanently stopped with some kind of exit method.

An event loop will need to block the main thread. This loop will poll for events to send to the Canvases as long as there is still at least one active.

In addition, it will be attempted to allow the user to modify the pixels

### Events
GLFW can emit many events, which this library will capture. The Canvas will have custom defined functions that will act as callbacks when the events are emitted. The events that will have functions associated with them are the following, subject to change:
 * Mouse Movement
 * Mouse Click/Press
 * Key Press
 * Window Focus?

### Canvas Options
The Canvas Base class will have certain properties that will be able to be modified:
  * Activated/Visible - Will determine whether a window is able to be seem on the screen. If it is false, then the window is hidden.
  * Framerate - The maximum amount of frames per second that the Canvas will run and update at.
  * Window Size?
  * Window Position
  * Cursor Style - Either pre-defined cursors or custom cursors loaded from a file

## Possible Additional Features
The following features may be implemented later down the line, but no plans are currently made as to how that's going to be done:
* Sound
* Simple 3D drawing
* Sprite Sheets
* Model Loading
* Saving canvas to pngs
