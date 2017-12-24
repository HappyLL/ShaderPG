#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 t_pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 texture_pos;
void main(){
	gl_Position =  projection * view * model * vec4(v_pos, 1.0);
	texture_pos = vec2(t_pos.x, t_pos.y);
}