#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include <functional>
class GWin {
public:
	GWin(unsigned int width, unsigned int height);
	~GWin();
public:
	void gInputProcess();
	bool gWinShouldClose();
	void gWinPollEvents();
	void gWinSwapBuffers();
	void register_key_down(std::function<void(int)> key_down_func);
	void register_key_up(std::function<void(int)> key_up_func);
public:
	glm::mat4& getMat4() { return _mat4; }
public:
	void update(double delta);
private:
	void _createWindow();
	void _inputProcess(int keycode, int status, std::function<void(int)> func);
private:
	unsigned int _width;
	unsigned int _height;
	GLFWwindow * _glfwWindow;
	glm::mat4 _mat4;
	Camera *_camera;
	std::function<void(int)> _key_down_func;
	std::function<void(int)> _key_up_func;
};