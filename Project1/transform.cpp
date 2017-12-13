//复习一下简单的纹理
#include <iostream>
//glad 是用于纠正显卡驱动不同导致函数地址不同
#include <glad/glad.h>
//glfw 是opengl的几个流行库中的一个
#include <glfw/glfw3.h>
#include "Shader.h"

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
	//step.1 建立一个简单的渲染循环
	//step.2 画一个简单的三角形
	//step.3 添加纹理
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(w_width, w_height, "transform", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return  -1;
	}
	// 设置上下文
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, windows_size_chg);
	float vertex[] = {
		0.0, 0.5, 0,
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//顶点数组绑定到GL_ARRAY_BUFFER 操作的是GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//第三个字段表示数据开始的指针
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	//glvertexa...link:https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	Shader shader = Shader("transform.vs", "transform.fs");
	
	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//渲染指令
		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		shader.use_program();
		// 上述着色器去掉 也是能画出三角形
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//双缓冲
		glfwSwapBuffers(window);
		//io
		glfwPollEvents();
	}
	return 0;
}