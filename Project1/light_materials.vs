#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_pos;

out vec3 frag_pos;
out vec3 frag_normal;
out vec3 light_pos2;

void main(){
	gl_Position = projection * view * model * vec4(pos, 1.0);
	frag_pos = vec3(view * model * vec4(pos, 1.0));
	vec3 Normal = mat3(transpose(inverse(model))) * normal;
	frag_normal = Normal;//normalize(mat3(view) * Normal);
	light_pos2 = vec3(view  * vec4(light_pos, 1));
}