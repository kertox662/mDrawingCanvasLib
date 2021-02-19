#version 330 core

layout(location = 0) in vec2 position;
out vec2 v_boundBoxPos;

void main(){
    
    if(gl_VertexID == 0){
        v_boundBoxPos = vec2(-1,1);
    } else if(gl_VertexID == 1){
        v_boundBoxPos = vec2(1,1);
    }else if(gl_VertexID == 2){
        v_boundBoxPos = vec2(1,-1);
    }else if(gl_VertexID == 3){
        v_boundBoxPos = vec2(-1,-1);
    }
    gl_Position = position;
}