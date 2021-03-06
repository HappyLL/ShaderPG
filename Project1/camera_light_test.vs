#version 330 core
// 必须加in表示有顶点输入
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//光源位置(世界空间坐标系下)
uniform vec3 light_pos;
//相机位置
uniform vec3 camera_pos;

uniform vec3 cube_color;
uniform vec3 light_color;

out vec3 out_cube_color;

void main(){
	gl_Position = projection * view * model * vec4(pos, 1);
	vec3 world_ob_pos = vec3(view * model * vec4(pos, 1));
	vec3 world_normal = normalize(mat3(view) * mat3(model) * normal);
	// diffuse_strength 是单位法向量相乘
	float diffuse_strength = max(dot(normalize(light_pos - world_ob_pos), world_normal), 0);
	vec3 world_reflect = reflect((world_ob_pos - light_pos), world_normal);
	float projection_strength = max(dot(normalize(world_reflect), normalize(-world_ob_pos)), 0);
	//环境光
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;
	//漫反射
	vec3 diffuse = diffuse_strength * light_color; 
	//镜面反射
	vec3 projection = pow(projection_strength, 32) * light_color * 0.05;
	// 冯氏模型 = 漫反射向量 + 环境光 + 镜面反射
	out_cube_color = cube_color * (ambient + diffuse + projection);
}