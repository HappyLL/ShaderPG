/*
֡����
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
	// �������ֹ��˷�ʽ ����
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
	// ���Կ������ڴ��ڵĲ��������漰glfw�ı��綨�崰�ڵ�һЩ�¼� ���ÿ� �汾��
	// �漰opengl��Ⱦ����gl��ͷ
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
	
	//Ϊ��ǰ��������һ����ɫ������Ϊ��ɫ����������ֵ�ķ�ʽ
	GLuint texture_attach;
	glGenTextures(1, &texture_attach);
	glBindTexture(GL_TEXTURE_2D, texture_attach);
	//���ڴ�������w��h���ڴ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, NULL);

	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);



	Shader cube_shader("depth_test_cube.vs", "depth_test_cube.fs");
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1.0, 0));
	//model = glm::translate(model, glm::vec3(0, 0f, 0));
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClearColor(0.2, 0.2, 0.2, 0.5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube_shader.use_program();
		cube_shader.set_uniform_matrix_4fv("mmodel", model);
		cube_shader.set_uniform_matrix_4fv("mview", camera.GetView());
		cube_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
		cube_shader.set_uniform1i("t0", 0);
		glBindVertexArray(CUBE_VAO);
		glBindTexture(GL_TEXTURE_2D, cube_texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// ����
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glDeleteFramebuffers(1, &FBO);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return 0;
}
