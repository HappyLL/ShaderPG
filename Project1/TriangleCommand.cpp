#include "TriangleCommand.h"

TriangleCommand::TriangleCommand()
{
	_render_type = TRIANGLE_COMMAND;
}

TriangleCommand::~TriangleCommand()
{
}

std::vector<TriangleVertex>& TriangleCommand::getVertexes()
{
	// TODO: 在此处插入 return 语句
	return _vertes;
}

std::vector<rsize_t>& TriangleCommand::getIndices()
{
	// TODO: 在此处插入 return 语句
	return _indices;
}


