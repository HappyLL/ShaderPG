#version 330 core

in vec3 frag_pos;
in vec3 frag_normal;

struct material{
	vec3 ambient;
	vec3 diffuse;
	vec3 speculum;
	float shiness;
};

uniform vec3 camera_pos;
uniform vec3 light_pos;
uniform vec3 light_color;

uniform material cube_material;

out vec4 frag_color;

void main(){
	vec3 ambient = cube_material.ambient * light_color;
	vec3 light_dir = normalize(light_pos - frag_pos);

	float diffuse_strength = max(dot(light_dir, frag_normal), 0);
	vec3 diffuse = (diffuse_strength * cube_material.diffuse) * light_color;
	vec3 view_reflect = normalize(reflect(-light_dir, frag_normal));
	float projection_strength = max(dot(view_reflect, normalize(-frag_pos)), 0);
	vec3 projection = (pow(projection_strength, cube_material.shiness) *  cube_material.speculum) * light_color;
	frag_color = vec4(ambient+ diffuse + projection , 1.0);
}