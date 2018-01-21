#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;
out vec3 frag_normal;

void main(){
	gl_Position = projection * view * model * vec4(pos, 1);
	frag_pos = vec3(view * model * vec4(pos, 1));
	frag_normal = mat3(view) * mat3(model) * normal;
}