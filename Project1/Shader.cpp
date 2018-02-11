#include "Shader.h"

Shader::Shader(const char * vs, const char * fs)
{
	//io读取对应的shader文件
	this->read_shader(vs, this->vs_code);
	this->read_shader(fs, this->fs_code);
	//构造顶点着色器
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char *code = this->vs_code.c_str();
	glShaderSource(vertex_shader, 1, &code, NULL);
	glCompileShader(vertex_shader);
	this->check_shader_compile(vertex_shader, GL_COMPILE_STATUS, "vertex");
	//构造片段着色器
	unsigned int frage_shader;
	frage_shader = glCreateShader(GL_FRAGMENT_SHADER);
	code = this->fs_code.c_str();
	glShaderSource(frage_shader, 1, &code, NULL);
	glCompileShader(frage_shader);
	this->check_shader_compile(frage_shader, GL_COMPILE_STATUS, "frage");
	//构造着色器程序
	this->gl_program = glCreateProgram();
	glAttachShader(this->gl_program, vertex_shader);
	glAttachShader(this->gl_program, frage_shader);
	glLinkProgram(this->gl_program);
	this->check_shader_link(this->gl_program, GL_LINK_STATUS);
	glDeleteShader(vertex_shader);
	glDeleteShader(frage_shader);
}

Shader::~Shader()
{
	glDeleteProgram(this->gl_program);
}

void Shader::use_program()
{
	glUseProgram(this->gl_program);
}

void Shader::set_uniform1f(const char * name, float value)
{
	GLuint un_location = glGetUniformLocation(this->gl_program, name);
	glUniform1f(un_location, value);
}

void Shader::set_uniform1i(const char * name, int value)
{
	GLuint un_location = glGetUniformLocation(this->gl_program, name);
	glUniform1i(un_location, value);
}

void Shader::set_uniformvec3(const char * name, glm::vec3 &value)
{
	// 不传引用的bug 会导致传gpu值时(c/s) 可能导致当前数据被销毁 所以必须要用引用
	int un_location = glGetUniformLocation(this->gl_program, name);
	// un_location 为负数表示当前找不到对应的字段(如果赋值 会影响着色器程序)
	if (un_location < 0)
		return;
	glUniform3fv(un_location, 1, &value[0]);
}

void Shader::set_uniform_matrix_4fv(const char * name, glm::mat4 & mat)
{
	GLuint un_location = glGetUniformLocation(this->gl_program, name);
	glUniformMatrix4fv(un_location, 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader::get_uniform_location(const char * name)
{
	GLuint un_location = glGetUniformLocation(this->gl_program, name);
	return un_location;
}

void Shader::read_shader(const char * shader_path, std::string & code)
{
	std::ifstream stream = std::ifstream(shader_path, std::ios::in);
	stream.seekg(0, std::ios::end);
	long tot_bytes = stream.tellg();
	stream.seekg(0, std::ios::beg);
	char * buff = new char[tot_bytes + 1];
	// 初始化时需要全部为0 不然会出现中文乱码
	memset(buff, 0, tot_bytes + 1);
	stream.read(buff, tot_bytes);
	code = buff;
	delete[]buff;
	stream.close();
	//std::cout << "read_shader is " << code << std::endl;
}

void Shader::check_shader_link(unsigned int shader, GLenum status)
{
	GLint success;
	glGetProgramiv(shader, status, &success);
	//返回0表示不成功
	if (success == 0) {
		char info[512];
		memset(info, 0, 512);
		glGetProgramInfoLog(shader, 512, NULL, info);
		std::cout << info << std::endl;
	}
}

void Shader::check_shader_compile(unsigned int shader, GLenum status, const char * shader_name)
{
	GLint success;
	glGetShaderiv(shader, status, &success);
	//返回0表示不成功
	if (success == 0) {
		char info[512];
		memset(info, 0, 512);
		glGetShaderInfoLog(shader, 512, NULL, info);
		std::cout << shader_name << " : " << info << std::endl;
	}
}

//http://blog.csdn.net/kingstar158/article/details/6859379 读文件