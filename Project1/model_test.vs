#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 o_texture_pos;
out vec3 o_normal;
out vec3 o_frag_pos;

void main(){
	gl_Position = projection * view * model * vec4(pos, 1);
	o_normal = normalize(mat3(transpose(inverse(model))) * normal);
	o_texture_pos = texture_pos;
	o_frag_pos = vec3(view * model * vec4(pos, 1));
}