#pragma once
#include <vector>
#include "Shader.h"
struct Vertex {
	// 顶点坐标
	glm::vec3 m_position;
	// 法线坐标
	glm::vec3 m_normal;
	// 纹理位置
	glm::vec2 m_textcoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
// 网格(opengl 绘制的最小单元)
public:
	// 顶点
	std::vector<Vertex> m_vertexs;
	// 索引
	std::vector<unsigned int> m_indices;
	// 纹理
	std::vector<Texture> m_textures;

	Mesh(std::vector<Vertex> vertexs, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader *shader);

private:
	/* 渲染数据 缓冲 */
	unsigned int VAO, VBO, EBO;
	// 处理顶点缓冲数据
	void step_mesh();
};