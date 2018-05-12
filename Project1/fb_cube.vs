#version 330 core
layout (location = 0) in vec3 cube_pos;
layout (location = 1) in vec2 cube_tex_pos;
out vec2 f_tex_pos;
void main(){
	gl_Position = vec4(cube_pos, 1);
	f_tex_pos = cube_tex_pos;
}