#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertexs, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->m_vertexs = vertexs;
	this->m_indices = indices;
	this->m_textures = textures;

	this->step_mesh();
}

void Mesh::draw(Shader *shader)
{
	// 注意上下文(使用过后要清空上下文)
	//遍历纹理数组 激活纹理单元 将纹理单元绑定到着色器上
	int diffuse_texure_num = 0;
	int specular_texture_num = 0;
	for (unsigned int i = 0; i < this->m_textures.size(); ++i) {
		std::string texture_type = this->m_textures[i].type;
		int texture_num = -1;
		if (texture_type == "texture_diffuse") {
			++diffuse_texure_num;
			texture_num = diffuse_texure_num;
		}
		else if (texture_type == "texture_specular") {
			++specular_texture_num;
			texture_num = specular_texture_num;
		}
		if (texture_num == -1)
			continue;
		
		// 纹理单元0是默认激活的 所以绑定到GL_TEXTURE_2D 便是默认绑定到0上 所以要将数据绑定到那个纹理单元就激活哪个
		glActiveTexture(GL_TEXTURE0 + i);
		shader->set_uniform1i(("material." + texture_type + std::to_string(texture_num)).c_str(), i);
		// 将数据绑定到对应的纹理单元
		glBindTexture(GL_TEXTURE_2D, this->m_textures[i].id);
	}
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_INT, 0);
	//恢复到原先默认值
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::step_mesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	// C++中当结构体作为一个数据数组使用时 内存是连续的(顺序的排列结构体的变量)
	// offsetof: 返回距离结构体头部的偏移量
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->m_vertexs.size() * sizeof(Vertex), (void *)( &(this->m_vertexs[0])), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(unsigned int), &(this->m_indices[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_textcoords));
	glBindVertexArray(0);
}
