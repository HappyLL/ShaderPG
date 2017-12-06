#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\\type_ptr.hpp"
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
	// 初始化glfw库
	if (glfwInit() != GLFW_TRUE) {
		glfwTerminate();
		return;
	}
	// 创建窗口
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 800);
	//注册窗口改变事件
	glfwSetFramebufferSizeCallback(window, window_size_change);
	//定义三角形三个顶点(直接定义在标准化坐标系中)
	/*float vertx[] = {
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0,
		-0.5, 0.5, 0,
		-1, 0.5, 0,
		-1, 1, 0,
		0, 0.5, 0
	};*/
	// 定义三角形顶点和颜色值
	float vertx_color[] = {
		//顶点		    //颜色
		-0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
		 0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.5,  0.0, 0.0, 0.0, 1.0,
	};
	Shader gl_shader = Shader("sample_vertex.vs", "sample_frag.fs");
	//申请顶点数组(用于记录顶点属性的状态集合)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//申请索引缓冲对象
	/*unsigned int ebo_vertex[] = {
		0,2,4,
		//0,3,2,
	};
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_vertex), ebo_vertex, GL_STATIC_DRAW);*/
	//申请顶点缓冲对象
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将目标数据赋值到目标顶点上
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertx_color), vertx_color, GL_STATIC_DRAW);
	/*//创建顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//将顶点着色器与程序相关联
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
	// 设置顶点属性指针
	// 第一个参数与顶点着色器中的顶点属性位置值相匹配
	// 第二个属性表示顶点属性的大小(vec3 表示三个数据)
	// 第4个表示下一个顶点数据的步长
	// 第5个表示起始位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// 开启location 0的顶点属性
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
	glm::mat4 mat;
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//增加渲染循环
	double xx = 0.0;
	while (!glfwWindowShouldClose(window)) {
		//处理输入
		processInput(window);
		//渲染指令
		//第一条语句表示清除颜色设为xxx颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//完成上述任务(先做颜色清除在赋值所设置的颜色)
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
		// 告诉图元我需要画什么(图元类型, 起始位置, 画多少个顶点)
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// 画三角形6个顶点 从索引0开始
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		// 通过drawarrays 来画出对应三角形
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//
		//交换缓冲
		glfwSwapBuffers(window);
		//判断事件是否触发并发通知
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}