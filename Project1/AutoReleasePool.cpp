#include "AutoReleasePool.h"

AutoReleasePool::AutoReleasePool()
{
	_refs.clear();
}

AutoReleasePool::~AutoReleasePool()
{
	this->clearGRefs();
}

void AutoReleasePool::addGRef(GRef * object)
{
	_refs.push_back(object);
}

void AutoReleasePool::clearGRefs()
{
	for(auto ref:_refs) {
		ref->release();
	}
	_refs.clear();
}
