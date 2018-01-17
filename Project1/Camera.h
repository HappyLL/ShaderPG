#pragma once
#include <iostream>
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
	Camera(glm::vec3 camera_pos, glm::vec3 direction, glm::vec3 up, float aspect = 1.775, float fov=45.0f,
			float near_plane=1.0f, float far_plane=100.0f);
	~Camera();
public:
	void Move(float camera_x, float camera_z);
	void Zoom(float delta);
	void Rotate(float sc_xpos, float sc_ypos);
	void BeginRotate();
	void EndRotate();
public:
	glm::vec3 GetCameraPos();
	glm::mat4 GetView();
	glm::mat4 GetProjection();
private:
	glm::vec3 camera_pos;
	glm::vec3 up;
	glm::vec3 direction;
	//fov
	float fov;
	//aspect
	float aspect;
	//near
	float near_plane;
	//far
	float far_plane;

	bool project_dirty;
	bool view_dirty;

	glm::mat4 projection;
	glm::mat4 view;

	float last_sc_posx, last_sc_posy;
	float first_rotate;
	float brotate;

	float yaw;
	float pitch;

	bool binit_pos;

	float senstive = 0.05;
};