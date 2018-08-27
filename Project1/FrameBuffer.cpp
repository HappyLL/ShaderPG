/*
帧缓冲
*/
#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIDTH 960
#define HEIGHT 640
glm::vec3 camera_pos(0, 0, 3.0f);
glm::vec3 camera_up(0.0, 1.0, 0.0f);
glm::vec3 camera_dir(0.0, 0.0, -1.0f);

Camera camera = Camera(camera_pos, camera_dir, camera_up, WIDTH * 1.0 / HEIGHT);

void frame_size_cb(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_code_cb(GLFWwindow *window, int keycode, int a, int b, int c) {
	if (keycode == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

void window_mouse_cb(GLFWwindow *window, double pos_x, double pos_y) {
	camera.Rotate((float)pos_x, (float)pos_y);
}

void window_sc_scroll_cb(GLFWwindow *window, double sc_x, double sc_y) {
	//printf("%.4lf %.4lf\n", sc_x, sc_y);
	camera.Zoom((float)sc_y);
}

void input(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.Move(0, -0.01);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.Move(0, 0.01);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.Move(-0.01, 0);
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.Move(0.01, 0);
		return;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		camera.BeginRotate();
		return;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		camera.EndRotate();
		return;
	}
}

int load_texture(const char *filepath) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 关于两种过滤方式 忘了
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int width, height, channel;
	unsigned char *image_data = stbi_load(filepath, &width, &height, &channel, 0);
	if (!image_data)
		return texture;
	int comp;
	if (channel == 3)
		comp = GL_RGB;
	else if (channel == 4)
		comp = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, comp, width, height, 0, comp, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);
	return texture;
}

int main() {
	// 可以看出关于窗口的操作都是涉及glfw的比如定义窗口的一些事件 所用库 版本等
	// 涉及opengl渲染都是gl开头
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "FRAME_BUFFER", NULL, NULL);
	if (!window)
		glfwTerminate();
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		glfwTerminate();
	glfwSetFramebufferSizeCallback(window, &frame_size_cb);
	glfwSetKeyCallback(window, &key_code_cb);
	glfwSetCursorPosCallback(window, window_mouse_cb);
	glfwSetScrollCallback(window, window_sc_scroll_cb);
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// 帧缓冲全屏坐标
	float cubeFbVertices[] = {
		-1, -1, 0, 0, 0,
		-1, 1, 0, 0, 1,
		1, -1, 0, 1, 0,
		1, 1, 0, 1, 1,
		1, -1, 0, 1, 0,
		-1, 1, 0, 0, 1,
	};
	int cube_texture = load_texture("marble.jpg");
	GLuint CUBE_VBO;
	glGenBuffers(1, &CUBE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), (void *)cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint CUBE_VAO;
	glGenVertexArrays(1, &CUBE_VAO);
	glBindVertexArray(CUBE_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//为当前缓冲增加一个颜色附件作为颜色缓冲区保存值的方式
	GLuint texture_attach;
	glGenTextures(1, &texture_attach);
	glBindTexture(GL_TEXTURE_2D, texture_attach);
	//在内存中申请w，h的内存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// 这两个必须要设置不然显示不出纹理附件的信息
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//为当前缓冲增加渲染缓冲用来充当模板和颜色的缓冲区
	GLuint render_buffer;
	glGenRenderbuffers(1, &render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//添加一个纹理附件
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_attach, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer);
	
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer complete" << std::endl;
	else
		std::cout << "framebuffer not complete " << status << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLuint FB_CUBE_VBO;
	glGenBuffers(1, &FB_CUBE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, FB_CUBE_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeFbVertices), cubeFbVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint FB_CUBE_VAO;
	glGenVertexArrays(1, &FB_CUBE_VAO);
	glBindVertexArray(FB_CUBE_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, FB_CUBE_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader cube_shader("depth_test_cube.vs", "depth_test_cube.fs");
	Shader cube_fb_shader("fb_cube.vs", "fb_cube.fs");
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1.0, 0));
	//model = glm::translate(model, glm::vec3(0, 0f, 0));
	while (!glfwWindowShouldClose(window)) {
		input(window);
		// FBO绑定需要放在最前面 表示之后的渲染都是基于这个缓冲区的
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.2, 0.2, 0.5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube_shader.use_program();
		cube_shader.set_uniform_matrix_4fv("mmodel", model);
		cube_shader.set_uniform_matrix_4fv("mview", camera.GetView());
		cube_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
		cube_shader.set_uniform1i("t0", 0);
		// 绑定FBO时应该不需要在设置其的GL_TEXTURE_2D 和 GL_RENDERBUFFER
		glBindVertexArray(CUBE_VAO);
		glBindTexture(GL_TEXTURE_2D, cube_texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// 绘制一个全景的正方体
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClearColor(1.0, 0, 0, 0.5);
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//为什么禁用深度缓冲会出现结果(虽然该结果不带有具体的纹理信息)
		//我可以拿到其的颜色附件
		//glBindVertexArray(CUBE_VAO);
		//glBindTexture(GL_TEXTURE_2D, texture_attach);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		// 绘制一个占满整个窗口的纹理附件(只要其的坐标不受相机控制就行)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		cube_fb_shader.use_program();
		cube_fb_shader.set_uniform1i("t0", 0);
		glBindVertexArray(FB_CUBE_VAO);
		glBindTexture(GL_TEXTURE_2D, texture_attach);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// 交换
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &FBO);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return 0;
}
