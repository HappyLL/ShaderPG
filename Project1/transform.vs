#version 330 core
layout (location = 0) in vec3 v_pos;
out vec3 frag_info;
void main(){
	gl_Position = vec4(v_pos.x, v_pos.y, v_pos.z, 1);
	frag_info = vec3(v_pos.x, v_pos.y, v_pos.z);
}