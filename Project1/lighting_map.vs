#version 330 core

layout(location = 0) in vec3 cube_pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 o_frag_pos;
out vec3 o_frag_normal;
out vec2 o_texture_pos;

void main(){
	vec4 frag_pos = view * model *vec4(cube_pos, 1.0);
	gl_Position = projection * frag_pos;
	o_frag_pos = vec3(frag_pos);
	o_texture_pos = texture_pos;
	o_frag_normal =  normalize(mat3(transpose(inverse(model))) * normal);
}