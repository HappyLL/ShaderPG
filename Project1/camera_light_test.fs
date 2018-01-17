#version 330 core

in float diffuse_strength;
in float projection_strength;

out vec4 frag_color;

uniform vec3 cube_color;
uniform vec3 light_color;


void main(){

	//环境光
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;
	//漫反射
	vec3 diffuse = diffuse_strength * light_color; 
	//镜面反射
	vec3 projection = pow(projection_strength, 32) * light_color * 0.5;

	// 冯氏模型 = 漫反射向量 + 环境光 + 镜面反射
	frag_color = vec4(cube_color * (ambient + diffuse + projection) , 1.0);
}