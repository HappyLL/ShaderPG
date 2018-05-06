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

Camera camera = Camera(camera_pos, camera_dir, camera_up, WIDTH * 1,0 / HEIGHT);

void frame_size_cb(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void key_code_cb(GLFWwindow *window, int keycode, int a, int b, int c) {
	if (keycode == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

void input(GLFWwindow *window) {

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
	while (!glfwWindowShouldClose(window)) {
		input(window);
		glClearColor(0.2, 0.2, 0.2, 0.5);
		glClear(GL_COLOR_BUFFER_BIT);
		// 交换
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}
