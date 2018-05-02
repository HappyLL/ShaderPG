#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// opengl������
// .1 ��������(��ʼ��glfw��, ����汾(��,�κͽӿ�ģʽ), ��������, ��ʼ��һЩapi�ĵ�ַ, ������Ⱦѭ��)
// .2 ��������ͷ���������Ӻ͵ذ�Ķ��㻺����, ������������ �����������
// .3 ��д��ɫ�����
// .4 ������ӦƬ��
const int window_height = 640;
const int window_width = 960;

glm::vec3 camera_pos(0, 0, 3.0f);
glm::vec3 camera_dir(0, 0, -1.0f);
glm::vec3 camera_up(0.0, 1.0f, 0.0f);
Camera camera = Camera(camera_pos, camera_dir, camera_up, float(window_width * 1.0 / window_height));

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

GLuint load_texture(const char *texture_path) {
	int comp, width, height;
	stbi_uc *image_data = stbi_load(texture_path, &width, &height, &comp, 0);
	if (image_data == NULL) {
		std::cout << "image_data is null texture_path is " << texture_path << std::endl;
		return -1;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (comp == 3)
		comp = GL_RGB;
	else if (comp == 4)
		comp = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, comp, width, height, 0, comp, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	//���ò��Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//���ù��˷�ʽ(��Ӧ�����������ϵĵ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image_data);
	return texture;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Blend", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// ��Щ������������ʱȷ����ַ���������ĵĲ�ͬ����ͬ ������Ҫ��ȡ���ڵ�ַ
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, window_width, window_height);
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

	GLuint cubeVBO;
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint planeVBO;
	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), (void *)planeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),(void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
	glBindVertexArray(0);

	GLuint planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Shader cube_shader("depth_test_cube.vs", "depth_test_cube.fs");
	Shader plane_shader("depth_test_plane.vs", "depth_test_plane.fs");
	Shader grass_shader("blend_discard_cube.vs", "blend_discard_cube.fs");
	GLuint cube_texture = load_texture("marble.jpg");
	GLuint plane_texture = load_texture("metal.png");
	//������ӦƬ��
	//GLuint grass_texture = load_texture("grass.png");

	glm::mat4 model;
	glm::mat4 cube_model = glm::rotate(model, 45.0f, glm::vec3(0, 1.0, 0));
	glm::vec3 translates[] = { glm::vec3(0.0f), glm::vec3(1.5f ,0, 2.5f), glm::vec3(-1.5f, 0, -0.8f) };
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2, 0.2, 0.2, 0.5);
		// ������Ⱦ����
		for (int index = 0; index < 3; ++index) {
			glm::mat4 trans = glm::translate(cube_model, translates[index]);

			/*
			// ������ӦƬ��
			// ��Ҫ�Ȼ��Ʋ� �������� ����Ϊ��Ȳ��Խ����޳�
			grass_shader.use_program();
			grass_shader.set_uniform_matrix_4fv("mmodel", trans);
			grass_shader.set_uniform_matrix_4fv("mview", camera.GetView());
			grass_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
			grass_shader.set_uniform1i("t0", 0);
			glBindVertexArray(cubeVAO);
			if (grass_texture != -1)
				glBindTexture(GL_TEXTURE_2D, grass_texture);
			glDrawArrays(GL_TRIANGLES, 6, 6);
			*/

			cube_shader.use_program();
			cube_shader.set_uniform_matrix_4fv("mmodel", trans);
			cube_shader.set_uniform_matrix_4fv("mview", camera.GetView());
			cube_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
			cube_shader.set_uniform1i("t0", 0);
			glBindVertexArray(cubeVAO);
			if (cube_texture != -1)
				glBindTexture(GL_TEXTURE_2D, cube_texture);
			glDrawArrays(GL_TRIANGLES, 0, 36);			
		}

		plane_shader.use_program();
		plane_shader.set_uniform_matrix_4fv("mmodel", model);
		plane_shader.set_uniform_matrix_4fv("mview", camera.GetView());
		plane_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
		plane_shader.set_uniform1i("t0", 0);
		glBindVertexArray(planeVAO);
		if(plane_texture != -1)
			glBindTexture(GL_TEXTURE_2D, plane_texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glBindVertexArray(0);
	glfwTerminate();
	return 0;
}