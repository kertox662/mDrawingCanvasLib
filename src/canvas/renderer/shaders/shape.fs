#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_Color;
in vec2 v_boundBoxPos;

void checkFragment() {
    float d  = dot( v_boundingBoxPosition , v_boundingBoxPosition ); 
    if( d > 1 ) {
       discard;
    }    
}

void main(){
    checkFragment();
    color = u_Color;
}