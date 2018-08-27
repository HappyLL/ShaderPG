#include "GWin.h"
#include <iostream>

GWin::GWin(unsigned int width, unsigned int height)
{
	_width = width;
	_height = height;
	_glfwWindow = nullptr;
	_createWindow();
}

GWin::~GWin()
{
	_glfwWindow = nullptr;
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
}
