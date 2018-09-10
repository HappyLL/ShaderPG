#include "GRender.h"
#include "TriangleCommand.h"
#include <algorithm>
//glad 是用于纠正显卡驱动不同导致函数地址不同
#include <glad/glad.h>
//glfw 是opengl的几个流行库中的一个
#include <glfw/glfw3.h>

static bool compareRenderCommand(RenderCommnad* a, RenderCommnad* b)
{
	return a->getGlobalZOrder() < b->getGlobalZOrder();
}

GRender::GRender()
{
	_render_queue = new RenderQueue(0);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

GRender::~GRender()
{
	_render_queue->release();
	_render_queue = nullptr;
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void GRender::render()
{
	_render_queue->sort();
	_vertexes.clear();
	auto p_cmds = _render_queue->getPositiveCommands();
	auto n_cmds = _render_queue->getNegativeCommands();
	RenderCommnad* tmp_cmd = nullptr;
	for (auto cmd : n_cmds) {
		auto _tcmd = (TriangleCommand*)cmd;
		if (!tmp_cmd) {
			tmp_cmd = cmd;
			_vertexes.insert(_vertexes.end(), _tcmd->getVertexes().begin(), _tcmd->getVertexes().end());
			_indices.insert(_indices.end(), _tcmd->getIndices().begin(), _tcmd->getIndices().end());
			continue;
		}
		if (tmp_cmd->getRenderID() != cmd->getRenderID()) {
			_drawBatchTriangles(tmp_cmd);
		}
		_vertexes.insert(_vertexes.end(), _tcmd->getVertexes().begin(), _tcmd->getVertexes().end());
		_indices.insert(_indices.end(), _tcmd->getIndices().begin(), _tcmd->getIndices().end());
		tmp_cmd = cmd;
	}

	for (auto cmd : p_cmds) {
		auto _tcmd = (TriangleCommand*)cmd;
		if (!tmp_cmd) {
			tmp_cmd = cmd;
			_vertexes.insert(_vertexes.end(), _tcmd->getVertexes().begin(), _tcmd->getVertexes().end());
			_indices.insert(_indices.end(), _tcmd->getIndices().begin(), _tcmd->getIndices().end());
			continue;
		}
		if (tmp_cmd->getRenderID() != cmd->getRenderID()) {
			_drawBatchTriangles(tmp_cmd);
		}
		_vertexes.insert(_vertexes.end(), _tcmd->getVertexes().begin(), _tcmd->getVertexes().end());
		_indices.insert(_indices.end(), _tcmd->getIndices().begin(), _tcmd->getIndices().end());
		tmp_cmd = cmd;
	}
	_drawBatchTriangles(tmp_cmd);
	_clearCommands();
}

void GRender::pushCommand(RenderCommnad * cmd)
{
	_render_queue->addCommand(cmd);
}

void GRender::_drawBatchTriangles(RenderCommnad *cmd)
{
	if (_vertexes.empty())
		return;
	// apply 表示已经绑定到默认的着色器
	cmd->apply();
	glBindVertexArray(VAO);
	glBufferData(VBO, _vertexes.size() * sizeof(TriangleVertex), &_vertexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(EBO, _indices.size() * sizeof(GL_INT), &_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void *)(offsetof(TriangleVertex, pos)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void *)(offsetof(TriangleVertex, color)));
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_INT, &_indices);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	_vertexes.clear();
	_indices.clear();
}

void GRender::_clearCommands()
{
	_render_queue->
}

RenderQueue::RenderQueue(size_t render_id)
{
	_render_id = render_id;
}

RenderQueue::~RenderQueue()
{
	clearCommands();
}

void RenderQueue::sort()
{
	std::sort(_negative_commands.begin(), _negative_commands.end(), compareRenderCommand);
	std::sort(_postive_commands.begin(), _postive_commands.end(), compareRenderCommand);
}

void RenderQueue::addCommand(RenderCommnad * cmd)
{
	if (cmd->getGlobalZOrder() < 0)
		_negative_commands.push_back(cmd);
	else
		_postive_commands.push_back(cmd);
}

void RenderQueue::clearCommands()
{
	for (auto render : _negative_commands) {
		render->release();
	}
	_negative_commands.clear();
	for (auto render : _postive_commands) {
		render->release();
	}
	_postive_commands.clear();
}
