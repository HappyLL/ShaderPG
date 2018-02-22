#version 330 core

struct Material{
	// 漫反射贴图在显存中的索引
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec3 o_frag_pos;
in vec3 o_frag_normal;
in vec2 o_texture_pos;

uniform Material material;
uniform vec3 light_color;
uniform vec3 light_pos;

out vec4 frag_Color;

void main(){
	// 环境光照
	vec3 ambition = vec3(texture(material.diffuse, o_texture_pos)) * light_color;
	// 漫反射光照
	vec3 diffuse_color = vec3(texture(material.diffuse, o_texture_pos));
	vec3 light_dir = normalize(light_pos - o_frag_pos);
	float diff = max(dot(light_dir, o_frag_normal), 0);
	vec3 diffuse = diffuse_color * diff * light_color;
	// 镜面光照
	vec3 view_dir = normalize(-o_frag_pos);
	vec3 reflect_light_dir = reflect(-light_dir, o_frag_normal);
	vec3 specular = pow(max(dot(reflect_light_dir, view_dir), 0), material.shininess) * vec3(texture(material.specular, o_texture_pos)) * light_color;
	frag_Color = vec4(ambition + diffuse + specular, 1.0);
}