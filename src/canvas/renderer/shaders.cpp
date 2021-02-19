#include "shaders.h"
std::string shapeVertexSource = "#version 330 core\n layout(location = 0) in vec4 position;\n void main(){ gl_Position = position;}";
std::string shapeFragmentSource = "#version 330 core\n layout(location = 0) out vec4 color;\n uniform vec4 u_Color;\n void main(){ color = u_Color; }";


// Ellipse Shaders from https://jvm-gaming.org/t/fill-ellipse-from-a-fragment-shader/53104/14
std::string ellipseVertexSource = "#version 330 core\nlayout(location = 0) in vec4 position; out vec2 v_boundBoxPos;void main(){if(gl_VertexID == 0){v_boundBoxPos = vec2(-1,1);} else if(gl_VertexID == 1){v_boundBoxPos = vec2(1,1);}else if(gl_VertexID == 2){v_boundBoxPos = vec2(1,-1);}else if(gl_VertexID == 3){v_boundBoxPos = vec2(-1,-1);}gl_Position = position;}";
std::string ellipseFragmentSource = "#version 330 core \n layout(location = 0) out vec4 color;uniform vec4 u_Color;in vec2 v_boundBoxPos;void checkFragment() {float d  = dot( v_boundBoxPos , v_boundBoxPos ); if( d > 1 ) {discard;}}void main(){checkFragment();color = u_Color;}";

std::string& getShapeVertShader(){return shapeVertexSource;}
std::string& getShapeFragShader(){return shapeFragmentSource;}
std::string& getEllipseVertShader(){return ellipseVertexSource;}
std::string& getEllipseFragShader(){return ellipseFragmentSource;}