#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

const char * vertexShaderCode = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec4 fgg_Color;\n"
"uniform float ani_pos_x;\n"
"void main(){\n"
"    gl_Position = vec4(pos.x, pos.y, pos.z, 1);\n"
"	 fgg_Color = vec4(pos.x, pos.y, pos.z, 1);\n"
"}\0";

const char * fragShaderCode = "#version 330 core\n"
"out vec4 fg_Color;\n"
"in vec4 fgg_Color;\n"
//"uniform vec4 un_fgg_Color;\n"
"void main(){\n"
"    fg_Color = fgg_Color;\n"
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

	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 800);
	//ע�ᴰ�ڸı��¼�
	glfwSetFramebufferSizeCallback(window, window_size_change);
	//������������������(ֱ�Ӷ����ڱ�׼������ϵ��)
	/*float vertx[] = {
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0,
		-0.5, 0.5, 0,
		-1, 0.5, 0,
		-1, 1, 0,
		0, 0.5, 0
	};*/
	// ���������ζ������ɫֵ
	float vertx_color[] = {
		//����		    //��ɫ
		-0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
		 0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.5,  0.0, 0.0, 0.0, 1.0,
	};
	Shader gl_shader = Shader("sample_vertex.vs", "sample_frag.fs");
	//���붥������(���ڼ�¼�������Ե�״̬����)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//���������������
	/*unsigned int ebo_vertex[] = {
		0,2,4,
		//0,3,2,
	};
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_vertex), ebo_vertex, GL_STATIC_DRAW);*/
	//���붥�㻺�����
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��Ŀ�����ݸ�ֵ��Ŀ�궥����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertx_color), vertx_color, GL_STATIC_DRAW);
	/*//����������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��������ɫ������������
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	int success = 0;
	char info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info << std::endl;
	}*/
	// ���ö�������ָ��
	// ��һ�������붥����ɫ���еĶ�������λ��ֵ��ƥ��
	// �ڶ������Ա�ʾ�������ԵĴ�С(vec3 ��ʾ��������)
	// ��4����ʾ��һ���������ݵĲ���
	// ��5����ʾ��ʼλ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// ����location 0�Ķ�������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/*unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderCode, NULL);
	glCompileShader(fragmentShader);
	unsigned int glProgram = glCreateProgram();
	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);
	glLinkProgram(glProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);*/

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//������Ⱦѭ��
	double xx = 0.0;
	while (!glfwWindowShouldClose(window)) {
		//��������
		processInput(window);
		//��Ⱦָ��
		//��һ������ʾ�����ɫ��Ϊxxx��ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//�����������(������ɫ����ڸ�ֵ�����õ���ɫ)
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		//int uni_fag = glGetUniformLocation(glProgram, "un_fgg_Color");
		gl_shader.use_program();
		gl_shader.set_uniform1f("uni_ani_pos", xx);
		double tm = glfwGetTime();
		double value = fabs((sin(tm) / 2)) * 0.01;
		if (xx + 0.5 >= 1);
		else {
			xx += value;
		}
		// ����ͼԪ����Ҫ��ʲô(ͼԪ����, ��ʼλ��, �����ٸ�����)
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// ��������6������ ������0��ʼ
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		// ͨ��drawarrays ��������Ӧ������
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		//��������
		glfwSwapBuffers(window);
		//�ж��¼��Ƿ񴥷�����֪ͨ
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}