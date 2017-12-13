#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 t_pos;
out vec2 texture_pos;
void main(){
	gl_Position = vec4(v_pos.x, v_pos.y, v_pos.z, 1);
	texture_pos = t_pos;
}