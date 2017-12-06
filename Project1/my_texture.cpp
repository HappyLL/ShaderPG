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
"uniform sampler2D ourTexture;\n"
"void main(){\n"
"	fg_Color = texture(ourTexture, tx_pos) * vec4(ot_color, 1);\n"
"}\n\0";

void window_resize_cb(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

void input_process(GLFWwindow * window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_TRUE) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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
	//创建顶点着色器
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
	
	//加载纹理
	int img_width, img_height, img_channel;
	unsigned char * img_bytes = stbi_load("container.jpg", &img_width, &img_height, &img_channel, 0);
	if (img_bytes == NULL) {
		glDeleteProgram(glShaderProgram);
		glfwTerminate();
	}
	//生成纹理
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB ,GL_UNSIGNED_BYTE, img_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	//设置缠绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置过滤方式(对应的纹理坐标上的点的像素)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//设置纹理属性
	stbi_image_free(img_bytes);
	//设置顶点属性
	float vertex[] = {
		//矩形		 //颜色值   //纹理坐标
		-0.5, -0.5, 0, 1, 0, 0,  0, 0,
		-0.5,  0.5, 0, 0, 1, 0,  0, 1,
		 0.5,  0.5, 0, 0, 0, 1,  1, 1,
		 0.5, -0.5, 0, 0, 0, 0,  1, 0,
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
		input_process(window);
		//渲染指令
		glClearColor(0.3, 0.3, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glUseProgram(glShaderProgram);
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