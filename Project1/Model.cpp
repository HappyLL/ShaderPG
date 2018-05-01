#include "Model.h"

// 写在.h文件会报错 写在cpp文件就可以
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void Model::draw(Shader * shader)
{
	for (unsigned int i = 0; i < this->m_meshes.size(); ++i) {
		this->m_meshes[i].draw(shader);
	}
}

void Model::load_model(char * path)
{
	Assimp::Importer importer;
	// 标志位 将所有图元转化为三角形 和 做一个y轴的翻转
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR ASSIMP " << importer.GetErrorString() << std::endl;
		return;
	}
	std::string s = std::string(path);
	this->m_directory = s.substr(0, s.find_last_of('/'));
	this->process_node(scene->mRootNode, scene);
}
// assimp 场景数据结构: https://learnopengl-cn.github.io/03%20Model%20Loading/01%20Assimp/
void Model::process_node(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		// mMeshes[i] 表示模型的具体索引
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_meshes.push_back(this->process_mesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		aiNode *child = node->mChildren[i];
		this->process_node(child, scene);
	}
}

Mesh Model::process_mesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	//处理顶点
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		aiVector3D ai_pos = mesh->mVertices[i];
		aiVector3D ai_normal = mesh->mNormals[i];
		Vertex v;
		v.m_position.x = ai_pos.x;
		v.m_position.y = ai_pos.y;
		v.m_position.z = ai_pos.z;
		v.m_normal.x = ai_normal.x;
		v.m_normal.y = ai_normal.y;
		v.m_normal.z = ai_normal.z;
		if (mesh->mTextureCoords[0])
		{
			//可以最多包含8个纹理(都含有与顶点数相同的纹理坐标)
			v.m_textcoords.x = mesh->mTextureCoords[0][i].x;
			v.m_textcoords.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			v.m_textcoords.x = 0;
			v.m_textcoords.y = 0;
		}
		vertices.push_back(v);
	}
	//处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace ai_face = mesh->mFaces[i];
		for (unsigned int j = 0; j < ai_face.mNumIndices; ++j) {
			indices.push_back(ai_face.mIndices[j]);
		}
	}
	//处理材质
	// 需要将对应的纹理信息绑定到纹理单元中
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial * ai_material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = this->load_material_texures(ai_material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		std::vector<Texture> specular_maps = this->load_material_texures(ai_material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_texures(aiMaterial * mat, aiTextureType type, std::string type_name)
{
	std::vector<Texture> textures;
	int cnt = mat->GetTextureCount(type);
	for (int i = 0; i < cnt; ++i) {
		aiString str;
		// 获取纹理的路径
		mat->GetTexture(type, i, &str);
		int texture_id = this->texture_from_file(str.C_Str());
		if (texture_id == -1)
			continue;
		Texture texture;
		texture.id = texture_id;
		texture.type = type_name;
		texture.path = std::string(str.C_Str());
		textures.push_back(texture);
	}
	return textures;
}

int Model::texture_from_file(const char * texture_name)
{

	int img_width = 0, img_height = 0, img_channel = 1;
	std::string path = this->m_directory + '/' + std::string(texture_name);
	unsigned char * img_bytes = stbi_load(path.c_str(), &img_width, &img_height, &img_channel, 0);
	if (img_bytes == NULL)
		return -1;
	GLenum format;
	if (img_channel == 1)
		format = GL_RED;
	else if (img_channel == 3)
		format = GL_RGB;
	else if (img_channel == 4)
		format = GL_RGBA;


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, img_width, img_height, 0, format, GL_UNSIGNED_BYTE, img_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(img_bytes);
	return texture;
}
