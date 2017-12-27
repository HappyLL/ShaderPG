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

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	//printf("%.5lf %.5lf\n", xpos, ypos);
	if (firstMouse) // ���bool������ʼʱ���趨Ϊtrue��
	{
		mouse_x = xpos;
		mouse_y = ypos;
		firstMouse = false;
	}
	double delta_x = xpos - mouse_x;
	// ��Ļ���µĶ���ʱ(width, height)
	double delta_y = mouse_y - ypos;
	mouse_x = xpos;
	mouse_y = ypos;

	pitch += delta_y * senstive;
	yaw += delta_x * 0.3;
	if (pitch >= 89.0) {
		pitch = 89.0;
	}
	else if (pitch <= -89.0) {
		pitch = -89.0;
	}

	glm::vec3 diretion;
	diretion.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	diretion.y = sin(glm::radians(pitch));
	diretion.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	diretion = glm::normalize(diretion);
	printf("%.3lf %.3lf %.3lf %.3lf\n", pitch, yaw, cos(glm::radians(pitch)) * cos(glm::radians(yaw)), cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
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
	glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//�ӿ�����
	//glViewport(0, 0, w_width, w_height);
	float vertex[] = {
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
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	//��������󶨵�GL_ARRAY_BUFFER ��������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�������ֶα�ʾ���ݿ�ʼ��ָ��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	//glvertexa...link:https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// �����㻺������״̬�󶨵�VAOʱ�ö��㻺��������Ҫ�ڰ�״̬(������б���)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//��������
	unsigned int textures;
	// ��ǰ�ȼ���Ҫ�󶨵�����Ԫ
	//glActiveTexture(,);
	glGenTextures(1, &textures);
	glBindTexture(GL_TEXTURE_2D, textures);
	// ��������Ĳ��Ʒ�ʽ(s,t)�൱��(x, y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int image_height, image_width, image_channel;
	unsigned char * buff = stbi_load("container.jpg", &image_width, &image_height, &image_channel, 0);
	// png����alphaͨ�� jpg û��
	// png��glformat��ͼƬ������GL_RGBA
	if (buff) {
		//�������:https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, buff);
		stbi_image_free(buff);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	Shader shader = Shader("transform.vs", "transform.fs");
	shader.use_program();
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view;
	//look at ���� ��Ҫλ��(�����λ��) Ŀ��(ȷ�������z�᷽�� Ϊ��������� - Ŀ�귽��) �� ������(������z������ȷ��x�᷽��)
	// ��������������ϵȥ���(�����������)
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection;
	float ratio = float(w_width * 1.0 / w_height);
	projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
	shader.set_uniform_matrix_4fv("model", model);
	shader.set_uniform_matrix_4fv("view", view);
	shader.set_uniform_matrix_4fv("projection", projection);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		input_process(window);
		//��Ⱦָ��
		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(VAO);
		shader.use_program();
		// �ڶ���������ʾĿ��λ��
		// eye direction up
		view = glm::lookAt(camera_pos, -camera_pos + cameraFront, glm::vec3(0.0, 1.0, 0.0));
		//printf("%.3lf\n", cameraFront.y);
		shader.set_uniform_matrix_4fv("view", view);
		for (int index = 0; index < 10; ++index) {
			glm::mat4 model;
			glm::mat4 trans;
			trans = glm::translate(trans, cubePositions[index%10]);
			if (index % 3 == 0) {
				model = glm::rotate(trans, float(glm::radians(-55.0) * glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			else {
				model = trans;
			}
			shader.set_uniform_matrix_4fv("model", model);
			float radius = 10.0f;
			glm::mat4 view;
			// ע�ⶥ����ɫ��ÿ�ζ�����һ�������Ķ�����Ϊ����
			// �ö�����������ݽ�����������(������ Ϊ�����xyz�Ͷ�Ӧ����������)
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//˫����
		glfwSwapBuffers(window);
		//io
		glfwPollEvents();
	}
	return 0;
}