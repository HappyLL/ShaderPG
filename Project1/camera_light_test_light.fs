#version 330 core
out vec4 Frag_Color;
//光源的颜色
uniform vec3 light_color;


void main(){
	Frag_Color = vec4(light_color * vec3(1.0), 1.0);
}