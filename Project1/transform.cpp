//复习一下简单的纹理
#include <iostream>
#include <string>
//glad 是用于纠正显卡驱动不同导致函数地址不同
#include <glad/glad.h>
//glfw 是opengl的几个流行库中的一个
#include <glfw/glfw3.h>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int w_width = 800;
const int w_height = 600;

void windows_size_chg(GLFWwindow *window, int width, int height) {
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}

void input_process(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void load_texture(const char *pth, std::string& image_buff,int &width, int &height, int& channel) {
	char * buff = (char *)stbi_load(pth, &width, &height, &channel, 0);
	if (buff == NULL) {
		return;
	}
	image_buff = buff;
}

int main() {
	//step.1 建立一个简单的渲染循环
	//step.2 画一个简单的三角形
	//step.3 添加纹理
	//step.4 坐标空间
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(w_width, w_height, "transform", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return  -1;
	}
	// 设置上下文
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, windows_size_chg);
	//视口坐标
	glViewport(0, 0, w_width, w_height);
	float vertex[] = {
		0.0, 0.5, 0, 0.5 , 1,
		-0.5, -0.5, 0, 0, 0,
		0.5, -0.5, 0, 1, 0,
	};
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//顶点数组绑定到GL_ARRAY_BUFFER 操作的是GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//第三个字段表示数据开始的指针
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	//glvertexa...link:https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// 将顶点缓冲区的状态绑定到VAO时该顶点缓冲区必须要在绑定状态(否则会有崩溃)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//增加纹理
	unsigned int textures;
	// 绑定前先激活要绑定的纹理单元
	//glActiveTexture(,);
	glGenTextures(1, &textures);
	glBindTexture(GL_TEXTURE_2D, textures);
	// 设置纹理的缠绕方式(s,t)相当于(x, y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int image_height, image_width, image_channel;
	unsigned char * buff = stbi_load("container.jpg", &image_width, &image_height, &image_channel, 0);
	// png是有alpha通道 jpg 没有
	// png的glformat的图片必须是GL_RGBA
	if (buff) {
		//参数详见:https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, buff);
		stbi_image_free(buff);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	Shader shader = Shader("transform.vs", "transform.fs");
	
	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//渲染指令
		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use_program();
		glBindVertexArray(VAO);
		// 注意顶点着色器每次都会以一个单独的顶点作为输入
		// 该顶点包含的数据叫做顶点属性(如上述 为顶点的xyz和对应的纹理坐标)
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//双缓冲
		glfwSwapBuffers(window);
		//io
		glfwPollEvents();
	}
	return 0;
}