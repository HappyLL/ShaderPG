#version 330 core

in float diffuse_strength;
in float projection_strength;

out vec4 frag_color;

uniform vec3 cube_color;
uniform vec3 light_color;


void main(){

	//������
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;
	//������
	vec3 diffuse = diffuse_strength * light_color; 
	//���淴��
	vec3 projection = pow(projection_strength, 32) * light_color * 0.5;

	// ����ģ�� = ���������� + ������ + ���淴��
	frag_color = vec4(cube_color * (ambient + diffuse + projection) , 1.0);
}