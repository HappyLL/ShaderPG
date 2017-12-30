//��ϰһ�¼򵥵�����
#include <iostream>
#include <string>
//glad �����ھ����Կ�������ͬ���º�����ַ��ͬ
#include <glad/glad.h>
//glfw ��opengl�ļ������п��е�һ��
#include <glfw/glfw3.h>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int w_width = 800;
const int w_height = 600;

double mouse_x = w_width / 2;
double mouse_y = w_height / 2;

//ָ�����������ָ��
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);

// ������
double pitch = 0.0;
//ƫ����
double yaw = 0.0;

double senstive = 0.5;

bool firstMouse = true;

bool last_update_tag = false;

void windows_size_chg(GLFWwindow *window, int width, int height) {
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);

void input_process(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}
	/*
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
	printf("asdasdad");
	camera_pos.y -= 0.01;
	return;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
	camera_pos.y += 0.01;
	return;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
	camera_pos.x -= 0.01;
	return;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	camera_pos.x += 0.01;
	return;
	}*/
}

void cal_glm_camera(glm::vec3 up, glm::vec3 camera_pos, glm::vec3 target_pos, glm::mat4 &view) {
	glm::mat4 t;
	t = glm::translate(t, -camera_pos);
	glm::vec3 z = camera_pos - target_pos;
	glm::vec3 nor_z = glm::normalize(z);
	glm::vec3 nor_x = glm::cross(up, nor_z);
	nor_x = glm::normalize(nor_x);
	glm::vec3  nor_y = glm::cross(nor_z, nor_x);
	nor_y = glm::normalize(nor_y);
	glm::mat4 r(nor_x.x, nor_x.y, nor_x.z, 0, nor_y.x, nor_y.y, nor_y.z, 0, nor_z.x, nor_z.y, nor_z.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	view = r * t;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	double delta_x = xpos - w_width / 2;
	// ��Ļ���µĶ���ʱ(width, height)
	double delta_y = ypos - w_height / 2;
	mouse_x = xpos;
	mouse_y = ypos;

	pitch = delta_y * 90.0 / (w_height / 2);
	yaw = delta_x * 90 / (w_width / 2);

	glm::vec3 diretion;
	diretion.x = cos(glm::radians(pitch)) * sin(glm::radians(180 + yaw));
	diretion.y = sin(glm::radians(pitch));
	diretion.z = cos(glm::radians(pitch)) * cos(glm::radians(180 + yaw));
	diretion = glm::normalize(diretion);
	//printf("%.3lf %.3lf %.3lf %.3lf\n", pitch, yaw, cos(glm::radians(pitch)) * cos(glm::radians(yaw)), cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
	//printf("%.3lf %.3lf\n", delta_x, delta_y);
	cameraFront = diretion;
}

int main() {
	//step.1 ����һ���򵥵���Ⱦѭ��
	//step.2 ��һ���򵥵�������
	//step.3 �������
	//step.4 ����ռ�
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(w_width, w_height, "transform", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return  -1;
	}
	// ����������
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, windows_size_chg);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//�ӿ�����
	//glViewport(0, 0, w_width, w_height);
	float new_vertex[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//��������󶨵�GL_ARRAY_BUFFER ��������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�������ֶα�ʾ���ݿ�ʼ��ָ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(new_vertex), new_vertex, GL_STATIC_DRAW);
	//glvertexa...link:https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int LIGHT_VAO;
	glGenVertexArrays(1, &LIGHT_VAO);
	glBindVertexArray(LIGHT_VAO);
	unsigned int LIGHT_VBO;
	glGenBuffers(1, &LIGHT_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, LIGHT_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(new_vertex), new_vertex, GL_STATIC_DRAW);
	//glvertexa...link:https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	Shader shader = Shader("color_light.vs", "color_light.fs");
	shader.use_program();
	shader.set_uniformvec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	shader.set_uniformvec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-1.0f, 0, -3.0f));
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.5f, 0.0f));
	glm::mat4 view;
	cal_glm_camera(glm::vec3(0.0, 1.0, 0.0), camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), view);
	glm::mat4 projection;
	float ratio = float(w_width * 1.0 / w_height);
	projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
	shader.set_uniform_matrix_4fv("model", model);
	shader.set_uniform_matrix_4fv("view", view);
	shader.set_uniform_matrix_4fv("projection", projection);

	Shader light_shader = Shader("color_light_l.vs", "color_light_l.fs");
	light_shader.use_program();
	glm::mat4 model1;
	model1 = glm::translate(model1, glm::vec3(1.0f, 3.5f, -8.0f));
	model1 = glm::rotate(model1, glm::radians(55.0f), glm::vec3(1.0f, 0.5f, 0.0f));
	light_shader.set_uniform_matrix_4fv("model", model1);
	light_shader.set_uniform_matrix_4fv("view", view);
	light_shader.set_uniform_matrix_4fv("projection", projection);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//��Ⱦָ��
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use_program();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		light_shader.use_program();
		glBindVertexArray(LIGHT_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//˫����
		glfwSwapBuffers(window);
		//io
		glfwPollEvents();
	}
	return 0;
}