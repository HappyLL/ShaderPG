#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertex_code = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ot_color;\n"
"void main(){\n"
"	gl_Position = vec4(pos, 1);\n"
"	ot_color = color;\n"
"}\n\0";

const char* fragment_code = "#version 330 core\n"
"in vec3 ot_color;\n"
"out vec4 fg_Color;\n"
"void main(){\n"
"	fg_Color = vec4(ot_color, 1);\n"
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
		std::cout << "window create error"<<std::endl;
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

	float vertex[] = {
		-0.5, -0.5, 0, 1.0, 0, 0,
		0.5, -0.5, 0, 0, 1.0, 0,
		0.0, 0.5, 0, 0, 0, 1.0,
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//渲染指令
		glClearColor(0.3, 0.3, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glUseProgram(glShaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(glShaderProgram);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}