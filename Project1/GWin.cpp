#include "GWin.h"
#include <iostream>

glm::vec3 camera_pos(0, 0, 3);
glm::vec3 camera_dir(0, 0, -1);
glm::vec3 camera_up(0, 1, 0);

void _frame_size_change_cb(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GWin::GWin(unsigned int width, unsigned int height)
{
	_width = width;
	_height = height;
	_glfwWindow = nullptr;
	_key_down_func = nullptr;
	_key_up_func = nullptr;
	_camera = new Camera(camera_pos, camera_dir, camera_up);
	_createWindow();
}

GWin::~GWin()
{
	_glfwWindow = nullptr;
	delete _camera;
}

void GWin::gInputProcess()
{
	if (glfwGetKey(_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(_glfwWindow, true);
		return;
	}
	_inputProcess(GLFW_KEY_W, GLFW_PRESS, _key_down_func);
	_inputProcess(GLFW_KEY_W, GLFW_RELEASE, _key_up_func);

	_inputProcess(GLFW_KEY_S, GLFW_PRESS, _key_down_func);
	_inputProcess(GLFW_KEY_S, GLFW_RELEASE, _key_up_func);

	_inputProcess(GLFW_KEY_A, GLFW_PRESS, _key_down_func);
	_inputProcess(GLFW_KEY_A, GLFW_RELEASE, _key_up_func);

	_inputProcess(GLFW_KEY_D, GLFW_PRESS, _key_down_func);
	_inputProcess(GLFW_KEY_D, GLFW_RELEASE, _key_up_func);

	_inputProcess(GLFW_KEY_SPACE, GLFW_PRESS, _key_down_func);
	_inputProcess(GLFW_KEY_SPACE, GLFW_RELEASE, _key_up_func);
}

void GWin::_inputProcess(int keycode, int status, std::function<void(int)> func)
{
	if (!func)
		return;
	if (glfwGetKey(_glfwWindow, keycode) == status)
		func(keycode);
}

bool GWin::gWinShouldClose()
{
	if (!_glfwWindow)
		return true;
	return glfwWindowShouldClose(_glfwWindow);
}

void GWin::gWinPollEvents()
{
	glfwPollEvents();
}

void GWin::gWinSwapBuffers()
{
	glfwSwapBuffers(_glfwWindow);
}

void GWin::register_key_down(std::function<void(int)> key_down_func)
{
	_key_down_func = key_down_func;
}

void GWin::register_key_up(std::function<void(int)> key_up_func)
{
	_key_up_func = key_up_func;
}

void GWin::update(double delta)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.3, 0.2, 0.4, 1.0);
}

void GWin::_createWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	_glfwWindow = glfwCreateWindow(_width, _height, "GEngine", nullptr, nullptr);
	if (!_glfwWindow)
	{
		std::cout << "[Gwin][_CreateWindow] Error" << std::endl;
		return;
	}
	glfwMakeContextCurrent(_glfwWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "[Gwin][_CreateWindow] glfwGetProcAddress Error" << std::endl;
		return;
	}
	glViewport(0, 0, _width, _height);
	glfwSetFramebufferSizeCallback(_glfwWindow, _frame_size_change_cb);
}
