#pragma once
#include <vector>
#include "Shader.h"
struct Vertex {
	// ��������
	glm::vec3 m_position;
	// ��������
	glm::vec3 m_normal;
	// ����λ��
	glm::vec2 m_textcoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
// ����(opengl ���Ƶ���С��Ԫ)
public:
	// ����
	std::vector<Vertex> m_vertexs;
	// ����
	std::vector<unsigned int> m_indices;
	// ����
	std::vector<Texture> m_textures;

	Mesh(std::vector<Vertex> vertexs, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader *shader);

private:
	/* ��Ⱦ���� ���� */
	unsigned int VAO, VBO, EBO;
	// �����㻺������
	void step_mesh();
};