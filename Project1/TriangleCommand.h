#pragma once
#include "RenderCommand.h"
#include <vector>
class TriangleVertex {
public:
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texture_pos;
};

class TriangleCommand : public RenderCommnad {
public:
	TriangleCommand();
	~TriangleCommand();
public:
	std::vector<TriangleVertex> &getVertexes();
	std::vector<rsize_t> &getIndices();
private:
	std::vector<TriangleVertex> _vertes;
	std::vector<size_t> _indices;
};