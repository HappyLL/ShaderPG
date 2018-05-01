#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	glfwSetFramebufferSizeCallback(window, window_frame_size_cb);
	glfwSetCursorPosCallback(window, window_mouse_cb);
	glfwSetScrollCallback(window, window_sc_scroll_cb);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	unsigned int CUBE_VAO;
	glGenVertexArrays(1, &CUBE_VAO);
	glBindVertexArray(CUBE_VAO);
	unsigned int VETX_BUFF;
	glGenBuffers(1, &VETX_BUFF);
	glBindBuffer(GL_ARRAY_BUFFER, VETX_BUFF);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//����һ����Դ��
	unsigned int LIGHT_VAO;
	glGenVertexArrays(1, &LIGHT_VAO);
	glBindVertexArray(LIGHT_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VETX_BUFF);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//������������ͼ
	int img_width, img_height, img_channel;
	unsigned char * img_bytes = stbi_load("container2.png", &img_width, &img_height, &img_channel, 0);
	//������������ͼ
	glActiveTexture(GL_TEXTURE0);
	// ��֮ǰ�����Ӧ������Ԫ�ͻ�󶨵��ĵ�Ԫ��
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	//���ò��Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//���ù��˷�ʽ(��Ӧ�����������ϵĵ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//���ؾ���߹���ͼ
	int img_width1, img_height1, img_channel1;
	unsigned char * img_bytes1 = stbi_load("lighting_maps_specular_color.png", &img_width1, &img_height1, &img_channel1, 0);
	//������������ͼ
	glActiveTexture(GL_TEXTURE1);
	// ��֮ǰ�����Ӧ������Ԫ�ͻ�󶨵��ĵ�Ԫ��
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width1, img_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_bytes1);
	glGenerateMipmap(GL_TEXTURE_2D);
	//���ò��Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//���ù��˷�ʽ(��Ӧ�����������ϵĵ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//���ط�����ͼ
	/*
	int img_width2, img_height2, img_channel2;
	unsigned char * img_bytes2 = stbi_load("matrix.jpg", &img_width2, &img_height2, &img_channel2, 0);
	//���������ͼ
	glActiveTexture(GL_TEXTURE2);
	// ��֮ǰ�����Ӧ������Ԫ�ͻ�󶨵��ĵ�Ԫ��
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width1, img_height1, 0, GL_RGB, GL_UNSIGNED_BYTE, img_bytes2);
	glGenerateMipmap(GL_TEXTURE_2D);
	//���ò��Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//���ù��˷�ʽ(��Ӧ�����������ϵĵ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/

	stbi_image_free(img_bytes);
	stbi_image_free(img_bytes1);
	//stbi_image_free(img_bytes2);

	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 light_pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 light_model;

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

	glm::vec3 light_ambient = glm::vec3(0.5);
	glm::vec3 light_diffuse = glm::vec3(2.0);
	glm::vec3 light_specular = glm::vec3(1.0);
	glm::vec3 light_dir = glm::vec3(0.0f, 0.0, -1.0f);

	Shader cube_shader = Shader("lighting_caster.vs", "lighting_caster.fs");
	Shader light_shader = Shader("camera_light_test_light.vs", "camera_light_test_light.fs");
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//float light_angle = float(sin(glfwGetTime())) * 180;

		light_model = glm::mat4();
		//light_model = glm::rotate(light_model, glm::radians(light_angle), glm::vec3(1.0f, 1.0f, 0.0f));
		light_model = glm::translate(light_model, glm::vec3(0.0, 0, 3.0));
		glm::vec4 view_light_pos = camera.GetView() * light_model * light_pos;
		glm::vec4 view_light_dir = camera.GetView() * light_model * glm::vec4(light_dir, 1.0);
		float radians = 0.0f;
		for (int i = 0; i < 10; ++i) {
			glm::mat4 model;
			model = glm::rotate(model, glm::radians(radians), glm::vec3(1.0, 0.5, 0));
			model = glm::translate(model, cubePositions[i]);
			radians += 20.0f;
			cube_shader.use_program();
			cube_shader.set_uniformvec3("light.light_dir", glm::vec3(view_light_dir));
			cube_shader.set_uniformvec3("light.light_color", light_color);
			cube_shader.set_uniformvec3("light.ambient", light_ambient);
			cube_shader.set_uniformvec3("light.diffuse", light_diffuse);
			cube_shader.set_uniformvec3("light.specular", light_specular);
			cube_shader.set_uniform1f("light.c", 1.0f);
			cube_shader.set_uniform1f("light.l", 0.09f);
			cube_shader.set_uniform1f("light.q", 0.032f);
			cube_shader.set_uniform1f("light.cutoff", glm::cos(glm::radians(55.5)));
			cube_shader.set_uniformvec3("light.light_pos", glm::vec3(view_light_pos));
			cube_shader.set_uniform_matrix_4fv("model", model);
			cube_shader.set_uniform_matrix_4fv("view", camera.GetView());
			cube_shader.set_uniform_matrix_4fv("projection", camera.GetProjection());
			cube_shader.set_uniformvec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			cube_shader.set_uniform1f("material.shininess", 32.0f);
			cube_shader.set_uniform1i("material.diffuse", 0);
			cube_shader.set_uniform1i("material.specular", 1);
			//cube_shader.set_uniform1i("material.emission", 2);
			//printf("%lf %lf %lf\n", camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z);
			//cube_shader.set_uniformvec3("camera_pos",  camera.GetCameraPos());
			//��ʾ��ȥִ�ж�����ɫ��36��
			glBindVertexArray(CUBE_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//�󶨲�ͬ�Ķ������� ��ɫ����Ҫ����
		light_shader.use_program();
		light_shader.set_uniformvec3("light.light_dir", light_dir);
		light_shader.set_uniformvec3("light.light_color", light_color);
		light_shader.set_uniformvec3("light.ambient", light_ambient);
		light_shader.set_uniformvec3("light.diffuse", light_diffuse);
		light_shader.set_uniformvec3("light.specular", light_specular);
		light_shader.set_uniform1f("light.c", 1.0f);
		light_shader.set_uniform1f("light.l", 0.09f);
		light_shader.set_uniform1f("light.q", 0.032f);
		light_shader.set_uniformvec3("light.light_pos", glm::vec3(view_light_pos));
		light_shader.set_uniform_matrix_4fv("model", light_model);
		light_shader.set_uniform_matrix_4fv("view", camera.GetView());
		light_shader.set_uniform_matrix_4fv("projection", camera.GetProjection());
		//��ʾ��ȥִ�ж�����ɫ��36��
		glBindVertexArray(LIGHT_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glBindVertexArray(0);
	return 0;
}