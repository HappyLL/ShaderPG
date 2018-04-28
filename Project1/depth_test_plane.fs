#version 330 core
in vec2 f_tex_pos;

uniform sampler2D t0;

out vec4 fg_Color;

float near = 1f;
float far = 100.0f;

float LinearizeDepth(float depth){
	float z = depth * 2.0 - 1.0;
	return (2 * near * far) / (far + near - z * (far - near));
}

void main(){
	//gl_FragCoord.z [0, 1]
	// πÈ“ªªØ
	//float depth = LinearizeDepth(gl_FragCoord.z) / far;
	fg_Color = texture(t0, f_tex_pos);//vec4(vec3(depth), 1.0);//texture(t0, f_tex_pos);
}