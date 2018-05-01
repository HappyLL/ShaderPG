#version 330 core

in vec3 out_cube_color;
out vec4 frag_color;

void main(){
	frag_color = vec4(out_cube_color, 1.0);
}