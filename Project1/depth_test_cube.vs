#version 330 core
layout(location = 0) in vec3 cube_pos;
layout(location = 1) in vec2 tex_pos;

uniform mat4 mmodel;
uniform mat4 mview;
uniform mat4 mprojection;

out vec2 f_tex_pos;

void main(){
	gl_Position = mprojection * mview * mmodel * vec4(cube_pos, 1.0);
	f_tex_pos = tex_pos;
}