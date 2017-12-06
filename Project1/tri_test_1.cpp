#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//练习2(创建两个VAO VBO来绘制两个三角形)
const char * vertexShaderCode = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main(){\n"
"    gl_Position = vec4(pos.x, pos.y, pos.z, 1);\n"
"}\0";

const char * fragShaderCode = "#version 330 core\n"
"out vec4 fg_Color;\n"
"void main(){\n"
"    fg_Color = vec4(1, 0, 0, 1);\n"
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
	//创建顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
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
	float vertx1[] = {
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0,
	};
	float vertx2[] = {
		-0.5, -0.5, 0,
		-0.5, 0.5, 0,
		0.5, 0.5, 0,
	};
	unsigned int VAO[2], VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	//先绑定第一个顶点数组已经其对应的VBO信息
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertx1), vertx1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertx2), vertx2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//增加渲染循环
	while (!glfwWindowShouldClose(window)) {
		//处理输入
		processInput(window);
		//渲染指令
		//第一条语句表示清除颜色设为xxx颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//完成上述任务(先做颜色清除在赋值所设置的颜色)
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(glProgram);
		// 告诉图元我需要画什么
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// 画三角形6个顶点 从索引0开始
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// 通过drawarrays 来画出对应三角形
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//交换缓冲
		glfwSwapBuffers(window);
		//判断事件是否触发并发通知
		glfwPollEvents();

	}
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(glProgram);
	glfwTerminate();
}