#include "canvas.h"
#include "color.h"
#include "shape.h"
#include "errorchecking.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>

using Canvas = mdcl::Canvas;

// Sets the whole canvas to the same color c.
void Canvas::clear(mdcl::Color* c){
    std::array<unsigned char, 4> color = c->getRGBA();
    glClearColor(color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, color[3]/255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// Sets the fill for shapes to c.
void Canvas::setFill(mdcl::Color* c){
    std::string name = "u_Color";
    auto rgba = c->getRGBA();
    std::array<float, 4> color = {(float)rgba[0]/255, (float)rgba[1]/255,(float)rgba[2]/255,(float)rgba[3]/255};
    _shapeShader.SetUniform<float,4>(name, color);
    _ellipseShader.SetUniform<float,4>(name,color);
}

// Sets the outline for shapes to c.
void Canvas::setOutline(mdcl::Color* c){_outline = c;}


// Starts a shape being drawn. endShape() must be run for each beginShape() before the end of the loop.
void Canvas::beginShape(){
    _openShapes.push_back(new Shape());
}

// Closes and renders the latest shape that was started with beginShape().
void Canvas::endShape(){
    if(_openShapes.size() == 0) return;
    Shape* s = _openShapes.back();
    _openShapes.pop_back();
    s->render(_winDim.width, _winDim.height, _shapeShader);
    delete s;
}

// Adds a vertex to the latest shape that was started with beginShape().
void Canvas::vertex(float x, float y){
    _openShapes.back()->add(x,y);
}

// Draws a rectangle with the top left corner at (x,y) with dimensions width and height.
void Canvas::rect(float x, float y, float width, float height){
    beginShape();
    vertex(x,y);
    vertex(x+width, y);
    vertex(x+width, y+height);
    vertex(x, y+height);
    endShape();
}

// Draws a rectangle with top left corner p1 and bottom right corner p2.
void Canvas::rect(std::pair<float,float> p1, std::pair<float,float> p2){
    beginShape();
    vertex(p1.first, p1.second);
    vertex(p2.first, p1.second);
    vertex(p2.first, p2.second);
    vertex(p1.first, p2.second);
    endShape();
}

// Draws a triangle with points (x1,y1),(x2,y2),(x3,y3).
void Canvas::triangle(float x1, float y1, float x2, float y2, float x3, float y3){
    beginShape();
    vertex(x1,y1);
    vertex(x2,y2);
    vertex(x3,y3);
    endShape();
}

// Draws a triangle with points p1,p2,p3.
void Canvas::triangle(std::pair<float,float> p1, std::pair<float,float> p2, std::pair<float,float> p3){
    beginShape();
    vertex(p1.first,p1.second);
    vertex(p2.first,p2.second);
    vertex(p3.first,p3.second);
    endShape();
}

// Draws an ellipse with center (x,y) with dimensions width,height.
void Canvas::ellipse(float x, float y, float width, float height){
    Shape ell;
    ell.add(x-width/2, y-height/2);
    ell.add(x+width/2, y-height/2);
    ell.add(x+width/2, y+height/2);
    ell.add(x-width/2, y+height/2);
    ell.render(_winDim.width, _winDim.height, _ellipseShader);
}

// Draws an ellipse that inscribes the rectangle with corners p1,p2.
void Canvas::ellipse(std::pair<float,float> p1, std::pair<float,float> p2){
    Shape ell;
    ell.add(p1.first, p1.second);
    ell.add(p2.first, p1.second);
    ell.add(p2.first, p2.second);
    ell.add(p1.first, p2.second);
    ell.render(_winDim.width, _winDim.height, _ellipseShader);
}


// Draws a circle with center (x,y) and radius of radius.
void Canvas::circle(float x, float y, float radius){
    ellipse(x,y,radius*2,radius*2);
}