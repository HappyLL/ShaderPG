#include "Camera.h"

Camera::Camera(glm::vec3 camera_pos, glm::vec3 direction, glm::vec3 up, float fov, float aspect, float near_plane, float far_plane)
{
	this->camera_pos = camera_pos;
	// 初始化观察矩阵
	this->view = glm::lookAt(camera_pos, direction, up);
	// 初始化投影矩阵
	this->projection = glm::perspective(fov, aspect, near_plane, far_plane);
}

Camera::~Camera()
{
}

void Camera::Move()
{
}

void Camera::Zoom()
{
}

void Camera::Rotate()
{
}

void Camera::SetAspect(float aspect)
{
	this->aspect = aspect;
}

float Camera::GetAspect()
{
	return this->aspect;
}
