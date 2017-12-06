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
	this->check_shader(vertex_shader, GL_COMPILE_STATUS);
	//构造片段着色器
	unsigned int frage_shader;
	frage_shader = glCreateShader(GL_FRAGMENT_SHADER);
	code = this->fs_code.c_str();
	glShaderSource(frage_shader, 1, &code, NULL);
	glCompileShader(frage_shader);
	this->check_shader(frage_shader, GL_COMPILE_STATUS);
	//构造着色器程序
	this->gl_program = glCreateProgram();
	glAttachShader(this->gl_program, vertex_shader);
	glAttachShader(this->gl_program, frage_shader);
	glLinkProgram(this->gl_program);
	this->check_shader(this->gl_program, GL_LINK_STATUS);
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

void Shader::set_uniform1f(const char * name, double value)
{
	GLuint un_location = glGetUniformLocation(this->gl_program, name);
	glUniform1f(this->gl_program, value);
}

void Shader::read_shader(const char * shader_path, std::string & code)
{
	std::ifstream stream = std::ifstream(shader_path, std::ios::in);
	stream.seekg(0, std::ios::end);
	long tot_bytes = stream.tellg();
	stream.seekg(0, std::ios::beg);
	char * buff = new char[tot_bytes];
	stream.read(buff, tot_bytes);
	code = buff;
	delete[]buff;
	stream.close();
	std::cout << "read_shader is " << code << std::endl;
}

void Shader::check_shader(unsigned int shader, GLenum status)
{
	GLint success;
	glGetProgramiv(shader, status, &success);
	//返回0表示编译不成功
	if (success == 0) {
		char info[512];
		glGetShaderInfoLog(shader, 512, NULL, info);
		std::cout << info << std::endl;
	}
}

//http://blog.csdn.net/kingstar158/article/details/6859379 读文件