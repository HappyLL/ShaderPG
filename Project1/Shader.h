#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Shader {
public:
	Shader(const char *vs, const char *fs);
	~Shader();
	void use_program();
	void set_uniform1f(const char *name, double value);
	void set_uniform1i(const char *name, int value);
private:
	void read_shader(const char *shader_path, std::string &code);
	void check_shader_link(unsigned int shader, GLenum status);
	void check_shader_compile(unsigned int shader, GLenum status);
private:
	std::string vs_code, fs_code;
	GLuint gl_program;
};