#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
//相机的主要功能
//wasd时相机能够前进后退向左向右
//滚轮可以改变相机的视野
//鼠标按下向左 相机会向左 鼠标按下向右 相机会向右
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
	//观察矩阵
	glm::mat4 view;
	//投影矩阵
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