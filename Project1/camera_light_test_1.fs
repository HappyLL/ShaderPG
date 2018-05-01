#version 330 core

in vec3 frag_pos;
in vec3 frag_normal;

uniform vec3 camera_pos;
uniform vec3 light_pos;
uniform vec3 cube_color;
uniform vec3 light_color;

out vec4 frag_color;

void main(){
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;
	float diffuse_strength = max(dot(normalize(light_pos - frag_pos), frag_normal), 0);
	// 散射少 反射多就会形成亮点
	vec3 diffuse = diffuse_strength * light_color * 0.1;
	vec3 view_reflect = reflect((frag_pos - light_pos), frag_normal);
	float projection_strength = max(dot(normalize(view_reflect), normalize(-frag_pos)), 0);
	vec3 projection = pow(projection_strength, 32) * light_color * 0.5;
	frag_color = vec4(cube_color * (ambient + diffuse + projection) , 1.0);
}