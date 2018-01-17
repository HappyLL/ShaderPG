#version 330 core
// �����in��ʾ�ж�������
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//��Դλ��(����ռ�����ϵ��)
uniform vec3 light_pos;
//���λ��
uniform vec3 camera_pos;

out float diffuse_strength;
out float projection_strength;

void main(){
	gl_Position = projection * view * model * vec4(pos, 1);
	vec3 world_ob_pos = vec3(model * vec4(pos, 1));
	vec3 world_normal = normalize(mat3(model) * normal);
	// diffuse_strength �ǵ�λ���������
	diffuse_strength = max(dot(normalize(light_pos - world_ob_pos), world_normal), 0);
	vec3 world_reflect = reflect((world_ob_pos - light_pos), world_normal);
	projection_strength = max(dot(normalize(world_reflect), normalize(camera_pos - world_ob_pos)), 0);
}