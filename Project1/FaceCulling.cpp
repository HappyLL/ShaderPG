/*
面剔除
*/
#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define window_width 1024
#define window_height 768

glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
glm::vec3 direction(0.0f, 0.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
Camera camera = Camera(camera_pos, direction, up);

void frame_buffer_cb(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_cb(GLFWwindow *window, int keycode, int a, int b, int c) {
	if (keycode == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

void input(GLFWwindow *window) {

}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow * window = glfwCreateWindow(window_width, window_height, "FaceCulling", NULL, NULL);
	if (!window)
		glfwTerminate();
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
	}
	glViewport(0, 0, window_width, window_height);
	glfwSetFramebufferSizeCallback(window, &frame_buffer_cb);
	glfwSetKeyCallback(window, &key_cb);
	// 正方形的点 已逆时针作为正向
	float cube_vertexs[] = {

	};

	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1, 0.1, 0.1, 0.5);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return 0;
}