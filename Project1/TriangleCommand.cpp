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
	// TODO: �ڴ˴����� return ���
	return _vertes;
}

std::vector<rsize_t>& TriangleCommand::getIndices()
{
	// TODO: �ڴ˴����� return ���
	return _indices;
}


