#version 330 core
in vec2 texture_pos;
//Ƭ����ɫ����Ĵ��������������������� ��ȥ�ɼ���Ӧ����Ԫ������
//����Ԫ�ڰ���������ǰ��Ҫ���ǰ������Ԫ,��ȥ���󶨵����ݰ󶨴�Ŀ����
uniform sampler2D texture_image;

out vec4 color;
void main(){
	color = texture(texture_image, texture_pos);
}
