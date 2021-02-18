#include "canvas.h"
#include "color.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using Canvas = mdcl::Canvas;

void Canvas::clear(mdcl::Color* c){
    std::array<unsigned char, 4> color = c->getRGBA();
    glClearColor(color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, color[3]/255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}