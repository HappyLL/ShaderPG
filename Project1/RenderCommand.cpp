#include "RenderCommand.h"

RenderCommnad::RenderCommnad()
{
	_render_type = DEFAULT_COMMAND;
	_renderID = 0;
}

RenderCommnad::~RenderCommnad()
{
}

void RenderCommnad::apply()
{
}
