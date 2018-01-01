#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
void main()
{
	//环境光照参数
	float ambientStrength = 0.1;
	//镜面反射强度
	float specularStrength = 1.0;
	vec3 ambient = ambientStrength * lightColor;
	vec3 nor = normalize(Normal);
	//fragPos 是世界坐标系下的
	vec3 light_dir = normalize(FragPos - lightPos);
	vec3 view_dir = normalize(viewPos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, nor);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	vec3 diffuse = max(dot(nor, light_dir), 0) * lightColor;
    FragColor = vec4((ambient + diffuse + specular)* objectColor, 1.0);
}
