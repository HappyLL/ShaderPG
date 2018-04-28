#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
// opengl������
// .1 ��������(��ʼ��glfw��, ����汾(��,�κͽӿ�ģʽ), ��������, ��ʼ��һЩapi�ĵ�ַ, ������Ⱦѭ��)
// .2 ��ʼ������Ⱦָ��
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
	// ��Щ������������ʱȷ����ַ���������ĵĲ�ͬ����ͬ ������Ҫ��ȡ���ڵ�ַ
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, window_width, window_height);
	glfwSetFramebufferSizeCallback(window, frame_buff_size_cb);
	glfwSetKeyCallback(window, &key_cb);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.5, 0.5, 0.5, 1.0);
		// ������Ⱦ����
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}