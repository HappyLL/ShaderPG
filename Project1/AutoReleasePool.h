#pragma once
#include <iostream>
#include <vector>
#include "GRef.h"
class AutoReleasePool {
public:
	AutoReleasePool();
	~AutoReleasePool();
public:
	void addGRef(GRef * object);
	void clearGRefs();
private:
	std::vector<GRef *> _refs;
};