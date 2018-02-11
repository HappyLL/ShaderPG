#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
class Shader {
public:
	Shader(const char *vs, const char *fs);
	~Shader();
	void use_program();
	void set_uniform1f(const char *name, float value);
	void set_uniform1i(const char *name, int value);
	void set_uniformvec3(const char * name, glm::vec3 & value);
	void set_uniform_matrix_4fv(const char *name, glm::mat4 &mat);
	GLuint get_uniform_location(const char *name);
private:
	void read_shader(const char *shader_path, std::string &code);
	void check_shader_link(unsigned int shader, GLenum status);
	void check_shader_compile(unsigned int shader, GLenum status, const char *);
private:
	std::string vs_code, fs_code;
	GLuint gl_program;
};