#version 330 core
in vec2 texture_pos;
//片段着色器里的代表纹理采样器纹理采样器 会去采集对应纹理单元的纹理
//纹理单元在绑定纹理数据前需要激活当前的纹理单元,再去将绑定的数据绑定带目标上
uniform sampler2D texture_image;

out vec4 color;
void main(){
	color = texture(texture_image, texture_pos);
}
