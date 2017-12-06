#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* vertex_code = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"layout (location = 1) in vec3 color;\n"
"layout (location = 2) in vec2 texture_pos;\n"
"out vec3 ot_color;\n"
"out vec2 tx_pos;\n"
"void main(){\n"
"	gl_Position = vec4(pos, 1);\n"
"	ot_color = color;\n"
"	tx_pos = texture_pos;\n"
"}\n\0";

const char* fragment_code = "#version 330 core\n"
"in vec3 ot_color;\n"
"in vec2 tx_pos;\n"
"out vec4 fg_Color;\n"
"uniform sampler2D texture1;\n"
"uniform sampler2D texture2;\n"
"uniform float alpha;\n"
"void main(){\n"
"	vec2 tx2_pos = vec2(-tx_pos.x, -tx_pos.y);\n"
"	fg_Color = mix(texture(texture1, tx_pos), texture(texture2, tx2_pos), alpha);\n"//texture(texture1, tx_pos) * vec4(ot_color, 1);\n"
//"	fg_Color1 = texture(texture2, tx_pos);\n"
"}\n\0";

void window_resize_cb(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

void input_process(GLFWwindow * window, float &alpha) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_TRUE) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_TRUE) {
		if (alpha + 0.1 <= 1)
			alpha += 0.1;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_TRUE) {
		if (alpha - 0.1 >= 0)
			alpha -= 0.1;
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window = glfwCreateWindow(800, 800, "my_triangle", NULL, NULL);
	if (!window) {
		std::cout << "window create error" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "loader error";
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, window_resize_cb);
	glViewport(0, 0, 800, 800);
	//����������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_code, NULL);
	glCompileShader(vertexShader);
	int compile_status;
	glGetProgramiv(vertexShader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		char info[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << info << std::endl;
	}
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragment_code, NULL);
	glCompileShader(fragShader);
	glGetProgramiv(fragShader, GL_COMPILE_STATUS, &compile_status);
	if (!compile_status) {
		char info[512] = {};
		glGetProgramInfoLog(fragShader, 512, NULL, info);
		std::cout << info << std::endl;
	}

	unsigned int glShaderProgram;
	glShaderProgram = glCreateProgram();
	glAttachShader(glShaderProgram, vertexShader);
	glAttachShader(glShaderProgram, fragShader);
	glLinkProgram(glShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	//��������
	int img_width, img_height, img_channel;
	unsigned char * img_bytes = stbi_load("container.jpg", &img_width, &img_height, &img_channel, 0);
	if (img_bytes == NULL) {
		glDeleteProgram(glShaderProgram);
		glfwTerminate();
	}
	//��������
	glActiveTexture(GL_TEXTURE0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	//���ò��Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//���ù��˷�ʽ(��Ӧ�����������ϵĵ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//������������
	stbi_image_free(img_bytes);

	char *path = "111.png";
	img_bytes = stbi_load(path, &img_width, &img_height, &img_channel, 0);
	if (img_bytes == NULL) {
		glDeleteProgram(glShaderProgram);
		glfwTerminate();
	}
	//��������
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//��ɫͨ����4��Ҫ͸����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	//���ò��Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//���ù��˷�ʽ(��Ӧ�����������ϵĵ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//������������
	stbi_image_free(img_bytes);
	glUseProgram(glShaderProgram);
	glUniform1i(glGetUniformLocation(glShaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(glShaderProgram, "texture2"), 1);
	float alpha = 0.0;
	glUniform1f(glGetUniformLocation(glShaderProgram, "alpha"), alpha);
	/*
		//���ö�������
	float vertex[] = {
		//����		 //��ɫֵ   //��������
		-0.5, -0.5, 0, 1, 0, 0,  0, 0,
		 0.0, -0.5, 0, 1, 0, 0,  1, 0,
		 0.0,  0.0, 0, 0, 0, 0,  1, 1,
		 -0.5, 0.0, 0, 0, 0, 0, 0, 1,
		 0.5, 0.0, 0, 0, 0, 0, 2, 1,
		 0.0, 0.5, 0, 0, 0, 0, 1, 2, 
		-0.5,  0.5, 0, 0, 1, 0,  0, 2,
		0.5,  0.5, 0, 0, 0, 1,  2, 2,
		0.5, -0.5, 0, 0, 0, 0,  2, 0,
	};
	unsigned int ebo_vertex[] = {
		0, 1, 2,
		0, 3, 2,
		3, 6, 5,
		3, 2, 5,
		2, 4, 7,
		2, 5, 7,
		1, 8, 4,
		1, 2, 4,
	};
	*/
	//���ö�������
	float vertex[] = {
		//����		 //��ɫֵ   //��������
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1, 1, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1, 0, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0, 0, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0, 1,  // top left 
	};
	unsigned int ebo_vertex[] = {
		0, 1, 2,
		0, 3, 2,
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_vertex), ebo_vertex, GL_STATIC_DRAW);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		input_process(window, alpha);
		//��Ⱦָ��
		glClearColor(0.3, 0.3, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glUseProgram(glShaderProgram);
		glUniform1f(glGetUniformLocation(glShaderProgram, "alpha"), alpha);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(glShaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}