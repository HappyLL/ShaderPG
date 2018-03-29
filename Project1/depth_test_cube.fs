#version 330 core
in vec2 f_tex_pos;

uniform sampler2D t0;

out vec4 fg_Color;

void main(){
	fg_Color = texture(t0, f_tex_pos);
}