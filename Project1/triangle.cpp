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
	// 初始化glfw库
	if (glfwInit() != GLFW_TRUE) {
		glfwTerminate();
		return;
	}
	// 创建窗口
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 800, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "window created failed";
		glfwTerminate();
	}

	//创建上下文
	glfwMakeContextCurrent(window);
	//第三方库(glad)加载opengl 需要保证正确地址
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to init glad";
		glfwTerminate();
	}
	//创建视口大小
	glViewport(0, 0, 600, 800);
	//注册窗口改变事件
	glfwSetFramebufferSizeCallback(window, window_size_change);
	//定义三角形三个顶点(直接定义在标准化坐标系中)
	float vertx[] = {
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0,

		-0.5, 0.5, 0,
		-1, 0.5, 0,
		-1, 1, 0,
	};
	//申请顶点数组(用于记录顶点属性的状态集合)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//申请索引缓冲对象
	unsigned int ebo_vertex[] = {
		0,1,2,
		0,3,2,
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_vertex), ebo_vertex, GL_STATIC_DRAW);
	//申请顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将目标数据赋值到目标顶点上
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertx), (void *)vertx, GL_STATIC_DRAW);
	//创建顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//将顶点着色器与程序相关联
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	// 设置顶点属性指针
	// 第一个参数与顶点着色器中的顶点属性位置值相匹配
	// 第二个属性表示顶点属性的大小(vec3 表示三个数据)
	// 第4个表示下一个顶点数据的步长
	// 第5个表示起始位置
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	// 开启location 0的顶点属性
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
	//增加渲染循环
	while (!glfwWindowShouldClose(window)) {
		//处理输入
		processInput(window);
		//渲染指令
		//第一条语句表示清除颜色设为xxx颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//完成上述任务(先做颜色清除在赋值所设置的颜色)
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glUseProgram(glProgram);
		int u = glGetUniformLocation(glProgram, "color_a");
		glUniform3f(u, 0.4f, 0.3f, 1.0f);
		u = glGetUniformLocation(glProgram, "color_b");
		glm::vec3 g = glm::vec3(1.0, 0.3, 0.2);
		glUniform3f(u, g.x, g.y, g.z);
		// 告诉图元我需要画什么
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// 画三角形6个顶点 从索引0开始
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// 通过drawarrays 来画出对应三角形
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		//交换缓冲
		glfwSwapBuffers(window);
		//判断事件是否触发并发通知
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(glProgram);
	glfwTerminate();
}