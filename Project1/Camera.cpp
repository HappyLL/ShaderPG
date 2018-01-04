#include "Camera.h"

Camera::Camera(glm::vec3 camera_pos, glm::vec3 direction, glm::vec3 up, float aspect, float fov, float near_plane, float far_plane)
{
	this->camera_pos = camera_pos;
	this->direction = direction;
	this->up = up;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	this->project_dirty = true;
	this->view_dirty = true;

	//俯仰角偏航角具体还是要根据direction来计算(先不做)
	this->yaw = 0.0;
	this->pitch = 180.0;
	this->binit_pos = true;
}

Camera::~Camera()
{
}

void Camera::Move(float camera_x, float camera_z)
{
	if (this->camera_pos.x == camera_x && this->camera_pos.z == camera_z)
		return;
	this->camera_pos.x = camera_x;
	this->camera_pos.z = camera_z;
	this->view_dirty = true;
}

void Camera::Zoom(float delta)
{
	if (delta == 0)
		return;
	if (delta >= 0 && this->fov + delta >= 60.0f)
		return;
	if (delta <=0 && this->fov - delta <= 30.0f)
		return;
	this->fov += delta;
	this->project_dirty = true;
}

void Camera::Rotate(float sc_xpos, float sc_ypos)
{
	if (!this->brotate)
		return;
	float delta_x = (sc_xpos - this->last_sc_posx) * this->senstive;
	float delta_y = (this->last_sc_posy - sc_ypos) * this->senstive;

	this->yaw += delta_y;
	this->pitch += delta_x;
	this->direction.z = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
	this->direction.y = glm::sin(glm::radians(this->yaw));
	this->direction.x = glm::cos(glm::radians(this->yaw)) * glm::sin(glm::radians(this->pitch));
	this->view_dirty = true;
	this->last_sc_posx = sc_xpos;
	this->last_sc_posy = sc_ypos;
}

void Camera::BeginRotate(float init_r_x, float init_r_y)
{
	if (this->brotate)
		return;
	this->brotate = true;
	if (this->binit_pos) {
		this->last_sc_posx = init_r_x;
		this->last_sc_posy = init_r_y;
		this->binit_pos = false;
	}
}

void Camera::EndRotate()
{
	if (!this->brotate)
		return;
	this->brotate = false;
}

glm::mat4 Camera::GetView()
{
	if (!this->view_dirty)
		return this->view;
	this->view_dirty = false;
	this->view = glm::lookAt(this->camera_pos, this->camera_pos + this->direction, this->up);
	return this->view;
}

glm::mat4 Camera::GetProjection()
{
	//printf("fov is %lf", this->fov);
	if (!this->project_dirty)
		return this->projection;
	this->project_dirty = false;
	this->projection = glm::perspective(glm::radians(this->fov), this->aspect, this->near_plane, this->far_plane);
	return this->projection;
}
