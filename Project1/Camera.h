#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
//�������Ҫ����
//wasdʱ����ܹ�ǰ��������������
//���ֿ��Ըı��������Ұ
//��갴������ ��������� ��갴������ ���������
class Camera
{
public:
	Camera(glm::vec3 camera_pos, glm::vec3 direction, glm::vec3 up, float fov=45.0f, float aspect=1.775,
			float near_plane=1.0f, float far_plane=100.0f);
	~Camera();
public:
	void Move();
	void Zoom();
	void Rotate();
	void SetAspect(float aspect);
	float GetAspect();
public:
	glm::mat4 GetView() { return this->view; }
	glm::mat4 GetProjection() { return this->projection; }
private:
	glm::vec3 camera_pos;
	//�۲����
	glm::mat4 view;
	//ͶӰ����
	glm::mat4 projection;
	//fov
	float fov;
	//aspect
	float aspect;
	//near
	float near_plane;
	//far
	float far_plane;
};