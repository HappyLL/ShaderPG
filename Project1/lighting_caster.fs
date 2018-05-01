#version 330 core

struct Material{
	// ��������ͼ���Դ��е�����
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	// Ͷ����: �ù���ƽ�й�(�۹�Ƶķ���)
	vec3 light_dir;
	// Ͷ����: ���Դ(�۹��)
	vec3 light_pos;
	float c;
	float l;
	float q;
	//�����ʽ(˥����ʽ�ĳ�������):
	//https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
	// Ͷ����: �۹��
	float cutoff;//���ھ۹ⷶΧ�ڵ�����ֵ
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
	//ƽ�й�
	//vec3 light_dir = normalize(-light.light_dir);
	// ���Դ
	vec3 light_dir1 = light.light_pos - o_frag_pos;
	//float dis = length(light_dir1);
	//float attention = 1.0 / (light.c + light.l * dis + light.q * dis * dis);
	vec3 light_color = light.light_color;
	// ��������
	vec3 ambition = vec3(texture(material.diffuse, o_texture_pos)) * light_color * light.ambient;
	// ���������
	// ���Դ
	vec3 light_dir = normalize(light_dir1);
	float frag_cutoff = dot(light_dir, normalize(-light.light_dir));
	// �۹��
	if (frag_cutoff >= light.cutoff){
		float diff = max(dot(light_dir, o_frag_normal), 0);
		vec3 diffuse_color = vec3(texture(material.diffuse, o_texture_pos));
		vec3 diffuse = diffuse_color * diff * light_color * light.diffuse;
		// �������
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