#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

const char * vertexShaderCode = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"out vec3 color;\n"
"void main(){\n"
"    gl_Position = vec4(pos.x, pos.y, pos.z, 1);\n"
"    color = pos * pos;\n"
"}\0";

const char * fragShaderCode = "#version 330 core\n"
"in vec3 color;\n"
"uniform vec3 color_a;\n"
"uniform vec3 color_b;\n"
"out vec4 fg_Color;\n"
"void main(){\n"
"    fg_Color = vec4(color_a * color_b, 1);\n"
"}\0";

void window_size_change(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void main() {
	// ��ʼ��glfw��
	if (glfwInit() != GLFW_TRUE) {
		glfwTerminate();
		return;
	}
	// ��������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 800, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "window created failed";
		glfwTerminate();
	}

	//����������
	glfwMakeContextCurrent(window);
	//��������(glad)����opengl ��Ҫ��֤��ȷ��ַ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to init glad";
		glfwTerminate();
	}
	//�����ӿڴ�С
	glViewport(0, 0, 600, 800);
	//ע�ᴰ�ڸı��¼�
	glfwSetFramebufferSizeCallback(window, window_size_change);
	//������������������(ֱ�Ӷ����ڱ�׼������ϵ��)
	float vertx[] = {
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0,

		-0.5, 0.5, 0,
		-1, 0.5, 0,
		-1, 1, 0,
	};
	//���붥������(���ڼ�¼�������Ե�״̬����)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//���������������
	unsigned int ebo_vertex[] = {
		0,1,2,
		0,3,2,
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_vertex), ebo_vertex, GL_STATIC_DRAW);
	//���붥�㻺�����
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��Ŀ�����ݸ�ֵ��Ŀ�궥����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertx), (void *)vertx, GL_STATIC_DRAW);
	//����������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��������ɫ������������
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	// ���ö�������ָ��
	// ��һ�������붥����ɫ���еĶ�������λ��ֵ��ƥ��
	// �ڶ������Ա�ʾ�������ԵĴ�С(vec3 ��ʾ��������)
	// ��4����ʾ��һ���������ݵĲ���
	// ��5����ʾ��ʼλ��
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	// ����location 0�Ķ�������
	glEnableVertexAttribArray(0);
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderCode, NULL);
	glCompileShader(fragmentShader);
	unsigned int glProgram = glCreateProgram();
	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);
	glLinkProgram(glProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//������Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		//��������
		processInput(window);
		//��Ⱦָ��
		//��һ������ʾ�����ɫ��Ϊxxx��ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//�����������(������ɫ����ڸ�ֵ�����õ���ɫ)
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glUseProgram(glProgram);
		int u = glGetUniformLocation(glProgram, "color_a");
		glUniform3f(u, 0.4f, 0.3f, 1.0f);
		u = glGetUniformLocation(glProgram, "color_b");
		glm::vec3 g = glm::vec3(1.0, 0.3, 0.2);
		glUniform3f(u, g.x, g.y, g.z);
		// ����ͼԪ����Ҫ��ʲô
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// ��������6������ ������0��ʼ
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// ͨ��drawarrays ��������Ӧ������
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		//��������
		glfwSwapBuffers(window);
		//�ж��¼��Ƿ񴥷�����֪ͨ
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(glProgram);
	glfwTerminate();
}