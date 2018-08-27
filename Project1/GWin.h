#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class GWin {
public:
	GWin(unsigned int width, unsigned int height);
	~GWin();
public:
	bool gWinShouldClose();
	void gWinPollEvents();
private:
	void _createWindow();
private:
	unsigned int _width;
	unsigned int _height;
	GLFWwindow * _glfwWindow;
};