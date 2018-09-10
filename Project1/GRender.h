#pragma once
#include "GRef.h"
#include <map>
#include "RenderCommand.h"
#include "TriangleCommand.h"
#include <vector>
class RenderQueue : public GRef{
public:
	RenderQueue(size_t render_id);
	~RenderQueue();
public:
	void sort();
	void addCommand(RenderCommnad* cmd);
	std::vector<RenderCommnad*>& getPositiveCommands() { return _postive_commands;}
	std::vector<RenderCommnad*>& getNegativeCommands() { return _negative_commands;}
	void clearCommands();
private:
private:
	size_t _render_id;
	std::vector<RenderCommnad*> _postive_commands;
	std::vector<RenderCommnad*> _negative_commands;
};

class GRender : public GRef {
public:
	GRender();
	~GRender();
public:
	void render();
	void pushCommand(RenderCommnad * rmd);
private:
	void _drawBatchTriangles(RenderCommnad * cmd);
	void _clearCommands();
private:
	RenderQueue *_render_queue;
	std::vector<TriangleVertex> _vertexes;
	std::vector<size_t> _indices;
	size_t VAO, VBO, EBO;
};