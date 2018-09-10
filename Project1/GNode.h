#pragma once
#include "GRef.h"
#include <vector>
class GNode : public GRef{
public:
	static GNode* create();
public:
	GNode();
	~GNode();
public:
	void visit();
	void onDraw();
	void addChild();
	void addChildHelper();
public:
	bool init();
private:
	std::vector<GNode* > _children;
	int _localZOrder;
};