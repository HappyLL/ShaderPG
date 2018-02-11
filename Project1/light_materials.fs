#version 330 core

in vec3 frag_pos;
in vec3 frag_normal;
in vec3 light_pos2;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 speculum;
	float shiness;
};

uniform vec3 camera_pos;
uniform vec3 light_color;

uniform Material cube_material;

out vec4 frag_color;

void main(){
	vec3 ambient = cube_material.ambient * light_color;
	vec3 light_dir = normalize(light_pos2 - frag_pos);
	vec3 view_dir = normalize(-frag_pos);

	float diffuse_strength = max(dot(light_dir, frag_normal), 0);
	vec3 diffuse = light_color * (diffuse_strength * cube_material.diffuse);
	vec3 view_reflect = reflect(-light_dir, frag_normal);
	float spec = pow(max(dot(view_dir, view_reflect), 0.0), cube_material.shiness);
	vec3 specular = light_color * (spec * cube_material.speculum);  
	vec3 result = ambient + diffuse + specular;
	frag_color = vec4(result, 1.0);
}