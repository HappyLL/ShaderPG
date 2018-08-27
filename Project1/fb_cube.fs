#version 330 core
in vec2 f_tex_pos;
uniform sampler2D t0;
out vec4 f_Color;
void main(){
	f_Color = texture(t0, f_tex_pos);
}