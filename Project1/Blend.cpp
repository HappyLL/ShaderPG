#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
// opengl混合相关
// .1 创建窗口(初始化glfw库, 定义版本(主,次和接口模式), 创建窗口, 初始化一些api的地址, 建立渲染循环)
// .2 开始绘制渲染指令
const int window_height = 600;
const int window_width = 800;

void frame_buff_size_cb(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_cb(GLFWwindow *window, int keycode, int a, int b, int c) {
	if (keycode == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Blend", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 有些函数是在运行时确定地址根据上下文的不同而不同 所以需要获取窗口地址
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, window_width, window_height);
	glfwSetFramebufferSizeCallback(window, frame_buff_size_cb);
	glfwSetKeyCallback(window, &key_cb);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.5, 0.5, 0.5, 1.0);
		// 其他渲染操作
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}