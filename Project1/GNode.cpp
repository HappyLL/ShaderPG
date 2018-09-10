#include "GNode.h"

GNode* GNode::create(){
	GNode* node = new GNode();
	if (node && node->init()) {
		node->autoRelease();
	}
	else {
		node = nullptr;
	}
	return node;
}

GNode::GNode()
{
}

GNode::~GNode()
{
}

void GNode::visit()
{
}

void GNode::onDraw()
{
}

void GNode::addChild()
{
}

void GNode::addChildHelper()
{
}

bool GNode::init()
{
	return true;
}
