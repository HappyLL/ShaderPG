//��ϰһ�¼򵥵�����
#include <iostream>
#include <string>
//glad �����ھ����Կ�������ͬ���º�����ַ��ͬ
#include <glad/glad.h>
//glfw ��opengl�ļ������п��е�һ��
#include <glfw/glfw3.h>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void load_texture(const char *pth, std::string& image_buff,int &width, int &height, int& channel) {
	char * buff = (char *)stbi_load(pth, &width, &height, &channel, 0);
	if (buff == NULL) {
		return;
	}
	image_buff = buff;
}

int main() {
	//step.1 ����һ���򵥵���Ⱦѭ��
	//step.2 ��һ���򵥵�������
	//step.3 �������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(w_width, w_height, "transform", NULL, NULL);
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
	float vertex[] = {
		0.0, 0.5, 0, 0 , 0,
		-0.5, -0.5, 0, 0.5, 1,
		0.5, -0.5, 0, 1, 0,
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//��������󶨵�GL_ARRAY_BUFFER ��������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�������ֶα�ʾ���ݿ�ʼ��ָ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	//glvertexa...link:https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//��������
	unsigned int textures;
	glGenTextures(1, &textures);
	std::string image_buff_data = "";
	int image_height, image_width, image_channel;
	load_texture("111.png", image_buff_data, image_width, image_height, image_channel);
	if (image_buff_data.length() > 0) {
		glBufferData(GL_TEXTURE, image_buff_data.length(), image_buff_data.c_str(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	
	Shader shader = Shader("transform.vs", "transform.fs");
	
	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//��Ⱦָ��
		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		shader.use_program();
		// ������ɫ��ȥ�� Ҳ���ܻ��������Σ���ɫ��Ĭ�ϣ�
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//˫����
		glfwSwapBuffers(window);
		//io
		glfwPollEvents();
	}
	return 0;
}