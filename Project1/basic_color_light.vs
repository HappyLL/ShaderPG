#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main(){
	gl_Position =  projection * view * model * vec4(v_pos, 1.0);
	Normal = v_normal;
	vec4 ret = model * vec4(v_pos, 1.0);
	FragPos = vec3(ret.x, ret.y , ret.z);
}