#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//��ϰ2(��������VAO VBO����������������)
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

const char * fragShaderCode2 = "#version 330 core\n"
"out vec4 fg_Color;\n"
"void main(){\n"
"    fg_Color = vec4(1, 1, 0, 1);\n"
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
	//����������ɫ��
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
	glDeleteShader(fragmentShader);

	unsigned int fragementShader2;
	fragementShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragementShader2, 1, &fragShaderCode2, NULL);
	glCompileShader(fragementShader2);

	unsigned int glShaderProgram2;
	glShaderProgram2 = glCreateProgram();
	glAttachShader(glShaderProgram2, vertexShader);
	glAttachShader(glShaderProgram2, fragementShader2);
	glLinkProgram(glShaderProgram2);
	glDeleteShader(fragementShader2);
	glDeleteShader(vertexShader);

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
	//�Ȱ󶨵�һ�����������Ѿ����Ӧ��VBO��Ϣ
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

	//������Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		//��������
		processInput(window);
		//��Ⱦָ��
		//��һ������ʾ�����ɫ��Ϊxxx��ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//�����������(������ɫ����ڸ�ֵ�����õ���ɫ)
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(glProgram);
		// ����ͼԪ����Ҫ��ʲô
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// ��������6������ ������0��ʼ
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// ͨ��drawarrays ��������Ӧ������
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(glShaderProgram2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//��������
		glfwSwapBuffers(window);
		//�ж��¼��Ƿ񴥷�����֪ͨ
		glfwPollEvents();

	}
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(glProgram);
	glDeleteProgram(glShaderProgram2);
	glfwTerminate();
}