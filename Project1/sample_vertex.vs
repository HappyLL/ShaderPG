#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
out vec4 fgg_Color;
uniform float ani_pos_x;
void main(){
	gl_Position = vec4(pos.x, pos.y, pos.z, 1);
	fgg_Color = vec4(ani_pos_x, ani_pos_x, ani_pos_x, 1);
}
