#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"

#define width 800
#define height 600

glm::vec3 camera_init_pos = glm::vec3(0, 0, 3.0f);
glm::vec3 camera_dir = glm::vec3(0.0, 0, -1.0);
glm::vec3 camera_up = glm::vec3(0.0, 1.0, 0);

Camera camera(camera_init_pos, camera_dir, camera_up, width * 1.0 / height);

void window_frame_size_cb(GLFWwindow *window, int window_width, int window_height) {
	glfwSetWindowSize(window, window_width, window_height);
	glViewport(0, 0, window_width, window_height);
}

void window_mouse_cb(GLFWwindow *window, double pos_x, double pos_y) {
	camera.Rotate(pos_x, pos_y);
}

void window_sc_scroll_cb(GLFWwindow *window, double sc_x, double sc_y) {
	//printf("%.4lf %.4lf\n", sc_x, sc_y);
	camera.Zoom(sc_y);
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

int main() {
	// ȷ���汾
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//����������
	GLFWwindow * window = glfwCreateWindow(width, height, "basic_light", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	// ȷ��ָ��λ��
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, window_frame_size_cb);
	glfwSetCursorPosCallback(window, window_mouse_cb);
	glfwSetScrollCallback(window, window_sc_scroll_cb);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	unsigned int CUBE_VAO;
	glGenVertexArrays(1, &CUBE_VAO);
	glBindVertexArray(CUBE_VAO);
	unsigned int VETX_BUFF;
	glGenBuffers(1, &VETX_BUFF);
	glBindBuffer(GL_ARRAY_BUFFER, VETX_BUFF);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//����һ����Դ��
	unsigned int LIGHT_VAO;
	glGenVertexArrays(1, &LIGHT_VAO);
	glBindVertexArray(LIGHT_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VETX_BUFF);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glm::vec3 light_color = glm::vec3(0.5f, 1.0f, 1.0f);
	glm::vec3 cube_color = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec4 light_pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 model;
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.5, 0));

	glm::mat4 light_model;
	light_model = glm::translate(light_model, glm::vec3(0.0, 3.5, -1.0));
	light_model = glm::rotate(light_model, glm::radians(55.0f), glm::vec3(1.0, 0.5, 0));

	Shader cube_shader = Shader("camera_light_test_1.vs", "camera_light_test_1.fs");
	Shader light_shader = Shader("camera_light_test_light.vs", "camera_light_test_light.fs");
	cube_shader.use_program();
	cube_shader.set_uniformvec3("cube_color", cube_color);
	cube_shader.set_uniformvec3("light_color", light_color);
	glm::vec4 world_light_pos = camera.GetView() * light_model * light_pos;
	cube_shader.set_uniformvec3("light_pos", glm::vec3(world_light_pos));
	light_shader.use_program();
	light_shader.set_uniformvec3("light_color", light_color);
	//cube_shader.use_program();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float light_angle = float(sin(glfwGetTime())) * 180;

		light_model = glm::mat4();
		light_model = glm::rotate(light_model, glm::radians(light_angle), glm::vec3(1.0f, 1.0f, 0.0f));
		light_model = glm::translate(light_model, glm::vec3(0.0, 3.5, -1.0));

		glBindVertexArray(CUBE_VAO);
		cube_shader.use_program();
		cube_shader.set_uniform_matrix_4fv("model", model);
		cube_shader.set_uniform_matrix_4fv("view", camera.GetView());
		cube_shader.set_uniform_matrix_4fv("projection", camera.GetProjection());
		glm::vec4 world_light_pos = camera.GetView() * light_model * light_pos;
		cube_shader.set_uniformvec3("light_pos", glm::vec3(world_light_pos));
		//printf("%lf %lf %lf\n", camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);
		cube_shader.set_uniformvec3("camera_pos", camera.GetCameraPos());
		//��ʾ��ȥִ�ж�����ɫ��36��
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(LIGHT_VAO);
		//�󶨲�ͬ�Ķ������� ��ɫ����Ҫ����
		light_shader.use_program();
		light_shader.set_uniform_matrix_4fv("model", light_model);
		light_shader.set_uniform_matrix_4fv("view", camera.GetView());
		light_shader.set_uniform_matrix_4fv("projection", camera.GetProjection());
		//��ʾ��ȥִ�ж�����ɫ��36��
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glBindVertexArray(0);
	return 0;
}