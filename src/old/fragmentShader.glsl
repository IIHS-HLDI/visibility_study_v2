#version 330 core
 
in vec3 fragment_color;
out vec3 color_out;
 
void main(){
    color_out = fragment_color;
}