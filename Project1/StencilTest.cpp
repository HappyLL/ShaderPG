#include <iostream>
#include "Camera.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef unsigned int uint;
typedef unsigned char uchar;

#define WIDTH  1024
#define HEIGHT 768
const char * title = "depth test";
const char * plane_file_name = "metal.png";
const char * cube_file_name = "marble.jpg";

glm::vec3 camera_pos(0, 0, 6.0f);
glm::vec3 camera_dir(0, 0, -1.0f);
glm::vec3 camera_up(0, 1.0f, 0);

Camera camera = Camera(camera_pos, camera_dir, camera_up, WIDTH * 1.0 / HEIGHT);

void frame_buff_size_cb(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_cb(GLFWwindow *window, int keycode, int a, int b, int c) {
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
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

GLuint load_texture(const char * file_name);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
	}
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, &frame_buff_size_cb);
	glfwSetKeyCallback(window, &key_cb);
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
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	//加载方形图片
	GLuint cube_texture = load_texture(cube_file_name);
	//地板方形图片
	GLuint plane_texture = load_texture(plane_file_name);

	//绘制效果https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/01%20Depth%20testing/
	GLuint CUBE_VBO;
	glGenBuffers(1, &CUBE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), (void *)cubeVertices, GL_STATIC_DRAW);

	GLuint PLANE_VBO;
	glGenBuffers(1, &PLANE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, PLANE_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), (void *)planeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint CUBE_VAO;
	glGenVertexArrays(1, &CUBE_VAO);
	glBindVertexArray(CUBE_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));

	GLuint PLANE_VAO;
	glGenVertexArrays(1, &PLANE_VAO);
	glBindVertexArray(PLANE_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, PLANE_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader cube_shader("depth_test_cube.vs", "depth_test_cube.fs");
	Shader plane_shader("depth_test_plane.vs", "depth_test_plane.fs");
	Shader outline_shader("stencil_test_outline.vs", "stencil_test_outline.fs");
	glm::mat4 model;
	glm::mat4 cube_model = glm::rotate(model, 45.0f, glm::vec3(0, 1.0, 0));
	glm::vec3 translates[] = { glm::vec3(0.0f), glm::vec3(1.5f ,0, 2.5f), glm::vec3(0.5f, 0, -0.8f) };
	glm::mat4 cube_scale_model = glm::scale(cube_model, glm::vec3(1.01f, 1.01f, 1.0f));
	// 开启深度测试(未开启深度测试 后渲染的会将新渲染的覆盖)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	// 模板值更新函数
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	while (!glfwWindowShouldClose(window)) {
		// 渲染循环
		// 每次渲染循环开始时清除颜色缓冲区和深度缓冲区的值
		input(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.2, 0.2, 0.2, 0.5);
		
		glStencilMask(0x00);
		plane_shader.use_program();
		plane_shader.set_uniform_matrix_4fv("mmodel", model);
		plane_shader.set_uniform_matrix_4fv("mview", camera.GetView());
		plane_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
		plane_shader.set_uniform1i("t0", 0);
		glBindVertexArray(PLANE_VAO);
		if (plane_texture != -1)
			glBindTexture(GL_TEXTURE_2D, plane_texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		cube_shader.use_program();
		cube_shader.set_uniform_matrix_4fv("mview", camera.GetView());
		cube_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
		cube_shader.set_uniform1i("t0", 0);
		cube_shader.set_uniform_matrix_4fv("mmodel", cube_model);
		glBindVertexArray(CUBE_VAO);
		if (cube_texture != -1)
			glBindTexture(GL_TEXTURE_2D, cube_texture);
		for (int index = 0; index < 3; ++index) {
			glm::mat4 new_cube_model = glm::translate(cube_model, translates[index]);
			cube_shader.set_uniform_matrix_4fv("mmodel", new_cube_model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glStencilMask(0x00);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glDisable(GL_DEPTH_TEST);
		outline_shader.use_program();
		outline_shader.set_uniform_matrix_4fv("mview", camera.GetView());
		outline_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
		glBindVertexArray(CUBE_VAO);
		for (int index = 0; index < 3; ++index) {
			glm::mat4 new_cube_model = glm::translate(cube_scale_model, translates[index]);
			outline_shader.set_uniform_matrix_4fv("mmodel", new_cube_model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glEnable(GL_DEPTH_TEST);
		glStencilMask(0xFF);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return 0;
}

GLuint load_texture(const char * file_name)
{
	int wid, heigth, channel;
	uchar *image_data = stbi_load(file_name, &wid, &heigth, &channel, 0);
	if (image_data == NULL)
		return -1;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (channel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wid, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	else if (channel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wid, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	//设置缠绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//设置过滤方式(对应的纹理坐标上的点的像素)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
