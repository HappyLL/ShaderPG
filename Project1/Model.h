#pragma once
#include "Mesh.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model {
public:
	void draw(Shader *shader);
	Model(char *path) {
		this->load_model(path);
	}
private:
	void load_model(char *path);
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> load_material_texures(aiMaterial *mat, aiTextureType type, std::string type_name);
	int texture_from_file(const char * texture_name);
};