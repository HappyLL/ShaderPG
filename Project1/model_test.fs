#version 330 core

struct Material{
	 sampler2D texture_diffuse1;
	 sampler2D texture_diffuse2;
	 sampler2D texture_diffuse3;
	 sampler2D texture_diffuse4;
	 sampler2D texture_specular1;
	 sampler2D texture_specular2;
	 sampler2D texture_specular3;
	 sampler2D texture_specular4;
	 float shininess;
};

in vec3 o_frag_pos;
in vec2 o_texture_pos;
in vec3 o_normal;

uniform Material material;
uniform vec3 light_color;
uniform vec3 light_pos;

out vec4 frag_color;

void main(){
	vec3 ambition = vec3(texture(material.texture_diffuse1, o_texture_pos)) * light_color;
	// 漫反射光照
	vec3 diffuse_color = vec3(texture(material.texture_diffuse1, o_texture_pos));
	vec3 light_dir = normalize(light_pos - o_frag_pos);
	float diff = max(dot(light_dir, o_normal), 0);
	vec3 diffuse = diffuse_color * diff * light_color;
	// 镜面光照
	vec3 view_dir = normalize(-o_frag_pos);
	vec3 reflect_light_dir = reflect(-light_dir, o_normal);
	vec3 specular_color = vec3(texture(material.texture_specular1, o_texture_pos));
	//specular_color = vec3(1.0 - specular_color.x, 1.0 - specular_color.y, 1.0 - specular_color.z);
	vec3 specular = pow(max(dot(reflect_light_dir, view_dir), 0), material.shininess) * specular_color * light_color;
	//specular *= 2.0;
	frag_color = vec4(ambition + diffuse + specular, 1.0);
}