#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"

#define width 800
#define height 600

glm::vec3 camera_init_pos = glm::vec3(0, 0, 3.0f);
glm::vec3 camera_dir = glm::vec3(-1.0, 0, 0);

Camera camera();

void window_frame_size_cb(GLFWwindow *window, int window_width, int window_height) {
	glfwSetWindowSize(window, window_width, window_height);
	glViewport(0, 0, window_width, window_height);
}

void input(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
}

int main() {
	// 确定版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建上下文
	GLFWwindow * window = glfwCreateWindow(width, height, "basic_light", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	// 确定指针位置
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, window_frame_size_cb);
	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return 0;
}