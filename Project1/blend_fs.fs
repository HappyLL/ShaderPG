#version 330 core
uniform sampler2D t0;
in vec2 f_tex_pos;
out vec4 f_Color;
void main(){
	vec4 tex_color = texture(t0, f_tex_pos);
	f_Color = vec4(tex_color.r, tex_color.g, tex_color.b, 0);
}