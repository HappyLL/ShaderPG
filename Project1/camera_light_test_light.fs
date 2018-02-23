#version 330 core
out vec4 Frag_Color;

struct Light{
	// 投光物: 该光是平行光
	vec3 light_dir;
	// 投光物: 点光源
	vec3 light_pos;
	float c;
	float l;
	float q;
	//详见公式:
	//https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
	vec3 light_color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main(){
	Frag_Color = vec4(light.light_color, 1.0);
}