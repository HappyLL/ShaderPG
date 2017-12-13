#version 330 core
in vec2 texture_pos;
// Œ∆¿Ì–≈œ¢?
uniform texture;
out vec4 color;
void main(){
	color = vec4(frag_info.x, frag_info.y, frag_info.z, 1);
}
