/*
面剔除
*/
#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define window_width 1024
#define window_height 768

glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
glm::vec3 direction(0.0f, 0.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
Camera camera = Camera(camera_pos, direction, up);

void frame_buffer_cb(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_cb(GLFWwindow *window, int keycode, int a, int b, int c) {
	if (keycode == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

void input(GLFWwindow *window) {

}

int load_texture(const char *image_path) {

	int image_width, image_height, channel;
	//先去加载纹理图片
	unsigned char *image_data = stbi_load(image_path, &image_width, &image_height, &channel, 0);
	if (image_data == NULL)
		return -1;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int comp = GL_RGB;
	if (channel == 4)
		comp = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D,0, comp, image_width, image_height, 0, comp, GL_UNSIGNED_BYTE, image_data);
	//远近距离采用哪种图
	glGenerateMipmap(GL_TEXTURE_2D);
	//采样点越界时 采取的采样方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//放大缩小时 采取的缩放算法
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
	GLFWwindow * window = glfwCreateWindow(window_width, window_height, "FaceCulling", NULL, NULL);
	if (!window)
		glfwTerminate();
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
	}
	glViewport(0, 0, window_width, window_height);
	glfwSetFramebufferSizeCallback(window, &frame_buffer_cb);
	glfwSetKeyCallback(window, &key_cb);
	// 正方形的点 已逆时针作为正向
	float cube_vertexs[] = {
		-0.5, -0.5, 0, 0, 0,
		0.5, -0.5, 0, 1, 0,
		0.5, 0.5, 0, 1, 1,
		-0.5, -0.5, 0, 0, 0,
		0.5, 0.5, 0, 1, 1,
		-0.5, 0.5, 0, 0, 1,
	};
	// 加载纹理
	int texture = load_texture("marble.jpg");

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertexs), (void *)cube_vertexs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	//尝试将normalize归一化会怎么样？
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader cube_shader("depth_test_cube.vs", "depth_test_cube.fs");
	glEnable(GL_DEPTH_TEST);
	glm::mat4 model;
	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3, 0.3, 0.3, 0.5);
		glBindVertexArray(VAO);
		cube_shader.use_program();
		cube_shader.set_uniform_matrix_4fv("mmodel", model);
		cube_shader.set_uniform_matrix_4fv("mview", camera.GetView());
		cube_shader.set_uniform_matrix_4fv("mprojection", camera.GetProjection());
		cube_shader.set_uniform1i("t0", 0);
		if (texture != -1)
			glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}