#pragma once
#include "GRef.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
enum RENDERTYPE
{
	DEFAULT_COMMAND,
	TRIANGLE_COMMAND,
};
class RenderCommnad : public GRef {
public:
	RenderCommnad();
	~RenderCommnad();
public:
	RENDERTYPE getRenderType() { return _render_type; }
	int getGlobalZOrder() { return _globalZOrder; }
	int getRenderID() { return _renderID; }
	void apply();
protected:
	RENDERTYPE _render_type;
	int _renderID;
private:
	glm::mat4 _mat4;
	int _globalZOrder;
};