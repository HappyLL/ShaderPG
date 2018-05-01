#version 330 core

struct Material{
	// 漫反射贴图在显存中的索引
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	// 投光物: 该光是平行光(聚光灯的方向)
	vec3 light_dir;
	// 投光物: 点光源(聚光灯)
	vec3 light_pos;
	float c;
	float l;
	float q;
	//详见公式(衰减公式的常数含义):
	//https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
	// 投光物: 聚光灯
	float cutoff;//落在聚光范围内的余弦值
	vec3 light_color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 o_frag_pos;
in vec3 o_frag_normal;
in vec2 o_texture_pos;

uniform Material material;
uniform Light light;

out vec4 frag_Color;

void main(){
	//平行光
	//vec3 light_dir = normalize(-light.light_dir);
	// 点光源
	vec3 light_dir1 = light.light_pos - o_frag_pos;
	//float dis = length(light_dir1);
	//float attention = 1.0 / (light.c + light.l * dis + light.q * dis * dis);
	vec3 light_color = light.light_color;
	// 环境光照
	vec3 ambition = vec3(texture(material.diffuse, o_texture_pos)) * light_color * light.ambient;
	// 漫反射光照
	// 点光源
	vec3 light_dir = normalize(light_dir1);
	float frag_cutoff = dot(light_dir, normalize(-light.light_dir));
	// 聚光灯
	if (frag_cutoff >= light.cutoff){
		float diff = max(dot(light_dir, o_frag_normal), 0);
		vec3 diffuse_color = vec3(texture(material.diffuse, o_texture_pos));
		vec3 diffuse = diffuse_color * diff * light_color * light.diffuse;
		// 镜面光照
		vec3 view_dir = normalize(-o_frag_pos);
		vec3 reflect_light_dir = reflect(-light_dir, o_frag_normal);
		vec3 specular_color = vec3(texture(material.specular, o_texture_pos));
		//specular_color = vec3(1.0 - specular_color.x, 1.0 - specular_color.y, 1.0 - specular_color.z);
		vec3 specular = pow(max(dot(reflect_light_dir, view_dir), 0), material.shininess) * specular_color * light_color * light.specular;
		frag_Color = vec4(ambition + diffuse + specular, 1.0);
	}
	else{
		frag_Color = vec4(ambition, 1.0);
	}
	//specular *= 2.0;
	//frag_Color = vec4(ambition + diffuse * attention + specular * attention, 1.0);
}