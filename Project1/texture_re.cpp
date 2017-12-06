#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
// 1.建立简单的渲染循环(创建窗口, 输入， 渲染指令 ，事件, 交换缓冲)
// 2.写着色器程序(顶点着色器，片段着色器)

void window_size_cb(GLFWwindow *window, int width, int height) {
	glViewport(0,0 , width, height);
}

void input_process(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GL_TRUE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

const char *vs_code = "#version 330 core\n"
"layout (location = 0) in vec3 vpos;\n"
"layout (location = 1) in vec3 color;\n"
"layout (location = 2) in vec2 txpos;\n"
"out vec4 fg_color;\n"
"out vec2 texture_pos;\n"
"void main(){\n"
"	gl_Position = vec4(vpos, 1);\n"
"	fg_color = vec4(color, 1);\n"
"	texture_pos = txpos;\n"
"}\0";

const char *fs_code = "#version 330 core\n"
"in vec4 fg_color;\n"
"in vec2 texture_pos;\n"
"uniform sampler2D texture0;\n"
"uniform sampler2D texture1;\n"
"out vec4 fg_Color;\n"
"void main(){\n"
"	fg_Color = mix(texture(texture0, texture_pos), texture(texture1, texture_pos), 0.2);\n"
"}\0";


int main() {
	//初始化库
	glfwInit();
	//设定使用的版本(主次版本)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "learn_texuture", NULL, NULL);
	glfwMakeContextCurrent(window);

	//设置glad-api的地址(必须要先设置上下win)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "load address error" << std::endl;
		return 0;
	}

	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, window_size_cb);

	//着色器(对uniform进行激活时一定要先运行着色器)
	unsigned int vs, fs, gl;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_code, 0);
	glCompileShader(vs);
	int success;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info[512];
		glGetShaderInfoLog(vs, 512, 0, info);
		std::cout << info << std::endl;
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_code, 0);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info[512];
		glGetShaderInfoLog(fs, 512, 0, info);
		std::cout << info << std::endl;
	}

	gl = glCreateProgram();
	glAttachShader(gl, vs);
	glAttachShader(gl, fs);
	glLinkProgram(gl);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(gl);
	glUniform1i(glGetUniformLocation(gl, "texture0"), 0);
	glUniform1i(glGetUniformLocation(gl, "texture1"), 1);

	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//渲染指令
		glClearColor(0.1, 0.5, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}