//��ϰһ�¼򵥵�����
#include <iostream>
//glad �����ھ����Կ�������ͬ���º�����ַ��ͬ
#include <glad/glad.h>
//glfw ��opengl�ļ������п��е�һ��
#include <glfw/glfw3.h>

const int w_width = 800;
const int w_height = 600;

void windows_size_chg(GLFWwindow *window, int width, int height) {
	glfwSetWindowSize(window, width, height);
}

void input_process(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	//step.1 ����һ���򵥵���Ⱦѭ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(w_width, w_height, "window", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return  -1;
	}
	// ����������
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, windows_size_chg);
	//glfwSetKeyCallback(window, key_call_back);
	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//��Ⱦָ��
		//˫����
		glfwSwapBuffers(window);
		//io
		glfwPollEvents();
	}
	return 0;
}